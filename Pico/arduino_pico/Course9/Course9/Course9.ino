#include <Arduino.h>
/*------------ Macro Definitions ------------*/
#define intervalTime  5
/*
Pins                             7    6     5    4     3      2      1      0
Register 0x03 0: Output Mode 1: Input Mode ECB  ECA  HALL  VIB  STEP4  STEP3  STEP2  STEP1
Configuration Register 0x03 Write 0xE0--->   1    1     1    0     0      0      0      0
Set Output Register Initial Value, Write 0x01 to Output Register 0x00
*/
// This Arduino code implements a stepper motor control program using a 4-phase 5-wire stepper motor with 8-step control method
#define pinA 22
#define pinB 26
#define pinC 27
#define pinD 28

/*------------ Variable Declarations ------------*/

/*------------ Function Declarations ------------*/
void Step1();  // Output Phase D HIGH
void Step2();  // Output Phases D and C HIGH
void Step3();  // Output Phase C HIGH
void Step4();  // Output Phases B and C HIGH
void Step5();  // Output Phase B HIGH
void Step6();  // Output Phases A and B HIGH
void Step7();  // Output Phase A HIGH
void Step8();  // Output Phases D and A HIGH
void turn(uint32_t count);        // Execute count complete cycles (360 degrees)
void turnSteps(uint32_t count);   // Execute count steps (1 step = 1/512 revolution)
void turnDegrees(uint32_t d);     // Rotate by specified degrees (0~360 degrees)
void turnDistance(uint32_t dis, uint32_t rad);  // Rotate by specified distance (requires wheel radius)
void close();


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(pinA, OUTPUT);  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);  pinMode(pinD, OUTPUT);
  digitalWrite(pinA, LOW);  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);  digitalWrite(pinD, LOW);
}

void loop() {
  turnSteps(1); // // Execute 1 step every 1 millisecond (approximately 0.7 degrees)       One cycle is one revolution
  delay(1);
}


void Step1() {
  digitalWrite(pinD, HIGH);
  delay(intervalTime);
  digitalWrite(pinD, LOW);
}
void Step2() {
  digitalWrite(pinD, HIGH);
  digitalWrite(pinC, HIGH);
  delay(intervalTime);
  digitalWrite(pinD, LOW);
  digitalWrite(pinC, LOW);
}
void Step3() {
  digitalWrite(pinC, HIGH);
  delay(intervalTime);
  digitalWrite(pinC, LOW);
}
void Step4() {
  digitalWrite(pinB, HIGH);
  digitalWrite(pinC, HIGH);
  delay(intervalTime);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
}
void Step5() {
  digitalWrite(pinB, HIGH);
  delay(intervalTime);
  digitalWrite(pinB, LOW);
}
void Step6() {
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, HIGH);
  delay(intervalTime);
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
}
void Step7() {
  digitalWrite(pinA, HIGH);
  delay(intervalTime);
  digitalWrite(pinA, LOW);
}
void Step8() {
  digitalWrite(pinD, HIGH);
  digitalWrite(pinA, HIGH);
  delay(intervalTime);
  digitalWrite(pinD, LOW);
  digitalWrite(pinA, LOW);
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
  digitalWrite(pinA, LOW);  
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);  
  digitalWrite(pinD, LOW);
}    