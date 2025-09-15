/*****************************************************************
File:         setWorkMode.ino
Description:  Put the module into sleep,Please ensure
              that the baud rate of the Serial Monitor is 9600.
******************************************************************/
#include "BM25S4021-1.h"
#define INT_PIN 7
#define TX_PIN 6
#define RX_PIN 5
BM25S4021_1 myTDS(INT_PIN,RX_PIN,TX_PIN);//SoftwareSerial RX TX pin of arduino UNO and BMduino
// BM25S4021_1 myTDS(STATUS1, &Serial1); //STAUTS1(D22), HardwareSerial of BMduino BMCOM1
// BM25S4021_1 myTDS(STATUS2, &Serial2); //STAUTS1(D25), HardwareSerial of BMduino BMCOM2
uint8_t ID, status;
void setup()
{
  ID = 1;
  myTDS.begin();
  Serial.begin(9600);
  delay(600);
}

void loop()
{
  myTDS.setWorkMode(ID, SLEEP_MODE);
  Serial.println("Module has shut off");
  status = myTDS.getWorkMode(ID);
  Serial.print("Work mode is ");
  Serial.println(status);
  while (1)
    ;
}