/*****************************************************************
File:         sleep
Description:  Put the module into sleep,Please ensure 
              that the baud rate of the Serial Monitor is 115200.
******************************************************************/

#include <BM25S4021_1.h>
#include <SoftwareSerial.h>
//BM25S4021_1 myTDS(7,6);//SoftwareSerial RX TX pin of arduino UNO and BMduino
BM25S4021_1 myTDS(&Serial1);//HardwareSerial of BMduino BMCOM1
uint8_t ID,status;
void setup() {
    ID = 1;
    myTDS.begin();
    Serial.begin(115200);
    delay(600);      
}

void loop() {               
    myTDS.setWorkMode(ID,SLEEP_MODE);
    Serial.println("Modular has shut off"); 
    status = myTDS.getWorkMode(ID);
    Serial.print("Work mode is "); 
    Serial.println(status); 
    while(1);                                                                   
}