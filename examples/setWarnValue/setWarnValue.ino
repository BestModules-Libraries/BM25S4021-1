/*****************************************************************
File:         setWarnValue
Description:  Reset the modular,Please ensure that the baud rate
              of the Serial Monitor is 115200.
******************************************************************/

#include <BM25S4021_1.h>
#include <SoftwareSerial.h>
//BM25S4021_1 myTDS(7,6);//SoftwareSerial RX TX pin of arduino UNO and BMduino
BM25S4021_1 myTDS(&Serial1);//HardwareSerial of BMduino BMCOM1
uint8_t channel,ID;
float warnValue;
void setup() 
{
    ID = 1;
    channel = 1;
    warnValue = 500;
    myTDS.begin();
    Serial.begin(115200);
    delay(600);      
}

void loop() 
{               
	myTDS.setWarn(ID,channel,warnValue);   
	Serial.print("Alarm value:");
	Serial.print(warnValue,1);
	Serial.println("ppm has set"); 
	warnValue = 0;
	warnValue = myTDS.getWarn(ID,channel);
	Serial.print("Get alarm value:");
	Serial.println(warnValue,1);
	while(1);                                                                   
}