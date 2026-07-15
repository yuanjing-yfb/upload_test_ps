#include <Wire.h>
#include <TCA9534.h>

/*------------ Macro definition ------------*/
#define ADDRESS  0x3c    //  0x3c---60

#define INPUT_REG     0x00
#define OUTPUT_REG    0x01
#define POLARITY_REG  0x02
#define CONFIG_REG    0x03

#define intervalTime  5

/*  TCA9534 iic Extended GPIO
pin                                         7    6     5    4     3      2      1      0
0x03register 0: OUTPUT mode 1: INPUT mode  ECB  ECA  HALL  VIB  STEP4  STEP3  STEP2  STEP1
configuration register 0x03write0xE0--->    1    1     1    0     0      0      0      0
Set initial value of output register, write 0x01 output register 0x00
*/
#define pinA 0
#define pinB 1
#define pinC 2
#define pinD 3
#define PI 3.14

/*------------ variable declaration ------------*/
TCA9534 ioex;

/*------------ function declaration ------------*/
void Step1();
void Step2();
void Step3();
void Step4();
void Step5();
void Step6();
void Step7();
void Step8();
void turn(uint32_t count);
void turnSteps(uint32_t count);
void turnDegrees(uint32_t d);
void turnDistance(uint32_t dis, uint32_t rad);
void close();


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  Wire.begin();
  ioex.attach(Wire);
  ioex.setDeviceAddress(ADDRESS);
  ioex.config(0, TCA9534::Config::OUT);
  ioex.config(1, TCA9534::Config::OUT);
  ioex.config(2, TCA9534::Config::OUT);
  ioex.config(3, TCA9534::Config::OUT);

  Serial.print("Configures the value of the register-->");
  Serial.println(ioex.config(), HEX);
  Serial.print("Output the value of the register-->");
  Serial.println(ioex.output(), HEX);
}

void loop() {
  turnSteps(1);
  delay(1);
}


void Step1() {
  ioex.output(pinD, TCA9534::Level::H);
  delay(intervalTime);
  ioex.output(pinD, TCA9534::Level::L);
}
void Step2() {
  ioex.output(pinD, TCA9534::Level::H);
  ioex.output(pinC, TCA9534::Level::H);
  delay(intervalTime);
  ioex.output(pinD, TCA9534::Level::L);
  ioex.output(pinC, TCA9534::Level::L);
}
void Step3() {
  ioex.output(pinC, TCA9534::Level::H);
  delay(intervalTime);
  ioex.output(pinC, TCA9534::Level::L);
}
void Step4() {
  ioex.output(pinB, TCA9534::Level::H);
  ioex.output(pinC, TCA9534::Level::H);
  delay(intervalTime);
  ioex.output(pinB, TCA9534::Level::L);
  ioex.output(pinC, TCA9534::Level::L);
}
void Step5() {
  ioex.output(pinB, TCA9534::Level::H);
  delay(intervalTime);
  ioex.output(pinB, TCA9534::Level::L);
}
void Step6() {
  ioex.output(pinA, TCA9534::Level::H);
  ioex.output(pinB, TCA9534::Level::H);
  delay(intervalTime);
  ioex.output(pinA, TCA9534::Level::L);
  ioex.output(pinB, TCA9534::Level::L);
}
void Step7() {
  ioex.output(pinA, TCA9534::Level::H);
  delay(intervalTime);
  ioex.output(pinA, TCA9534::Level::L);
}
void Step8() {
  ioex.output(pinD, TCA9534::Level::H);
  ioex.output(pinA, TCA9534::Level::H);
  delay(intervalTime);
  ioex.output(pinD, TCA9534::Level::L);
  ioex.output(pinA, TCA9534::Level::L);
}

void turn(uint32_t count) {
  for(int i = 0; i < count; i++) {
    Step1();
    Step2();
    Step3();
    Step4();
    Step5();
    Step6();
    Step7();
    Step8();
  }
}
void turnSteps(uint32_t count) {
  for(int i = 0; i < count; i++) {
    turn(1);
  }
}
void turnDegrees(uint32_t d) {
  turn((uint32_t)(d*512)/360);
}
void turnDistance(uint32_t dis, uint32_t rad) {
  turn((uint32_t)((512*dis)/(2*PI*rad)));
}
void close() {
  ioex.output(pinA, TCA9534::Level::L);
  ioex.output(pinB, TCA9534::Level::L);
  ioex.output(pinC, TCA9534::Level::L);
  ioex.output(pinD, TCA9534::Level::L);
}
