#include <BH1750.h>
#include <Wire.h>

#define lightSensor_Address 0x5c
#define SDAPIN    16
#define SCLPIN    17

#define Buzzer_pin 19

/*
  BH1750 can be physically configured to use two I2C addresses:
    - 0x23 (most common) (if ADD pin had < 0.7VCC voltage)
    - 0x5C (if ADD pin had > 0.7VCC voltage)

  Library uses 0x23 address as default, but you can define any other address.
  If you had troubles with default value - try to change it to 0x5C.

*/
BH1750 lightMeter(lightSensor_Address);

void setup() {
  Serial.begin(9600);
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.setSDA(SDAPIN);
  Wire.setSCL(SCLPIN);
  Wire.begin();
  /*
    BH1750 has six different measurement modes. They are divided in two groups;
    continuous and one-time measurements. In continuous mode, sensor
    continuously measures lightness value. In one-time mode the sensor makes
    only one measurement and then goes into Power Down mode.

    Each mode, has three different precisions:

      - Low Resolution Mode - (4 lx precision, 16ms measurement time)
      - High Resolution Mode - (1 lx precision, 120ms measurement time)
      - High Resolution Mode 2 - (0.5 lx precision, 120ms measurement time)

    By default, the library uses Continuous High Resolution Mode, but you can
    set any other mode, by passing it to BH1750.begin() or BH1750.configure()
    functions.

    [!] Remember, if you use One-Time mode, your sensor will go to Power Down
    mode each time, when it completes a measurement and you've read it.

    Full mode list:

      BH1750_CONTINUOUS_LOW_RES_MODE
      BH1750_CONTINUOUS_HIGH_RES_MODE (default)
      BH1750_CONTINUOUS_HIGH_RES_MODE_2

      BH1750_ONE_TIME_LOW_RES_MODE
      BH1750_ONE_TIME_HIGH_RES_MODE
      BH1750_ONE_TIME_HIGH_RES_MODE_2
  */
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, lightSensor_Address, &Wire)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }
}

void loop() {
  if (lightMeter.measurementReady(true)) {
    float lux = lightMeter.readLightLevel();
    // Serial.print("[-] Light: [");
    // Serial.print(constrain(lux, 0, 300));
    // Serial.println("] lx");

    int luxToPWM = 255 - map(constrain(lux, 0, 300), 0, 300, 0, 255);
    Serial.print("[-] Setting buzzer pwm:[");
    Serial.print(luxToPWM);
    Serial.println("]");
    analogWrite(Buzzer_pin, luxToPWM);
  }
  delay(500);
}
