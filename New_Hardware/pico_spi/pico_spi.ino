#include <SPI.h>

// 定义 SPI 片选引脚
#define SS_PIN 13
#define RockerX_Channel  0x02
#define RockerY_Channel   0x01
#define Key_Channel  0x03
#define Voltage_Channel   0x04


byte ReadChannel(byte dataToSend) {
  // 选择从设备
  digitalWrite(SS_PIN, LOW);
  // 发送数据
  byte receivedData = SPI1.transfer(dataToSend);
  // 释放从设备
  digitalWrite(SS_PIN, HIGH);
  return receivedData;
}

void setup() {
  // 初始化 SPI
  SPI1.begin();
  // 设置 SPI 时钟频率为 5MHz，MSB 先传，SPI_MODE0 模式
  SPI1.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  // 设置 SS 引脚为输出
  pinMode(SS_PIN, OUTPUT);
  // 初始化串口通信，用于调试
  Serial.begin(115200);
}

void loop() {
  byte receivedRockerXData = ReadChannel(RockerX_Channel);
  delay(100);
  byte receivedRockerYData = ReadChannel(RockerY_Channel);
  delay(100);
//  byte receivedKeyData = ReadChannel(Key_Channel);
//  delay(100);
//  byte receivedVoltageData = ReadChannel(Voltage_Channel);
//  delay(100);
  // 打印接收到的数据
  Serial.print("receivedRockerXData: ");
  Serial.println(receivedRockerXData);
  Serial.print("receivedRockerYData: ");
  Serial.println(receivedRockerYData);
//  Serial.print("receivedKeyData: ");
//  Serial.println(receivedKeyData);
//  Serial.print("receivedVoltageData: ");
//  Serial.println(receivedVoltageData);
  delay(100);
}
