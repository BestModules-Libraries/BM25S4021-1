/*****************************************************************
File:					BM25S4021-1.cpp
Author:				BEST MODULES CORP.
Description:	Define classes and required variables if this class
							has parameters,input parameters when this class defined
							the class(this function) can call its mumbers in current
							function,or by called outside else
History:			V1.0.2   -- 2025-06-12
******************************************************************/
#include "BM25S4021-1.h"

unsigned char checkSum, uint8_tSWRxbuff[12] = {0};
/**********************************************************
Description: Constructor
Parameters:	 intPin：Alarm signal receiving pin
						 *theSerial: Wire object if your board has more than one UART interface
						 parameter range:&Serial(Arduino & BMduino), &Serial1、&Serial2、&Serial3、&Serial4(BMduino)
Return:      None
Others:      None
**********************************************************/
BM25S4021_1::BM25S4021_1(uint8_t intPin, HardwareSerial *theSerial)
{
	_hardSerial = theSerial;
	_intPin = intPin;
	_softSerial = NULL;
	checkSum = 0;
	pinMode(_intPin, INPUT);
}
/**********************************************************
Description: Constructor
Parameters:  intPin：Alarm signal receiving pin
						 rxPin: Receiver pin of the UART
						 txPin: Send signal pin of UART
Return:      None
Others:   	 None
**********************************************************/
BM25S4021_1::BM25S4021_1(uint8_t intPin, uint8_t rxPin, uint8_t txPin)
{
	_hardSerial = NULL;
	_intPin = intPin;
	_rxPin = rxPin;
	_txPin = txPin;
	_softSerial = new SoftwareSerial(_rxPin, _txPin);
	checkSum = 0;
	pinMode(_intPin, INPUT);
}
/**********************************************************
Description: Set the baudrate of the serial communicates with module
Parameters:	 None
Return:      void
Others:      baudRate: 9600bps
**********************************************************/
void BM25S4021_1::begin()
{
	checkSum = 0;
	if (_softSerial != NULL)
	{
		_softSerial->begin(9600);
		_softSerial->setTimeout(25); // set softwareUart boudrate
	}
	else
	{
		_hardSerial->begin(9600);
		_hardSerial->setTimeout(25); // set hardwareUart timeout
	}
}
/**********************************************************
Description: Get the status of the int pin
Parameters:  None
Return:      HIGH(1): Alarm
						 LOW(0):  No alarm
Others:      None
**********************************************************/
uint8_t BM25S4021_1::getINT()
{
	return digitalRead(_intPin);
}

/**********************************************************
Description:	Get the current single channel TDS and the temperature
Parameters:		channel: Channels of TDS/NTC to be get
Return:     	TDSValue: Water quality concentration
Others:     	None
**********************************************************/
void BM25S4021_1::selectModule(uint8_t ID)
{
	_selectModule = ID;
}

/**********************************************************
Description:	Get the current single channel TDS and the temperature
Parameters:		channel: Channels of TDS/NTC to be get
Return:     	TDSValue: Water quality concentration
Others:     	None
**********************************************************/
float BM25S4021_1::readTDS(uint8_t channel)
{
	return readTDS(_selectModule, channel);
}
/**********************************************************
Description:Get the current single channel TDS and the temperature
Parameters: channel:Variable for storing Channels of TDS/NTC to be get
Return:     NTCValue:Water temperature
Others:     None
**********************************************************/
float BM25S4021_1::readTemperature(uint8_t channel)
{
	return readTemperature(_selectModule, channel);
}
/**********************************************************
Description:	Get the current single channel TDS
Parameters:		ID: Module identification code
							channel: Channels of TDS/NTC to be get
Return:     	TDSValue: Water quality concentration
Others:     	None
**********************************************************/
float BM25S4021_1::readTDS(uint8_t ID, uint8_t channel)
{
	float TDSValue = 0; /*TDS value*/
	float NTCValue = 0; /*NTC value*/
	readTDSAndTEMP(ID, channel, &TDSValue, &NTCValue);
	return TDSValue;
}
/**********************************************************
Description:	Get the current single channel TDS and the temperature
Parameters: 	ID: Module identification code
							channel: Channels of TDS/NTC to be get
Return:     	NTCValue:Water temperature
Others:     	None
**********************************************************/
float BM25S4021_1::readTemperature(uint8_t ID, uint8_t channel)
{
	float TDSValue = 0; /*TDS value*/
	float NTCValue = 0; /*NTC value*/
	readTDSAndTEMP(ID, channel, &TDSValue, &NTCValue);
	return NTCValue;
}
/**********************************************************
Description: 	Get alarm value
Parameters:		ID: Module identification code
Return:				AlarmValue: TDS alarm value
Others:				None
**********************************************************/
float BM25S4021_1::getAlarmValue(uint8_t ID, uint8_t channel)
{
	float uint32_tAlarmValue = 0;
	uint8_t check, i;
	if (channel > '0')
	{
		channel = channel - '0'; /*input ascii format*/
	}
	if ((channel != 1) && (channel != 2))
	{
		return CHECK_FAIL;
	}
	checkSum = 0;
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_WARN + WARN_TX_LENGTH2 + GET;
	checkSum = ~checkSum;
	checkSum++;
	uint8_t txBuff[8] = {HEAD1, HEAD2, CLASS, ID, CMD_WARN, WARN_TX_LENGTH2, GET, checkSum};
	writeBytes(txBuff, 8);
	check = getData(uint8_tSWRxbuff, CMD_WARN, ID, WARN_RX_LENGTH2);
	if (check == CHECK_OK && channel == uint8_tSWRxbuff[6])
	{
		uint32_tAlarmValue = float((uint16_t(uint8_tSWRxbuff[7]) << 8) + uint8_tSWRxbuff[8]) / 10;
	}
	for (i = 0; i < 12; i++)
	{
		uint8_tSWRxbuff[i] = 0;
	}
	return uint32_tAlarmValue;
}
/**********************************************************
Description:	Get work mode
Parameters: 	ID: Module identification code
Return:     	0: Sleep
							1: Only CH1 works
							2: Only CH2 works
							3: CH1 and CH2 work together
Others:     	None
**********************************************************/
uint8_t BM25S4021_1::getWorkMode(uint8_t ID)
{
	uint8_t check, i, result;
	uint8_tSWRxbuff[0] = 0;
	checkSum = 0;
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_WORKMODE + WORKMODE_TX_LENGTH2 + GET;
	checkSum = ~checkSum;
	checkSum++;
	uint8_t txBuff[8] = {HEAD1, HEAD2, CLASS, ID, CMD_WORKMODE, WORKMODE_TX_LENGTH2, GET, checkSum};
	writeBytes(txBuff, 8);
	result = CHECK_FAIL;
	check = getData(uint8_tSWRxbuff, CMD_WORKMODE, ID, WORKMODE_RX_LENGTH2);
	if (check == CHECK_OK)
	{
		result = uint8_tSWRxbuff[6];
	}
	for (i = 0; i < 12; i++)
	{
		uint8_tSWRxbuff[i] = 0;
	}
	return result;
}

/**********************************************************
Description:	Reset module
Parameters: 	ID: Module identification code
Return:    		0xaa:success
							0xbb:fail
Others:     	None
**********************************************************/
uint8_t BM25S4021_1::reset(uint8_t ID)
{
	uint8_t check, i, result;
	uint8_tSWRxbuff[0] = 0;
	checkSum = 0;
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_RESET + RESET_TX_LENGTH;
	checkSum = ~checkSum;
	checkSum++;
	uint8_t txBuff[7] = {HEAD1, HEAD2, CLASS, ID, CMD_RESET, RESET_TX_LENGTH, checkSum};
	writeBytes(txBuff, 7);
	result = CHECK_FAIL;
	check = getData(uint8_tSWRxbuff, CMD_RESET, ID, RESET_RX_LENGTH);
	if (check == CHECK_OK)
	{
		result = CHECK_OK;
	}
	for (i = 0; i < 12; i++)
	{
		uint8_tSWRxbuff[i] = 0;
	}
	return result;
}

/**********************************************************
Description: 	Set module ID
Parameters: 	ID: Module identification code
Return:     	0xaa:success
							0xbb:fail
Others:     	None
**********************************************************/
uint8_t BM25S4021_1::setID(uint8_t oldID, uint8_t newID)
{
	uint8_t check, i, result;
	uint8_tSWRxbuff[0] = 0;
	checkSum = 0;
	checkSum = HEAD1 + HEAD2 + CLASS + oldID + CMD_SETID + SETID_TX_LENGTH + newID;
	checkSum = ~checkSum;
	checkSum++;
	uint8_t txBuff[8] = {HEAD1, HEAD2, CLASS, oldID, CMD_SETID, SETID_TX_LENGTH, newID, checkSum};
	writeBytes(txBuff, 8);
	result = CHECK_FAIL;
	check = getData(uint8_tSWRxbuff, CMD_SETID, newID, SETID_RX_LENGTH);
	if (check == CHECK_OK && uint8_tSWRxbuff[3] == newID)
	{
		result = CHECK_OK;
	}
	for (i = 0; i < 12; i++)
	{
		uint8_tSWRxbuff[i] = 0;
	}
	return result;
}

/**********************************************************
Description:	Set alarm value
Parameters: 	ID: Module identification code
							channel: Channels of TDS/NTC to be get
							alarmValue:TDS alarm upper limit
Return:     	0xaa:success
							0xbb:fail
Others:     	None
**********************************************************/
uint8_t BM25S4021_1::setAlarmValue(uint8_t ID, uint8_t channel, float alarmValue)
{
	uint32_t uint32_tAlarmValue;
	uint8_t result, check, i;

	uint32_tAlarmValue = uint32_t(alarmValue * 10);
	if (channel > '0')
	{
		channel = channel - '0'; /*input ascii format*/
	}
	if ((channel != 1) && (channel != 2))
	{
		return CHECK_FAIL;
	}
	checkSum = 0;
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_WARN + WARN_TX_LENGTH1 + SET + channel + uint8_t(uint32_tAlarmValue >> 8) + uint8_t(uint32_tAlarmValue);
	checkSum = ~checkSum;
	checkSum++;
	uint8_t txBuff[11] = {HEAD1, HEAD2, CLASS, ID, CMD_WARN, WARN_TX_LENGTH1, SET, channel, uint8_t(uint32_tAlarmValue >> 8), uint8_t(uint32_tAlarmValue), checkSum};
	writeBytes(txBuff, 11);
	check = getData(uint8_tSWRxbuff, CMD_WARN, ID, WARN_RX_LENGTH1);
	result = CHECK_FAIL;
	if (check == CHECK_OK && uint8_tSWRxbuff[7] == 1)
	{
		result = CHECK_OK;
	}
	for (i = 0; i < 12; i++)
	{
		uint8_tSWRxbuff[i] = 0;
	}
	return result;
}

/**********************************************************
Description:	Set work mode
Parameters: 	ID: Module identification code
Return:     	0xaa:success
							0xbb:fail
Others:     	None
**********************************************************/
uint8_t BM25S4021_1::setWorkMode(uint8_t ID, uint8_t mode)
{
	uint8_t check, i, result;
	uint8_tSWRxbuff[0] = 0;
	checkSum = 0;
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_WORKMODE + WORKMODE_TX_LENGTH1 + SET + mode;
	checkSum = ~checkSum;
	checkSum++;
	uint8_t txBuff[9] = {HEAD1, HEAD2, CLASS, ID, CMD_WORKMODE, WORKMODE_TX_LENGTH1, SET, mode, checkSum};
	writeBytes(txBuff, 9);
	result = CHECK_FAIL;

	check = getData(uint8_tSWRxbuff, CMD_WORKMODE, ID, WORKMODE_RX_LENGTH1);
	if (check == CHECK_OK)
	{
		result = CHECK_OK;
	}
	for (i = 0; i < 12; i++)
	{
		uint8_tSWRxbuff[i] = 0;
	}
	return result;
}

/**********************************************************
Description:Get the current single channel TDS and the temperature
Parameters: ID: Module identification code
			channel: Channels of TDS/NTC to be get
			getTDSValue:Variable for storing current TDS value
			getNTCValue:Variable for storing current NTC value
Return:     0xaa:CHECK_OK
			0x01:NO_MODULE
			0x02:NO_TDS_MODULE
			0x03:TIMEOUT
			0x04:CHECKSUM_ERROR
			0x05:COMMAND_ERROR
			0x06:CHANNEL_ERROR
			0x07:ID_ERROR
Others:     None
**********************************************************/
uint8_t BM25S4021_1::readTDSAndTEMP(uint8_t ID, uint8_t channel, float *getTDSValue, float *getTempValue)
{
	uint8_t check, i;
	uint8_tSWRxbuff[0] = 0;
	if (channel > '0')
	{
		channel = channel - '0'; /*input ascii format*/
	}
	if ((channel != 1) && (channel != 2))
	{
		return CHANNEL_ERROR;
	}
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_GETCON + GETCON_TX_LENGTH + channel;
	checkSum = ~checkSum;
	checkSum++;
	uint8_t dataCMD[8] = {HEAD1, HEAD2, CLASS, ID, CMD_GETCON, GETCON_TX_LENGTH, channel, checkSum};
	writeBytes(dataCMD, 8);
	checkSum = 0;
	check = getData(uint8_tSWRxbuff, CMD_GETCON, ID, GETCON_RX_LENGTH);
	if (check == CHECK_OK)
	{
		*getTDSValue = float((uint16_t(uint8_tSWRxbuff[7]) << 8) + uint8_tSWRxbuff[8]) / 10;
		*getTempValue = float((uint16_t(uint8_tSWRxbuff[9]) << 8) + uint8_tSWRxbuff[10]) / 10;
	}
	else
	{
		*getTDSValue = 0;
		*getTempValue = 0;
	}
	for (i = 0; i < 12; i++)
	{
		uint8_tSWRxbuff[i] = 0;
	}
	return check;
}
/**********************************************************
Description: UART write bytes
Parameters: wBuffer:buff for storing uart send data
						wLen: Data length
Return:	void
Others:	None
**********************************************************/
void BM25S4021_1::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
	{
		/* Select SoftwareSerial Interface */
		if (_softSerial != NULL)
		{
			while (_softSerial->available() > 0)
			{
				_softSerial->read();
			}
			_softSerial->write(wbuf, wlen);
		}
		/* Select HardwareSerial Interface */
		else
		{
			while (_hardSerial->available() > 0)
			{
				_hardSerial->read();
			}
			_hardSerial->write(wbuf, wlen);
		}
	}
}
/**********************************************************
Description:UART read Data
Parameters: buff:buff for storing uart receive data
						command:Module command instructions
						ID: Module identification code
						rxLength: Data bit length
Return:	1:CHECKSUM_OK
				2:NO_MODULE
				3:NO_TDS_MODULE
				4:TIMEOUT
				5:CHECKSUM_ERROR
				6:COMMAND_ERROR
Others:	None
**********************************************************/
uint8_t BM25S4021_1::getData(uint8_t *buff, uint8_t cmmand, uint8_t ID, uint8_t rxLength)
{
	uint8_t i, length;
	length = rxLength;
	length = length + 7;
	if (_softSerial != NULL)
	{
		if (_softSerial->readBytes(buff, 1) != 0)
		{

			while (buff[0] != HEAD1)
			{

				if (_softSerial->readBytes(buff, 1) == 0)
				{
					return NO_MODULE;
				}
			}

			_softSerial->readBytes(buff, length - 1);
			for (i = length - 1; i > 0; i--)
			{

				buff[i] = buff[i - 1];
			}
			buff[0] = HEAD1;
		}
	}
	else
	{
		if (_hardSerial->readBytes(buff, 1) != 0)
		{
			while (buff[0] != HEAD1)
			{
				if (_hardSerial->readBytes(buff, 1) == 0)
				{
					return NO_MODULE;
				}
			}
			_hardSerial->readBytes(buff, length - 1);
			for (i = length - 1; i > 0; i--)
			{

				buff[i] = buff[i - 1];
			}
			buff[0] = HEAD1;
		}
	}
	if (buff[0] != HEAD1 && buff[1] != HEAD2)
		return NO_MODULE;
	if (buff[2] != CLASS)
		return NO_TDS_MODULE;
	if (buff[3] != ID)
		return ID_ERROR;
	if (buff[4] != cmmand + 0x80)
		return COMMAND_ERROR;
	checkSum = 0;
	for (i = 0; i < length; i++)
	{
		checkSum = checkSum + buff[i];
	}
	if (checkSum == 0)
	{
		return CHECK_OK;
	}
	else
	{
		return CHECKSUM_ERROR;
	}
	return TIMEOUT;
}