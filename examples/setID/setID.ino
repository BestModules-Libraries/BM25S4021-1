/*****************************************************************
File:         setID
Description:  Set module ID,Please ensure that the baud rate
              of the Serial Monitor is 9600.
******************************************************************/
#include "BM25S4021-1.h"
#define INT_PIN 7
#define TX_PIN 6
#define RX_PIN 5
BM25S4021_1 myTDS(INT_PIN,RX_PIN,TX_PIN);//SoftwareSerial RX TX pin of arduino UNO and BMduino
// BM25S4021_1 myTDS(STATUS1, &Serial1); //STAUTS1(D22), HardwareSerial of BMduino BMCOM1
// BM25S4021_1 myTDS(STATUS2, &Serial2); //STAUTS1(D25), HardwareSerial of BMduino BMCOM2
uint8_t oldID;
uint8_t newID;
void setup()
{
  oldID = 1;
  newID = 2;
  myTDS.begin();
  Serial.begin(9600);
  delay(600);
}

void loop()
{
  if (myTDS.setID(oldID, newID)== CHECK_OK)
  {
    Serial.println("ID set ok");
  }
  else
  {
    Serial.println("ID set fail");
  }
  while (1)
    ;
}
