/*
 * SERCOMforI2C.c
 *
 * Created: 21.06.2022 09:32:40
 *  Author: ARGE
 */ 
#include "SERCOMforI2C.h"
uint8_t aa, bb;
float ffgclk = 8000000;

void i2cinit(float ffscl){
	
	//NVIC_EnableIRQ(SERCOM1_IRQn); // global interrupt has been enabled for i2c.
	//NVIC_SetPriority(SERCOM1_IRQn, 0); // with this way i2c has higher priorty than uart
	
	SERCOM1->I2CM.CTRLA.bit.ENABLE = 0; // first i2c is disabled
	SERCOM1->I2CM.CTRLA.bit.MODE = 0x5; //These bits must be written to 0x5 to select the I2C host serial communication
	SERCOM1->I2CM.CTRLB.bit.SMEN = 1; //When Smart mode is enabled, acknowledge action is sent when DATA.DATA is read. Not need 
									 // for lm75
	//SERCOM1->I2CM.CTRLA.bit.LOWTOUTEN = 1; // This bit enables the SCL low time-out. If SCL is held low for 25ms-35ms, the host will release its clock hold, if
										 //enabled, and complete the current transaction. A stop condition will automatically be transmitted.
	//SERCOM1->I2CM.CTRLA.bit.INACTOUT = 0x2; //If the inactive bus time-out is enabled and the bus is inactive for longer than the time-out setting, the bus state logic
											//will be set to idle. An inactive bus arise when either an I2C host or client is holding the SCL low.
	SERCOM1->I2CM.CTRLA.bit.SCLSM = 0; //This strategy can be used when it is not necessary to check DATA before acknowledging
	SERCOM1->I2CM.CTRLA.bit.SPEED = 0x1;// for Fast-mode Plus (Fm+) up to 1 MHz

	aa = (ffgclk / ffscl) - 10 - 0.8;
	bb = aa / 3;
	SERCOM1->I2CM.BAUD.bit.BAUDLOW = (2 * bb);
	SERCOM1->I2CM.BAUD.bit.BAUD = bb;
	SERCOM1->I2CM.CTRLA.bit.ENABLE = 1;
	SERCOM1->I2CM.STATUS.bit.BUSSTATE = 0x1; //!! dont write 0b01 instead of 0x1 
											 // we forced bus state to idle mode. 
										    //When the bus is IDLE it is ready for a new transaction
}

void i2cAddresswrite(uint8_t adres){ 
	SERCOM1->I2CM.ADDR.bit.ADDR = ((adres << 1) | 0x0); //The I2C host starts a bus transaction by writing 
														//the I2C client address to ADDR.ADDR and the direction bit											
	//  If the address is acknowledged, SCL is forced and held low, and STATUS.CLKHOLD and INTFLAG.MB are set. 
	while(!(SERCOM1->I2CM.STATUS.bit.CLKHOLD && SERCOM1->I2CM.INTFLAG.bit.MB)){}
	while(SERCOM1->I2CM.STATUS.bit.RXNACK){}
	while(SERCOM1->I2CM.SYNCBUSY.bit.SYSOP){}
}

void i2cpointerwrite(uint8_t i2cpointdeger){
	
	SERCOM1->I2CM.DATA.bit.DATA = i2cpointdeger;
	while(SERCOM1->I2CM.SYNCBUSY.bit.SYSOP){}// wait until the sync
	while (!(SERCOM_I2CM_INTFLAG_MB && (SERCOM1->I2CM.STATUS.bit.ARBLOST != 0x1))){} // if mb is set and arblost is not 1 
	//. When this flag is set and arbitration is not lost, the host forces the SCL line low, stretching the I2C clock period
	while(SERCOM1->I2CM.STATUS.bit.RXNACK){}// it is waiting until ack bit is received from slave
}

void i2cRepeatedstartaddress(uint8_t adres){
	
	SERCOM1->I2CM.ADDR.bit.ADDR = ((adres << 1) | 0x1); //The I2C host starts a bus transaction by writing
	//the I2C client address to ADDR.ADDR and the direction bit
	while(SERCOM1->I2CM.SYNCBUSY.bit.SYSOP){} //Writing CTRLB.CMD, STATUS.BUSSTATE, ADDR, or DATA when the SERCOM is enabled requires synchronization.
											  //When written, the SYNCBUSY.SYSOP bit will be set until synchronization is complete.										  
	while(SERCOM1->I2CM.STATUS.bit.RXNACK){}// it is waiting until ack bit is received  from slave
	while(0 == (SERCOM1->I2CM.INTFLAG.reg &  SERCOM_I2CM_INTFLAG_SB)); // bunu koymadýðýn diye çalýþmadý
}

void i2csendAck(){
	
	SERCOM1->I2CM.CTRLB.bit.ACKACT = 0; // send ack
}

void i2csendNack(){
	
	SERCOM1->I2CM.CTRLB.bit.ACKACT = 1; // send nack
}

uint8_t i2cread(){
	uint8_t denede = 0x00;
	while(SERCOM1->I2CM.SYNCBUSY.bit.SYSOP == 1);
	//*readingdata = SERCOM1->I2CM.DATA.bit.DATA; // After reading this bit, smen will send ack or nack
	denede = SERCOM1->I2CM.DATA.bit.DATA; 
	while(SERCOM1->I2CM.SYNCBUSY.bit.SYSOP){}
	while (0 == (SERCOM1->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_SB));
	return denede;
}

void i2cAddressread(uint8_t adres){
	
	SERCOM1->I2CM.ADDR.bit.ADDR = ((adres << 1) | 0x1); //The I2C host starts a bus transaction by writing
	//the I2C client address to ADDR.ADDR and the direction bit
	while(SERCOM1->I2CM.STATUS.bit.RXNACK){}
}

void i2cwrite(uint8_t writingdata){
	
	SERCOM1->I2CM.DATA.bit.DATA = writingdata;
	while (!(SERCOM_I2CM_INTFLAG_MB && (SERCOM1->I2CM.STATUS.bit.ARBLOST != 0x1))){} // if mb is set and arblost is not 1
	//. When this flag is set and arbitration is not lost, the host forces the SCL line low, stretching the I2C clock period
	while(SERCOM1->I2CM.STATUS.bit.RXNACK){}// it is waiting until ack bit is resived  from slave
}

void i2cstop(){
	
	SERCOM1->I2CM.CTRLB.bit.CMD = 0x3; //Execute acknowledge action succeeded by issuing a Stop condition
	while(SERCOM1->I2CM.SYNCBUSY.bit.SYSOP){}
}