/*****************************************************************
File:        readTDS
Description: power on to obtain the current TDS1/NTC1 values directly.
             After successful reception, print the current TDS1/NTC1
             value every second in the serial port,Please ensure that
             the baud rate of the Serial Monitor is 9600.
******************************************************************/
#include "BM25S4021-1.h"
#define INT_PIN 7
#define TX_PIN 6
#define RX_PIN 5
BM25S4021_1 myTDS(INT_PIN,RX_PIN,TX_PIN);//SoftwareSerial RX TX pin of arduino UNO and BMduino
// BM25S4021_1 myTDS(STATUS1, &Serial1); //STAUTS1(D22), HardwareSerial of BMduino BMCOM1
// BM25S4021_1 myTDS(STATUS2, &Serial2); //STAUTS1(D25), HardwareSerial of BMduino BMCOM2
uint8_t channel, ID;
float TDSValue;
float TempValue;

void setup()
{
  ID = 1;
  myTDS.begin();
  Serial.begin(9600);
  delay(600);
}

void loop()
{
  channel = 1; // CH1
  TDSValue = myTDS.readTDS(ID, channel);
  Serial.print("TDS");
  Serial.print(channel);
  Serial.print(" value is ");
  Serial.print(TDSValue, 1);
  Serial.print(" ppm");
  Serial.print("(");
  Serial.print(TDSValue * 2, 1);
  Serial.println(" uS/cm)");
  TempValue = myTDS.readTemp(ID, channel);
  if (TempValue == 6548.6)
  {
    Serial.print("NTC interface is open circuit");
  }
  else if (TempValue == 150)
  {
    Serial.print("NTC interface is short circuit");
  }
  Serial.print("temperature");
  Serial.print(" value is ");
  Serial.print(TempValue, 1);
  Serial.println(" ℃");
  delay(1000);
}