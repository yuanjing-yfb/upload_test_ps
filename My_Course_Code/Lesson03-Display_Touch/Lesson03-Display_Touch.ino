/*
 * Lesson 3: Screen Display and Touch
 *
 * Function:
 * Initial screen displays 0
 * Each time the screen is touched, the number increments by 1
 * After reaching 10 touches, the screen starts full-screen Red / Green / Blue color cycling
 *
 * Hardware: ESP32-Watch (V0.3 / V0.4)
 * Development Environment: Arduino IDE 2.3.6+, ESP32 Board v3.3.3
 *
 * Dependent Libraries: LVGL, Arduino_GFX (Uses the main project libraries directory)
 */

#include <lvgl.h>                                                     // Include LittlevGL graphics library
#include <Arduino_GFX_Library.h>                                      // Include Arduino graphics driver library
#include "config.h"                                                   // Include project hardware configuration
#include "esp_lcd_touch_axs5106l.h"                                   // Include touch controller driver

/*********************** Display and Touch Global Variables ***********************/
Arduino_DataBus *display_bus;                                         // Data bus pointer for display communication
Arduino_GC9309 *gfx;                                                  // Graphics driver instance pointer for GC9309 display

lv_disp_draw_buf_t draw_buf;                                          // LVGL display draw buffer structure
lv_color_t *disp_draw_buf;                                            // Pointer to the actual pixel buffer allocation
lv_disp_drv_t disp_drv;                                               // LVGL display driver configuration structure

SemaphoreHandle_t te_sync_semaphore;                                  // Binary semaphore used for Tearing Effect (TE) line synchronization
SemaphoreHandle_t lvgl_mutex;                                         // Recursive mutex used for thread-safe LVGL operations
SemaphoreHandle_t touch_mutex;                                        // Mutex used to guard I2C read operations of the touch IC

lv_obj_t *count_label = NULL;                                         // Pointer to the LVGL label object tracking touch counts

int touch_count = 0;                                                  // Counter tracking total valid touch inputs
bool color_cycle_mode = false;                                        // Status flag: true if color rotation mode is active

uint8_t color_index = 0;                                              // Array index for the current active color in rotation
uint32_t last_color_ms = 0;                                           // Keeps track of the last time a color change occurred (in milliseconds)
const uint32_t COLOR_INTERVAL_MS = 500;                               // Color swap rate threshold set to 500 milliseconds

const uint32_t color_array[] = {
  0xFF0000,  // Red
  0x00FF00,  // Green
  0x0000FF,  // Blue
};

/*********************** LVGL Mutex ***********************/
// Locks the LVGL environment to prevent race conditions during concurrent tasks
bool lvgl_port_lock(uint32_t timeout_ms) {
  const TickType_t timeout_ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms); // Handle timeout limits
  return xSemaphoreTakeRecursive(lvgl_mutex, timeout_ticks) == pdTRUE; // Safely lock resource recursively
}

// Unlocks the LVGL environment allowing other tasks or loops to update UI
void lvgl_port_unlock(void) {
  xSemaphoreGiveRecursive(lvgl_mutex);                                // Release recursive lock
}

/*********************** TE Sync Interrupt ***********************/
// Hardware ISR triggered on falling edge of the display's TE line to prevent screen tearing
void IRAM_ATTR te_isr_handler() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;                      // Context switch flag initialized to false
  xSemaphoreGiveFromISR(te_sync_semaphore, &xHigherPriorityTaskWoken); // Unlock semaphore within ISR context
  if (xHigherPriorityTaskWoken == pdTRUE) {
    portYIELD_FROM_ISR();                                             // Request immediate context switch if higher priority task was woken
  }
}

/*********************** Display Flush Callback ***********************/
// Hook registered to LVGL to render processed VDB blocks into the physical hardware VRAM
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
  if (!gfx) {                                                         // Safety verification if graphics engine is uninitialized
    lv_disp_flush_ready(disp_drv);                                    // Immediately tell LVGL flushing completed to avoid hang
    return;
  }

  uint32_t w = (area->x2 - area->x1 + 1);                             // Extract rectangle block bounding width
  uint32_t h = (area->y2 - area->y1 + 1);                             // Extract rectangle block bounding height

  xSemaphoreTake(te_sync_semaphore, 0);                               // Flush any older or stale unhandled TE signals
  if (xSemaphoreTake(te_sync_semaphore, 100) == pdTRUE) {             // Wait up to 100ms for a fresh hardware VSYNC / TE pulse
    ;                                                                 // Token executed immediately when hardware asserts ready state
  }

#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h); // Draw swapped Big Endian 16-bit format
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);   // Draw normal 16-bit RGB canvas pixels
#endif

  lv_disp_flush_ready(disp_drv);                                      // Notify LVGL that the rendering buffer pipeline is ready for reuse
}

/*********************** Touch Read Callback ***********************/
// Routine queried by LVGL core engine at intervals to monitor input device coordinate changes
void touchpad_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
  touch_data_t touch_data;                                            // Local storage structure for decoded touch metrics

  xSemaphoreTake(touch_mutex, portMAX_DELAY);                         // Acquire structural lock to guard the shared hardware I2C bus
  bsp_touch_read();                                                   // Process low-level controller register maps over I2C
  xSemaphoreGive(touch_mutex);                                        // Free up the bus for other peripheral communication

  bool touchpad_pressed = bsp_touch_get_coordinates(&touch_data);     // Evaluate coordinates validity state

  if (touchpad_pressed) {
    data->point.x = touch_data.coords[0].x;                           // Set raw relative coordinate x matrix data map onto LVGL structures
    data->point.y = touch_data.coords[0].y;                           // Set raw relative coordinate y matrix data map onto LVGL structures
    data->state = LV_INDEV_STATE_PRESSED;                             // Assign status macro to marked button state
  } else {
    data->state = LV_INDEV_STATE_RELEASED;                            // Reset status macro back to standard idling released stage
  }
}

/*********************** Backlight Control ***********************/
// Sets the PWM signal duty cycle to modulate backlight panel illumination output
void set_backlight_brightness(uint8_t brightness_percent) {
  if (brightness_percent > 100) {                                     // Limit bounds check threshold parameters
    brightness_percent = 100;                                         // Prevent unexpected overflows beyond max scale
  }
  uint32_t duty = brightness_percent * 255 / 100;                     // Recalculate percentage maps onto an 8-bit scale factor (0-255)
  analogWrite(PIN_LCD_BL, duty);                                      // Generate constant output frequency matching recalculated hardware constraints
}

/*********************** Display Initialization ***********************/
// Set up raw peripheral IO parameters and structural hardware handles for displaying data
void display_init() {
  te_sync_semaphore = xSemaphoreCreateBinary();                       // Create hardware-tied synchronization interface instance
  if (te_sync_semaphore == NULL) {                                    // Guard checking system memory failures
    Serial.println("FATAL: Semaphore creation failed!");              // Print panic diagnostics
    while (1);                                                        // Halt pipeline system forever
  }

  pinMode(PIN_LCD_TE, INPUT_PULLUP);                                  // Establish hardware TE logic level line mapping
  attachInterrupt(digitalPinToInterrupt(PIN_LCD_TE), te_isr_handler, FALLING); // Bind low edge falling transition handler

  display_bus = new Arduino_ESP32SPIDMA(
    PIN_LCD_DC, PIN_LCD_CS, PIN_LCD_CLK, PIN_LCD_MOSI, PIN_LCD_MISO, HSPI, true); // Instantiate low-level high speed DMA channel block

  gfx = new Arduino_GC9309(
    display_bus, PIN_LCD_RST, 0, false,
    DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0, 0, 0);                       // Map panel specific specifications configuration parameters

  if (!gfx->begin(DISPLAY_SPI_FREQ)) {                                // Initialize master clock sequences
    Serial.println("gfx->begin() failed!");                           // Warn structural pipeline exceptions over serial console
  }
  gfx->setRotation(0);                                                // Force absolute orientation configuration maps matrix
  gfx->fillScreen(RGB565_BLACK);                                      // Clear stale frame memory out with raw black values mapping 
}

/*********************** Touch Initialization ***********************/
// Bring up peripheral connection interfaces to the touch control driver chip
void touch_init() {
  touch_mutex = xSemaphoreCreateMutex();                              // Instantiate structural mutual exclusion flag guard
  bsp_touch_init(&Wire, PIN_TOUCH_RST, PIN_TOUCH_INT, gfx->getRotation(), gfx->width(), gfx->height()); // Initialize underlying controller via I2C mapping matching display limits
}

/*********************** LVGL Initialization ***********************/
// Handle system allocations and structural driver properties registration for LVGL core library
void lvgl_init() {
  lvgl_mutex = xSemaphoreCreateRecursiveMutex();                      // Instantiate safe engine mutex handle loops
  lv_init();                                                          // Launch internally managed sub-routines tracking graphic states

  uint32_t screenWidth = gfx->width();                                // Gather layout boundary limits metrics matching physical width
  uint32_t screenHeight = gfx->height();                              // Gather layout boundary limits metrics matching physical height
  uint32_t bufSize = screenWidth * screenHeight;                      // Determine absolute maximum buffer volume bounds allocation constraints

  disp_draw_buf = (lv_color_t *)malloc(bufSize * sizeof(lv_color_t)); // Dynamically request memory pool matching matrix scale dimensions
  if (!disp_draw_buf) {                                               // Check allocation constraints sanity checks
    Serial.println("LVGL disp_draw_buf malloc failed!");              // Output diagnosis failure
    return;                                                           // Terminate initialization sequence due to lack of heap resources
  }

  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, bufSize);     // Setup frame layout variables with single buffer layout

  lv_disp_drv_init(&disp_drv);                                        // Prepare base default configurations map parameters context
  disp_drv.hor_res = screenWidth;                                     // Map display width to driver configuration metadata entries
  disp_drv.ver_res = screenHeight;                                    // Map display height to driver configuration metadata entries
  disp_drv.flush_cb = my_disp_flush;                                  // Attach execution block for copying buffers to device panels
  disp_drv.draw_buf = &draw_buf;                                      // Assign processing allocations variables reference fields
  disp_drv.direct_mode = true;                                        // Enable strict layout canvas rendering features optimization properties
  lv_disp_drv_register(&disp_drv);                                    // Apply system profile values directly down inside layout registry tracking blocks

  static lv_indev_drv_t indev_drv;                                    // Establish persistent static handler container variables tracks matching layouts
  lv_indev_drv_init(&indev_drv);                                      // Initialize standard tracking metrics default setups structures layout
  indev_drv.type = LV_INDEV_TYPE_POINTER;                             // Explicitly define input context behaves as generic mouse cursor / tactile inputs
  indev_drv.read_cb = touchpad_read_cb;                              // Direct interaction checking triggers down onto custom peripheral reading methods
  lv_indev_drv_register(&indev_drv);                                  // Register operational tactile touch panel interface down internally

  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN); // Force basic active display canvas to black
}

/*********************** Interface and Touch Events ***********************/
// Trigger target block managed synchronously whenever full screen clicked events surface out from framework core
void on_screen_clicked(lv_event_t *event) {
  if (color_cycle_mode) {                                             // Exit early if the threshold logic has already triggered the cycling loop animation phase
    return;                                                           // Ignore ongoing interaction clicks inputs during display loop colors
  }

  touch_count++;                                                      // Increment valid hit detection tracked parameters integers variables
  lv_label_set_text_fmt(count_label, "%d", touch_count);              // Refresh displayed numerical value mapping matching counts values directly
  Serial.printf("Touch Count: %d\n", touch_count);                    // Sync numerical status logs over hardware console tracking lines

  if (touch_count >= 10) {                                            // Detect when hits metrics matches target condition configurations definitions limits
    color_cycle_mode = true;                                          // Flag global loop changes rules variables context sets
    lv_obj_add_flag(count_label, LV_OBJ_FLAG_HIDDEN);                 // Wipe counting label from layout hierarchy view layers visibly
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(color_array[0]), LV_PART_MAIN); // Set the display canvas background color to the first layout array element (Red)
    color_index = 1;                                                  // Advance rotation index markers parameters forwards to next layout items track
    last_color_ms = millis();                                         // Record historical runtime markers tracking system execution timelines tracking profiles
    Serial.println("Touch reached 10 times, starting RGB color cycling"); // Signal status changes across active console logs tracks
  }
}

// Configures and mounts graphic UI layout modules inside the active root screen node
void ui_init() {
  // Make the entire screen clickable to capture touch counts globally
  lv_obj_add_flag(lv_scr_act(), LV_OBJ_FLAG_CLICKABLE);               // Apply flag to make screen node listen for inputs
  lv_obj_add_event_cb(lv_scr_act(), on_screen_clicked, LV_EVENT_CLICKED, NULL); // Register internal listener tracking click states

  // Display number centered, initial value is 0
  count_label = lv_label_create(lv_scr_act());                        // Instantiate a label widget inside active display view layouts
  lv_label_set_text(count_label, "0");                                // Map default display characters strings values fields properties
  lv_obj_set_style_text_color(count_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN); // Configure typography colors hex values to White
  lv_obj_set_style_text_font(count_label, &lv_font_montserrat_40, LV_PART_MAIN); // Assign large structural display typography font types profile definitions
  lv_obj_center(count_label);                                         // Adjust positioning offsets rules to anchor directly in center axis parameters
}

// Shifts active target scene colors based on elapsed timeline comparisons
void update_color_cycle() {
  uint32_t now = millis();                                            // Capture current uptime duration measurements metrics values ticks fields
  if (now - last_color_ms < COLOR_INTERVAL_MS) {                      // Check whether defined loop animation time boundaries has fully elapsed yet
    return;                                                           // Skip update block sequence execution loops until time window passes completely
  }
  last_color_ms = now;                                                // Advance historical timeline marker measurements ahead to current loop times limits

  if (lvgl_port_lock(0)) {                                            // Secure thread safety locks context loops before operating inside data structures
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(color_array[color_index]), LV_PART_MAIN); // Refresh display color matching modern targeted indexed parameters
    color_index = (color_index + 1) % (sizeof(color_array) / sizeof(color_array[0])); // Cycle index securely using array bounds modulo constraints formulas
    lvgl_port_unlock();                                               // Free up thread safety resource locks properties interfaces immediately
  }
}

/*********************** Arduino Setup Loop ***********************/
void setup() {
  Serial.begin(115200);                                               // Initialize hardware hardware monitoring serial link parameters configuration rates

  // Power and Backlight (Consistent with the main project configuration)
  pinMode(PIN_POWER_CTL, OUTPUT);                                     // Declare pin mode state configurations settings parameters rules structures maps
  digitalWrite(PIN_POWER_CTL, HIGH);                                  // Assert logic high state output to deliver stable power levels lines to chips

  pinMode(PIN_LCD_EN, OUTPUT);                                        // Initialize power enabling line configuration tracks matching panel requirements
  digitalWrite(PIN_LCD_EN, LOW);                                      // Pull output line logic low state to toggle display component operational sequences

  Wire.begin(PIN_SDA, PIN_SCL);                                       // Boot master I2C protocol hardware interfaces pins layouts parameters setups maps

  display_init();                                                     // Launch physical layout screens setups instructions sequences loops blocks sets
  touch_init();                                                       // Boot physical input tracking tactile panel controller configurations layouts tracks
  lvgl_init();                                                        // Build system allocations metrics structures properties tracking software layers

  if (lvgl_port_lock(0)) {                                            // Secure access lock parameters prior to building widgets trees layouts profiles
    ui_init();                                                        // Instantiate structural display layout graphical component objects variables instances trees
    lvgl_port_unlock();                                               // Release processing locks interfaces tracks safely back to loop environments
  }

  set_backlight_brightness(100);                                      // Set the backlight illumination level to max output parameters definitions scale maps

  Serial.println();
  Serial.println("====== Lesson 3: Screen Display and Touch ======");
  Serial.println("Touch the screen, the number increments by +1 each time");
  Serial.println("After reaching 10 times, full-screen Red/Green/Blue cycling begins");
  Serial.println("================================================");
}

/*********************** Arduino Main Loop ***********************/
void loop() {
  if (color_cycle_mode) {                                             // Evaluate condition tracks updates loops conditions
    update_color_cycle();                                             // Execute timed background color transitions routines tracks fields
  }

  if (lvgl_port_lock(0)) {                                            // Guard core scheduler cycles loops processing structures variables setups safely
    lv_timer_handler();                                               // Command internal engine to check timers and refresh layout changes context
    lvgl_port_unlock();                                               // Drop recursive structural lock permissions mappings checks fields lines
  }

  delay(5);                                                           // Release processor slice thread execution context to core task manager scheduler sequences
}
