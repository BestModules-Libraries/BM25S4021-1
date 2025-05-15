/*****************************************************************
File:					BM25S4021-1.h
Author:				BEST MODULES CORP.
Description:	Define classes and required variables
History:			V1.0.1   -- 2025-05-09
******************************************************************/
#ifndef BM25S4021_1_h_
#define BM25S4021_1_h_
#include <Arduino.h>
#include <SoftwareSerial.h>

class BM25S4021_1
{
public:
	BM25S4021_1(uint8_t intPin, HardwareSerial *theSerial = &Serial);
	BM25S4021_1(uint8_t intPin, uint8_t rxPin, uint8_t txPin);
	void begin();
	uint8_t getINT();
	float readTDS(uint8_t ID, uint8_t channel);
	float readTemp(uint8_t ID, uint8_t channel);
	float getAlarmValue(uint8_t ID, uint8_t channel);
	uint8_t getWorkMode(uint8_t ID);
	uint8_t reset(uint8_t ID);
	uint8_t setID(uint8_t oldID, uint8_t newID);
	uint8_t setAlarmValue(uint8_t ID, uint8_t channel, float warnValue);
	uint8_t setWorkMode(uint8_t ID, uint8_t mode);

private:
	uint8_t readData(uint8_t ID, uint8_t channel, float *getTDSValue, float *getTempValue);
	uint8_t getData(uint8_t *buff, uint8_t cmmand, uint8_t ID, uint8_t rxLength);
	uint8_t checksum;
	uint8_t _intPin;
	uint8_t _rxPin;
	uint8_t _txPin;
	HardwareSerial *_hardSerial;
	SoftwareSerial *_softSerial;
};
// Communication check code
#define CHECK_OK uint8_t(0Xaa)			 /*Correct communication*/
#define CHECK_FAIL uint8_t(0Xbb)		 /*Error communication*/
#define NO_MODULE uint8_t(0X01)			 /*No module access*/
#define NO_TDS_MODULE uint8_t(0X02)	 /*No TDS module access*/
#define TIMEOUT uint8_t(0X03)				 /*Communication timeout*/
#define CHECKSUM_ERROR uint8_t(0X04) /*Receive checksum error*/
#define COMMAND_ERROR uint8_t(0X05)	 /*The receive command is incorrect*/
#define CHANNEL_ERROR uint8_t(0X06)	 /*the input channel is incorrect*/
#define ID_ERROR uint8_t(0X07)			 /*the return ID is incorrect*/

#define SLEEP_MODE uint8_t(0X00)
#define CH1_MODE uint8_t(0X01)
#define CH2_MODE uint8_t(0X02)
#define TWO_CH_MODE uint8_t(0X03)

#define SET uint8_t(0X00)
#define GET uint8_t(0X01)
// Read data command

#define HEAD1 uint8_t(0x42)
#define HEAD2 uint8_t(0x4D)
#define CLASS uint8_t(0x61)
#define DEFAULTID uint8_t(0X01)

#define CMD_GETCON uint8_t(0x01)
#define GETCON_TX_LENGTH uint8_t(0x01)
#define GETCON_RX_LENGTH uint8_t(0x05)

#define CMD_WARN uint8_t(0x02)
#define WARN_TX_LENGTH1 uint8_t(0x04)
#define WARN_RX_LENGTH1 uint8_t(0x02)
#define WARN_TX_LENGTH2 uint8_t(0x01)
#define WARN_RX_LENGTH2 uint8_t(0x03)

#define CMD_SETID uint8_t(0X05)
#define SETID_TX_LENGTH uint8_t(0X01)
#define SETID_RX_LENGTH uint8_t(0X00)

#define CMD_WORKMODE uint8_t(0X06)
#define WORKMODE_TX_LENGTH1 uint8_t(0X02)
#define WORKMODE_RX_LENGTH1 uint8_t(0X00)
#define WORKMODE_TX_LENGTH2 uint8_t(0X01)
#define WORKMODE_RX_LENGTH2 uint8_t(0X01)

#define CMD_RESET uint8_t(0X07)
#define RESET_TX_LENGTH uint8_t(0X00)
#define RESET_RX_LENGTH uint8_t(0X00)

#endif
