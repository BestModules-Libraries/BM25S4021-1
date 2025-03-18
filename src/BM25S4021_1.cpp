 /*****************************************************************
File:             BM25S4021_1->cpp
Author:           BESTMODULE
Description:      Define classes and required variables if this class 
				  has parameters,input parameters when this class defined 
				  the class(this function) can call its mumbers in current
				  function,or by called outside else
History：         
V1.0.1-- initial version；2023-3-3；Arduino IDE : ≥v1.8.13
******************************************************************/
#include  "BM25S4021_1.h"
#include <SoftwareSerial.h>
unsigned char checkSum,uint8_tSWRxbuff[12]={0};
/**********************************************************
Description: Constructor
Parameters: *theSerial: Wire object if your board has more than one UART interface      
                         parameter range:
                                         BMduino UNO: &Serial、&Serial1、&Serial2、&Serial3、&Serial4
                                         Arduino UNO：&Serial
Return:      None    
Others:      None
**********************************************************/
BM25S4021_1::BM25S4021_1(HardwareSerial*theSerial)
{
  _Serial = theSerial;
  softSerial = NULL;
  checksum=0;
}
/**********************************************************
Description: Constructor
Parameters:  rxPin: Receiver pin of the UART
             txPin: Send signal pin of UART         
Return:      None    
Others:   	 None
**********************************************************/
BM25S4021_1::BM25S4021_1(uint8_t rxPin,uint8_t txPin)
{
  _Serial = NULL;
  _rxPin = rxPin;
  _txPin = txPin;
  softSerial = new SoftwareSerial(_rxPin,_txPin);
  checksum=0;
}
/**********************************************************
Description: Set the baudrate of the serial communicates 
			 with module

Parameters:  baudRate:(default 9600bps)  
Output:      None
Return:      None    
Others:      None
**********************************************************/
void BM25S4021_1::begin(uint32_t baudRate)
{
  checksum=0;	
	if(softSerial != NULL)
	{	
		softSerial->begin(baudRate);
		softSerial->setTimeout(25);				//set softwareUart boudrate	
	}
	else
	{
		_Serial->begin(baudRate);		
		_Serial->setTimeout(25);					//set hardwareUart timeout		
	}		
}

/**********************************************************
Description:Get the current single channel TDS and the temperature
Parameters: ID:Modular identification code
			channel: Channels of TDS/NTC to be get  
Return:     TDSValue:Water quality concentration       
Others:     None 
**********************************************************/
float  BM25S4021_1::readTDS(uint8_t ID,uint8_t channel)
{
  float TDSValue = 0;/*TDS value*/
  float NTCValue = 0;/*NTC value*/
  readData(ID,channel,&TDSValue,&NTCValue);  
  return TDSValue; 
}
/**********************************************************
Description:Get the current single channel TDS and the temperature
Parameters: ID:Modular identification code
			channel: Channels of TDS/NTC to be get              
Return:     NTCValue:Water temperature      
Others:     None 
**********************************************************/
float  BM25S4021_1::readTemp(uint8_t ID,uint8_t channel)
{
  float TDSValue = 0;/*TDS value*/
  float NTCValue = 0;/*NTC value*/
  readData(ID,channel,&TDSValue,&NTCValue);  
  return NTCValue; 
}
/**********************************************************
Description:Get the current single channel TDS and the temperature
Parameters: ID:Modular identification code
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

uint8_t  BM25S4021_1::readData(uint8_t ID,uint8_t channel,float *getTDSValue,float *getTempValue)
{
	uint8_t check,i;
	uint8_tSWRxbuff[0] = 0;
	if(channel>'0')
	{
		channel = channel - '0';/*input ascii format*/				
	}
	if((channel != 1)&&(channel != 2))
	{
		return CHANNEL_ERROR;
	}
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_GETCON + GETCON_TX_LENGTH + channel;
// Serial.print(ID);
	checkSum =~checkSum;
	checkSum++;	
	if(softSerial != NULL)
  {
		softSerial->write(HEAD1); 
		softSerial->write(HEAD2); 
		softSerial->write(CLASS); 
		softSerial->write(ID); 
		softSerial->write(CMD_GETCON);
		softSerial->write(GETCON_TX_LENGTH);
		softSerial->write(channel);
		softSerial->write(checkSum);		
	}
	else
	{
		_Serial->write(HEAD1); 
		_Serial->write(HEAD2); 
		_Serial->write(CLASS); 
		_Serial->write(ID); 
		_Serial->write(CMD_GETCON);
		_Serial->write(GETCON_TX_LENGTH);
		_Serial->write(channel);
		_Serial->write(checkSum);			
	}

	checkSum = 0;	
	check = getData(uint8_tSWRxbuff,CMD_GETCON,ID,GETCON_RX_LENGTH);
	if(check == CHECK_OK)
	{
		*getTDSValue = float((uint16_t(uint8_tSWRxbuff[7])<<8) + uint8_tSWRxbuff[8])/10; 
		*getTempValue = float((uint16_t(uint8_tSWRxbuff[9])<<8) + uint8_tSWRxbuff[10])/10;
	}
	else
	{
		*getTDSValue = 0;
		*getTempValue = 0;
	}
	for(i = 0;i<12;i++)
	{
		uint8_tSWRxbuff[i] = 0;
	}	
	return check;	
}
/**********************************************************
Description:
Parameters: ID:Modular identification code
			channel: Channels of TDS/NTC to be get 			      
			warnValue:TDS alarm upper limit 
Return:     0xaa:success 
			0xbb:fail   
Others:     None 
**********************************************************/
uint8_t  BM25S4021_1::setWarn(uint8_t ID,uint8_t channel,float warnValue)
{
	uint32_t uint32_tWarnValue;
	uint8_t result,check,i;
	
	uint32_tWarnValue = uint32_t(warnValue*10);
	if(channel>'0')
	{
		channel = channel - '0';/*input ascii format*/        
	}
	if((channel != 1)&&(channel != 2))
	{
		return CHECK_FAIL;
	}
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_WARN + WARN_TX_LENGTH1 + SET + channel + uint8_t(uint32_tWarnValue>>8) + uint8_t(uint32_tWarnValue);
	checkSum =~checkSum;
	checkSum++;	
	if(softSerial != NULL)
	{
		softSerial->write(HEAD1); 
		softSerial->write(HEAD2); 
		softSerial->write(CLASS); 
		softSerial->write(ID); 
		softSerial->write(CMD_WARN);
		softSerial->write(WARN_TX_LENGTH1);
		softSerial->write(SET);
		softSerial->write(channel);
		softSerial->write(uint8_t(uint32_tWarnValue>>8));
		softSerial->write(uint8_t(uint32_tWarnValue));
		softSerial->write(checkSum);    
	}
	else
	{
		_Serial->write(HEAD1); 
		_Serial->write(HEAD2); 
		_Serial->write(CLASS); 
		_Serial->write(ID); 
		_Serial->write(CMD_WARN);
		_Serial->write(WARN_TX_LENGTH1);
		_Serial->write(SET);
		_Serial->write(channel);
		_Serial->write(uint8_t(uint32_tWarnValue>>8));
		_Serial->write(uint8_t(uint32_tWarnValue));
		_Serial->write(checkSum);     
	} 
	check = getData(uint8_tSWRxbuff,CMD_WARN,ID,WARN_RX_LENGTH1);
	result = CHECK_FAIL;
	if(check == CHECK_OK&&uint8_tSWRxbuff[7] == 1)
	{
		result = CHECK_OK;
	}
	for(i = 0;i<12;i++)
	{
		uint8_tSWRxbuff[i] = 0;
	} 
	return result;   
}
/**********************************************************
Description:
Parameters: 	
Return:     
Others:     
**********************************************************/
float  BM25S4021_1::getWarn(uint8_t ID,uint8_t channel)
{
	float uint32_tWarnValue = 0;
	uint8_t check,i;
	if(channel>'0')
	{
		channel = channel - '0';/*input ascii format*/        
	}
	if((channel != 1)&&(channel != 2))
	{
		return CHECK_FAIL;
	}
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_WARN + WARN_TX_LENGTH2 + GET;
	checkSum =~checkSum;
	checkSum++;	
	if(softSerial != NULL)
	{
		softSerial->write(HEAD1); 
		softSerial->write(HEAD2); 
		softSerial->write(CLASS); 
		softSerial->write(ID); 
		softSerial->write(CMD_WARN);
		softSerial->write(WARN_TX_LENGTH2);
		softSerial->write(GET);
		softSerial->write(checkSum);    
	}
	else
	{
		_Serial->write(HEAD1); 
		_Serial->write(HEAD2); 
		_Serial->write(CLASS); 
		_Serial->write(ID); 
		_Serial->write(CMD_WARN);
		_Serial->write(WARN_TX_LENGTH2);
		_Serial->write(GET);
		_Serial->write(checkSum);     
	} 
	check = getData(uint8_tSWRxbuff,CMD_WARN,ID,WARN_RX_LENGTH2);
	if(check == CHECK_OK&&channel == uint8_tSWRxbuff[6])
	{
		uint32_tWarnValue = float((uint16_t(uint8_tSWRxbuff[7])<<8) + uint8_tSWRxbuff[8])/10; 
	}
	for(i = 0;i<12;i++)
	{
		uint8_tSWRxbuff[i] = 0;
	} 
	return uint32_tWarnValue;   
}
/**********************************************************
Description:
Parameters: ID:Modular identification code   
Return:     0xaa:success 
			0xbb:fail 
Others:     None 
**********************************************************/
uint8_t  BM25S4021_1::setID(uint8_t oldID,uint8_t newID)
{
	uint8_t check,i,result;
	uint8_tSWRxbuff[0] = 0;
	checkSum = HEAD1 + HEAD2 + CLASS + oldID + CMD_SETID + SETID_TX_LENGTH + newID;
	checkSum =~checkSum;
	checkSum++;	
	if(softSerial != NULL)
	{
		softSerial->write(HEAD1); 
		softSerial->write(HEAD2); 
		softSerial->write(CLASS);
		softSerial->write(oldID);
		softSerial->write(CMD_SETID);
		softSerial->write(SETID_TX_LENGTH);
		softSerial->write(newID);
		softSerial->write(checkSum);    
	}
	else
	{
		_Serial->write(HEAD1); 
		_Serial->write(HEAD2); 
		_Serial->write(CLASS); 
		_Serial->write(oldID); 
		_Serial->write(CMD_SETID);
		_Serial->write(SETID_TX_LENGTH);
		_Serial->write(newID);
		_Serial->write(checkSum);     
	}

	checkSum = 0; 
	result = CHECK_FAIL;
	check = getData(uint8_tSWRxbuff,CMD_SETID,newID,SETID_RX_LENGTH);
	if(check == CHECK_OK&&uint8_tSWRxbuff[3] == newID)
	{
		result = CHECK_OK;
	}
	for(i = 0;i<12;i++)
	{
		uint8_tSWRxbuff[i] = 0;
	} 
	return result; 
}
/**********************************************************
Description:
Parameters: ID:Modular identification code   
Return:     0xaa:success 
			0xbb:fail 
Others:     None 
**********************************************************/
uint8_t  BM25S4021_1::setWorkMode(uint8_t ID,uint8_t mode)
{
	uint8_t check,i,result;
	uint8_tSWRxbuff[0] = 0;
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_WORKMODE + WORKMODE_TX_LENGTH1 + SET + mode;
	checkSum =~checkSum;
	checkSum++;	
	if(softSerial != NULL)
	{
		softSerial->write(HEAD1); 
		softSerial->write(HEAD2); 
		softSerial->write(CLASS); 
		softSerial->write(ID); 
		softSerial->write(CMD_WORKMODE);
		softSerial->write(WORKMODE_TX_LENGTH1);
		softSerial->write(SET);
		softSerial->write(mode);
		softSerial->write(checkSum);    
	}
	else
	{
		_Serial->write(HEAD1); 
		_Serial->write(HEAD2); 
		_Serial->write(CLASS); 
		_Serial->write(ID); 
		_Serial->write(CMD_WORKMODE);	
		_Serial->write(WORKMODE_TX_LENGTH1);
		_Serial->write(SET);
		_Serial->write(mode);
		_Serial->write(checkSum);     
	}
	checkSum = 0; 
	result = CHECK_FAIL;

	check = getData(uint8_tSWRxbuff,CMD_WORKMODE,ID,WORKMODE_RX_LENGTH1);
	if(check == CHECK_OK)
	{
		result = CHECK_OK;
	}
	for(i = 0;i<12;i++)
	{
		uint8_tSWRxbuff[i] = 0;
	} 
	return result; 
}
/**********************************************************
Description:
Parameters: ID:Modular identification code   
Return:     0xaa:success 
			0xbb:fail 
Others:     None 
**********************************************************/
uint8_t  BM25S4021_1::getWorkMode(uint8_t ID)
{
	uint8_t check,i,result;
	uint8_tSWRxbuff[0] = 0;
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_WORKMODE + WORKMODE_TX_LENGTH2 +  GET;
	checkSum =~checkSum;
	checkSum++;	
	if(softSerial != NULL)
	{
		softSerial->write(HEAD1); 
		softSerial->write(HEAD2); 
		softSerial->write(CLASS); 
		softSerial->write(ID); 
		softSerial->write(CMD_WORKMODE);
		softSerial->write(WORKMODE_TX_LENGTH2);
		softSerial->write(GET);
		softSerial->write(checkSum);    
	}
	else
	{
		_Serial->write(HEAD1); 
		_Serial->write(HEAD2); 
		_Serial->write(CLASS); 
		_Serial->write(ID); 
		_Serial->write(CMD_WORKMODE);	
		_Serial->write(WORKMODE_TX_LENGTH2);
		_Serial->write(GET);
		_Serial->write(checkSum);     
	}
	checkSum = 0; 
	result = CHECK_FAIL;
	check = getData(uint8_tSWRxbuff,CMD_WORKMODE,ID,WORKMODE_RX_LENGTH2);
	if(check == CHECK_OK)
	{
		result = uint8_tSWRxbuff[6];
	}
	for(i = 0;i<12;i++)
	{
		uint8_tSWRxbuff[i] = 0;
	} 
	return result; 
}
/**********************************************************
Description:
Parameters: ID:Modular identification code   
Return:     0xaa:success 
			0xbb:fail 
Others:     None 
**********************************************************/
uint8_t  BM25S4021_1::reset(uint8_t ID)
{
	uint8_t check,i,result;
	uint8_tSWRxbuff[0] = 0;
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_RESET + RESET_TX_LENGTH;
	checkSum =~checkSum;
	checkSum++;	
	if(softSerial != NULL)
	{
		softSerial->write(HEAD1); 
		softSerial->write(HEAD2); 
		softSerial->write(CLASS); 
		softSerial->write(ID); 
		softSerial->write(CMD_RESET);
		softSerial->write(RESET_TX_LENGTH);
		softSerial->write(checkSum);    
	}
	else
	{
		_Serial->write(HEAD1); 
		_Serial->write(HEAD2); 
		_Serial->write(CLASS); 
		_Serial->write(ID); 
		_Serial->write(CMD_RESET);
		_Serial->write(RESET_TX_LENGTH);
		_Serial->write(checkSum);     
	}
	checkSum = 0; 
	result = CHECK_FAIL;	
	check = getData(uint8_tSWRxbuff,CMD_RESET,ID,RESET_RX_LENGTH);
	if(check == CHECK_OK)
	{
		result = CHECK_OK;
	}
	for(i = 0;i<12;i++)
	{
		uint8_tSWRxbuff[i] = 0;
	} 
	return result; 
}
/**********************************************************
Description:
Parameters: ID:Modular identification code   
Return:     0xaa:success 
			0xbb:fail 
Others:     None 
**********************************************************/
/* uint8_t  BM25S4021_1::calRecover(uint8_t ID)
{
	uint8_t check,i,result;
	uint8_tSWRxbuff[0] = 0;
	checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_CALRECOVER + CALRECOVER_TX_LENGTH;
	checkSum =~checkSum;
	checkSum++;	
	if(softSerial != NULL)
	{
		softSerial->write(HEAD1); 
		softSerial->write(HEAD2); 
		softSerial->write(CLASS); 
		softSerial->write(ID); 
		softSerial->write(CMD_CALRECOVER);
		softSerial->write(CALRECOVER_TX_LENGTH);
		softSerial->write(checkSum); 
	}
	else
	{  
		_Serial->write(HEAD1); 
		_Serial->write(HEAD2); 
		_Serial->write(CLASS); 
		_Serial->write(ID); 
		_Serial->write(CMD_CALRECOVER);
		_Serial->write(CALRECOVER_TX_LENGTH);
		_Serial->write(checkSum); 
	}
	checkSum = 0; 
	result = CHECK_FAIL;		
	check = getData(uint8_tSWRxbuff,CMD_CALRECOVER,ID);
	if(check == CHECK_OK)
	{
		result = CHECK_OK;
	}	
	for(i = 0;i<12;i++)
	{
		uint8_tSWRxbuff[i] = 0;
	} 
	return result; 
} */
/**********************************************************
Description:
Parameters: ID:Modular identification code   
Return:     0xaa:success 
			0xbb:fail 
Others:     None 
**********************************************************/
// uint32_t  BM25S4021_1::readInfo(uint8_t ID)
// {
	// uint8_t check,i;
	// uint32_t result;
	// uint8_tSWRxbuff[0] = 0;
	// checkSum = HEAD1 + HEAD2 + CLASS + ID + CMD_READINFO + READINFO_TX_LENGTH;
	// checkSum =~checkSum;
	// checkSum++;
	// if(softSerial != NULL)
	// {
		// softSerial->write(HEAD1); 
		// softSerial->write(HEAD2); 
		// softSerial->write(CLASS); 
		// softSerial->write(ID); 
		// softSerial->write(CMD_READINFO);
		// softSerial->write(READINFO_TX_LENGTH);
		// softSerial->write(checkSum); 
	// }
	// else
	// {	  
		// _Serial->write(HEAD1); 
		// _Serial->write(HEAD2); 
		// _Serial->write(CLASS); 
		// _Serial->write(ID); 
		// _Serial->write(CMD_READINFO);
		// _Serial->write(READINFO_TX_LENGTH);
		// _Serial->write(checkSum); 
	// }
	// checkSum = 0; 
	// result = CHECK_FAIL;		
	// check = getData(uint8_tSWRxbuff,CMD_READINFO,ID);
	// if(check == CHECK_OK)
	// {
		// result = (uint8_tSWRxbuff[6]<<24) + (uint8_tSWRxbuff[7]<<16) + (uint8_tSWRxbuff[8]<<8) + uint8_tSWRxbuff[9] ;
	// }	
	// for(i = 0;i<12;i++)
	// {
		// uint8_tSWRxbuff[i] = 0;
	// } 
	// return result; 
// }
/**********************************************************
Description:UART read Data 
Parameters: buff:buff for storing uart receive data    			 
Return:     1:CHECKSUM_OK 
			2:NO_MODULE 
			3:NO_TDS_MODULE 
			4:TIMEOUT 
			5:CHECKSUM_ERROR  
			6:COMMAND_ERROR	        
Others:   	None   
**********************************************************/
uint8_t  BM25S4021_1::getData(uint8_t *buff,uint8_t cmmand,uint8_t ID,uint8_t rxLength)
{
	uint8_t i,length;
	length = rxLength;
	length = length + 7;  	
	if(softSerial != NULL)
	{		
		if(softSerial->readBytes(buff, 1) != 0)
		{
			
			while(buff[0] != HEAD1)	
			{		
				
				if(softSerial->readBytes(buff, 1) == 0)
				{
					return NO_MODULE;
				}
			}
			
			softSerial->readBytes(buff, length - 1);
			for(i = length - 1;i>0;i--)
			{

				buff[i] = buff[i-1];
			}
			buff[0] = HEAD1;
			
		}
	}
	else
	{
		if(_Serial->readBytes(buff, 1) != 0)
		{
			while(buff[0] != HEAD1)	
			{		
				if(_Serial->readBytes(buff, 1) == 0)
				{
					return NO_MODULE;
				}
			}
			_Serial->readBytes(buff, length - 1);
			for(i = length - 1;i>0;i--)
			{

				buff[i] = buff[i-1];
			}
			buff[0] = HEAD1;
		}		
	}		
	if(buff[0]!=HEAD1&&buff[1]!=HEAD2)
		return NO_MODULE;
	if(buff[2]!=CLASS)
		return NO_TDS_MODULE; 
	if(buff[3]!=ID)
		return ID_ERROR;         
	if(buff[4]!=cmmand+0x80)
		return COMMAND_ERROR;	
	checkSum = 0; 
 //Serial.print("RX DATA:"); 
 // for(i = 0; i<length;i++)
 // {
   // Serial.print(uint8_tSWRxbuff[i],HEX); 
   // Serial.print(" "); 
 // }
 // Serial.println(" ");  
//  Serial.print("checkSum:"); 
	for(i = 0;i<length;i++)
	{				
		checkSum = checkSum + buff[i];
//    Serial.print(checkSum);
//    Serial.print(" "); 
	}
//  Serial.println(" ");
	if(checkSum == 0)
	{	
		return CHECK_OK;
	}	
	else
	{
		return CHECKSUM_ERROR;
	}				
	return TIMEOUT;
}
