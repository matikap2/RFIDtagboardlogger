#include "rc522.h"

/* Write/Read help functions */
void RC522_writeReg(uint8_t adr, uint8_t val){
	adr = (adr << 1) & 0x7E;												//Frame format (datasheet 8.1.2)							
	SPI1_writeReg(adr, val);
}


uint8_t RC522_readReg(uint8_t adr){
	uint8_t temp;
	
	adr = ((adr << 1) & 0x7E) | 0x80;								//Frame format (datasheet 8.1.2)
	temp = SPI1_readReg(adr);
	
	return temp;
}


void RC522_setMask(uint8_t reg, uint8_t mask){
	RC522_writeReg(reg, RC522_readReg(reg) | mask);
}


void RC522_clearMask(uint8_t reg, uint8_t mask){
	RC522_writeReg(reg, RC522_readReg(reg) & ~(mask) );
}



/* Initialization function */
void RC522_Init(void){
	RC522_Reset();
	//prescalers, antenna gain, antenna on
	RC522_writeReg(TModeReg, 0x8D);					//Configuring timer inside RC522 - prescalers
	RC522_writeReg(TPrescalerReg, 0x3E);
	RC522_writeReg(TReloadRegL, 30);
	RC522_writeReg(TReloadRegH, 0);
	
	RC522_writeReg(RFCfgReg, 0x70);					//Antenna gain 48dB
	RC522_writeReg(TxAutoReg, 0x40);				//Tx modulation setting
	RC522_writeReg(ModeReg, 0x3D);					//Defining mode for tx and rx
	
	RC522_antennaOn();											//Antenna on
}

void RC522_antennaOn(void){
	uint8_t temp;
	
	temp = RC522_readReg(TxControlReg);			//Turning on antenna
	if (!(temp & 0x03))
		RC522_setMask(TxControlReg, 0x03);
}


void RC522_antennaOff(void){
	RC522_clearMask(TxControlReg, 0x03);		//Turning off antenna
}


void RC522_Reset(void){
	RC522_writeReg(CommandReg, PCD_RESETPHASE);		//Soft reset
}


void RC522_firmwareVer(void){
	char buffer[50];
	uint8_t temp;
	
	UART2_sendString("Wersja RC522\r\n");
	temp = RC522_readReg(VersionReg);						//Read version of chip -> 0x12 probably counterfit chip (info google)
	sprintf(buffer, "%x", temp);
	UART2_sendString("0x");
	UART2_sendString(buffer);
	UART2_sendString("\r\n");
}



/* Main functions */
uint8_t RC522_commandTag(uint8_t* sendData, uint8_t sendLen, uint8_t* backData, uint16_t* backLen){		//communicating with Mifare card
	uint8_t status = ERR;
	uint8_t irqEn = 0x77;				//transceive command
	uint8_t waitIRq = 0x30;			//transceive command
	uint8_t lastBits;
	uint8_t n;
	uint16_t i;

	RC522_writeReg(CommIEnReg, irqEn | 0x80);		//interrupt request
	RC522_clearMask(CommIrqReg, 0x80);					//clearing all interrupts request bits
	RC522_setMask(FIFOLevelReg, 0x80);					//Init FIFO
	RC522_writeReg(CommandReg, PCD_IDLE);				//canceling current command

	
	for (i = 0; i < sendLen; i++){											// Writing data to the FIFO
		RC522_writeReg(FIFODataReg, sendData[i]);
	}
	
	RC522_writeReg(CommandReg, PCD_TRANSCEIVE);					// Execute the command		 
		RC522_setMask(BitFramingReg, 0x80);								// Start transmission of data
																											// Waiting for the command to complete so we can recieve
	i = 2000;	// M1 card maximum waiting time 25ms
	while ((i!=0) && !(n&0x01) && !(n&waitIRq)){																	
		n = RC522_readReg(CommIrqReg);											// CommIrqReg[7..0] 
		i--;																								// Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
	}

	RC522_clearMask(BitFramingReg, 0x80);								//End transmission of data

	if (i != 0)  {																			//Request didnt timeout
		if (!(RC522_readReg(ErrorReg) & 0x1B)) {
			status = OK;
			if (n & irqEn & 0x01) status = NOTAGERR;
				n = RC522_readReg(FIFOLevelReg);
				lastBits = RC522_readReg(ControlReg) & 0x07;
				if (lastBits) 
					*backLen = (n - 1) * 8 + lastBits; 
				else 
					*backLen = n * 8;
				if (n == 0)
					n = 1;
				if (n > 16) //max length
					n = 16;
				for (i = 0; i < n; i++){ 											// Reading the received data in FIFO
				backData[i] = RC522_readReg(FIFODataReg);
				}		
		} else status = ERR;
	}
	return status;
}


uint8_t RC522_request(uint8_t reqMode, uint8_t* TagType){				//Checking if there is tag nearby
	uint8_t status;  
	uint16_t backBits;																						//The received data bits
	
	RC522_writeReg(BitFramingReg, 0x07);													//TxLastBists = BitFramingReg[2..0]
	TagType[0] = reqMode;
	status = RC522_commandTag(TagType, 1, TagType, &backBits);		//Communicating with tag
	if ((status != OK) || (backBits != 0x10)) 
		status = ERR;
	return status;
}


uint8_t RC522_antiCollision(uint8_t* serNum){								//Handle collisions if there is multiple tags nearby
	uint8_t status;
	uint8_t i;
	uint8_t serNumCheck = 0;
	uint16_t unLen;

	RC522_writeReg(BitFramingReg, 0x00);											//TxLastBists = BitFramingReg[2..0]
	serNum[0] = PICC_ANTICOLL;
	serNum[1] = 0x20;
	status = RC522_commandTag(serNum, 2, serNum, &unLen);
	if (status == OK) {
		for (i = 0; i < 4; i++){																//Checking checksum of tag (the checksum is ^ if all values)
			serNumCheck ^= serNum[i];
		}
		if (serNumCheck != serNum[i]) 													//Checksum should be the same
			status = ERR;
	}
	return status;
}


void RC522_calcCRC(uint8_t* pIndata, uint8_t len, uint8_t* pOutData){
	uint8_t i, n;

	RC522_clearMask(DivIrqReg, 0x04);											//CRCIrq = 0
	RC522_setMask(FIFOLevelReg, 0x80);  								  //Clear the FIFO pointer

	// Writing data to the FIFO	
	for (i = 0; i < len; i++){
		RC522_writeReg(FIFODataReg, *(pIndata+i));
	}
	RC522_writeReg(CommandReg, PCD_CALCCRC);							//Dending calculation crc command

	i = 0xFF;
	while ((i!=0) && !(n&0x04)){													// Wait CRC calculation is complete
		n = RC522_readReg(DivIrqReg);
		i--;
	}																											// CRCIrq = 1

	pOutData[0] = RC522_readReg(CRCResultRegL);						// Read CRC calculation result
	pOutData[1] = RC522_readReg(CRCResultRegM);
}


void RC522_Halt(void){																	//Sends halt command for current tag
	uint16_t unLen;
	uint8_t buff[4]; 

	buff[0] = PICC_HALT;
	buff[1] = 0;
	RC522_calcCRC(buff, 2, &buff[2]);	
	RC522_commandTag(buff, 4, buff, &unLen);							//Send Halt cmd
}


uint8_t RC522_checkID(uint8_t* id){
	uint8_t status;
	
	status = RC522_request(PICC_REQIDL, id);						//Find cards, return card type	
	if (status == OK) {																	//Card detected
		status = RC522_antiCollision(id);									//Anti-collision, return card serial number 4 bytes
	}
	RC522_Halt();																				//Command card into hibernation 

	return status;
}


void RC522_dumpUIDtoSerial(uint8_t* UID){
		char buffer[50];
	UART2_sendString("UID:\r\n");
	UART2_sendString("0x");
	//UART2_sendString(buffer);
	
	for(int i = 0; i <4; i++){
	sprintf(buffer, "%x", UID[i]);
	UART2_sendString(buffer);
	UART2_sendString(" ");	
	}
	
	UART2_sendString("\r\n");
}
	
void RC522_dumpUIDtoNodeMCU(uint8_t* UID){
		char buffer[256];
	sprintf(buffer, "%x%x%x%x", UID[0],UID[1],UID[2],UID[3]);
	UART1_sendString(buffer);
}

