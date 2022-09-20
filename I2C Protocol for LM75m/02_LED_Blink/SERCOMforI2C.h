/*
 * SERCOMforI2C.h
 *
 * Created: 21.06.2022 09:30:44
 *  Author: ARGE
 */ 


#ifndef SERCOMFORI2C_H_
#define SERCOMFORI2C_H_

///////////////////////// Needed Library
#include "sam.h"
#include "delay.h"
/////////////////////////

void i2cinit(float ffscl);
void i2cAddresswrite(uint8_t adres);
void i2cAddressread(uint8_t adres);
void i2cpointerwrite(uint8_t i2cpointdeger);
void i2cwrite(uint8_t writingdata);
uint8_t i2cread();
void i2cRepeatedstartaddress(uint8_t adres);
void i2csendAck();
void i2csendNack();
void i2cstop();
#endif /* SERCOMFORI2C_H_ */