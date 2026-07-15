#pragma once

/*********************** Pin Definitions (Consistent with main project config.h) ***********************/

// I2C: Touch
#define PIN_SCL           (3)
#define PIN_SDA           (4)

// Touch
#define PIN_TOUCH_INT     (2)
#define PIN_TOUCH_RST     (5)

// Screen, Backlight
#define PIN_LCD_EN        (40)          // Power enable for display, touch, and backlight; active LOW
#define PIN_LCD_BL        (13)          // Backlight
#define PIN_LCD_TE        (21)
#define PIN_LCD_RST       (6)
#define PIN_LCD_DC        (10)
#define PIN_LCD_CS        (9)
#define PIN_LCD_CLK       (7)
#define PIN_LCD_MOSI      (8)
#define PIN_LCD_MISO      (-1)

// Power Management
#define PIN_POWER_CTL     (47)

/*********************** Display Definitions ***********************/
#define DISPLAY_WIDTH     240
#define DISPLAY_HEIGHT    296
#define DISPLAY_SPI_FREQ  (80 * 1000 * 1000)

/*********************** Display Definitions ***********************/
