/*****************************************************************
File:         setID
Description:  Reset the modular,Please ensure that the baud rate
              of the Serial Monitor is 115200.
******************************************************************/

#include <BM25S4021_1.h>
#include <SoftwareSerial.h>
//BM25S4021_1 myTDS(7,6);//SoftwareSerial RX TX pin of arduino UNO and BMduino
BM25S4021_1 myTDS(&Serial1);//HardwareSerial of BMduino BMCOM1
uint8_t oldID;
uint8_t newID;
void setup() 
{
    oldID = 1;
    newID = 2;
    myTDS.begin();
    Serial.begin(115200);
    delay(600);      
}

void loop() 
{               
    myTDS.setID(oldID,newID);
    Serial.println("modular new ID has set"); 
    //Serial.println(myTDS.setID(oldID,newID));
    while(1);                                                                   
}
