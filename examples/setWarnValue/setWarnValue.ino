/*****************************************************************
File:         setWarnValue
Description:  Reset the modular,Please ensure that the baud rate
              of the Serial Monitor is 9600.
******************************************************************/
#include "BM25S4021-1.h"
#define INT_PIN 7
#define TX_PIN 6
#define RX_PIN 5
BM25S4021_1 myTDS(INT_PIN,RX_PIN,TX_PIN);//SoftwareSerial RX TX pin of arduino UNO and BMduino
// BM25S4021_1 myTDS(STATUS1, &Serial1); //STAUTS1(D22), HardwareSerial of BMduino BMCOM1
// BM25S4021_1 myTDS(STATUS2, &Serial2); //STAUTS1(D25), HardwareSerial of BMduino BMCOM2
uint8_t channel, ID;
float warnValue;
void setup()
{
  ID = 1;
  channel = 1;
  warnValue = 500;
  myTDS.begin();
  Serial.begin(9600);
  delay(600);
}

void loop()
{
  myTDS.setAlarmValue(ID, channel, warnValue);
  Serial.print("Alarm value:");
  Serial.print(warnValue, 1);
  Serial.println("ppm has set");
  warnValue = 0;
  warnValue = myTDS.getAlarmValue(ID, channel);
  Serial.print("Get alarm value:");
  Serial.println(warnValue, 1);
  while (1)
    ;
}