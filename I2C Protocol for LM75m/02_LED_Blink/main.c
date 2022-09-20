//////////////////////////////////////////////////////////////////////////
// Include and defines
//////////////////////////////////////////////////////////////////////////
#include "sam.h"
#include "definitions.h"
#include "delay.h"
#include "app.h"
#include "SERCOMforUSART.h"
#include "SERCOMforI2C.h"


//////////////////////////////////////////////////////////////////////////
// Function Prototypes
//////////////////////////////////////////////////////////////////////////

void ClocksInit(void);	// Configure Clock, Wait States and synch, bus clocks for 48MHz operation
void readtolm75();
static char * _float_to_char(float x, char *p);
void adding();

//////////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////////

uint8_t GelenData[100];
int i = 0;
float cc = 0x00;
uint8_t deger1, deger2;
uint8_t CHAR_BUFF_SIZE = 5;

/*******************************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This is our main function
 *
 * Note:
 *
 ******************************************************************************/
int main(void){
	AppInit(); // clock has been started and port which is needed is config.
	uartInit(9600);
	i2cinit(100000); 
	// Super loop
	while(1)
	{
		readtolm75();
		adding();
		delay_ms(1000);	 
	}
	
} // main()

void readtolm75(){
	
	i2cAddresswrite(0x4f);
	i2cpointerwrite(0x00);
	i2cRepeatedstartaddress(0x4f);
	i2csendAck();
	deger1 = i2cread();
	i2csendNack();
	deger2 = i2cread();
	i2cstop();

}
void adding(){
	
	deger1 <<= 1;
	deger2 &= 0x80;
	deger2 >>= 7;
	cc = (deger1 + deger2);
	cc *= 0.5f;
	char buf[7];
	buf[5] = '\r';
	buf[6] = '\n';
	_float_to_char(cc, buf);
	uartWrite(buf, 7);
}

static char * _float_to_char(float x, char *p) {
	char *s = p + CHAR_BUFF_SIZE; // go to end of buffer
	uint16_t decimals;  // variable to store the decimals
	int units;  // variable to store the units (part to left of decimal place)
	if (x < 0) { // take care of negative numbers
		decimals = (int)(x * -100) % 100; // make 1000 for 3 decimals etc.
		units = (int)(-1 * x);
		} 
	else { // positive numbers
		decimals = (int)(x * 100) % 100;
		units = (int)x;
	}

	*--s = (decimals % 10) + '0';
	decimals /= 10; // repeat for as many decimal places as you need
	*--s = (decimals % 10) + '0';
	*--s = '.';

	while (units > 0) {
		*--s = (units % 10) + '0';
		units /= 10;
	}
	if (x < 0) *--s = '-'; // unary minus sign for negative numbers
	return s;
}

void SERCOM3_Handler(){
	
	/* it is not used for this example sir :)
	
	GelenData[i] = SERCOM3->USART.DATA.bit.DATA; // This flag is cleared by reading the Data register (DATA) or by disabling the receiver.

	if (GelenData[i] == '*')
	{
		uartWrite(GelenData + i, 1);
		i = 0;
		uint8_t sonKontrol[] = "son deger geldi\r";
		uartWrite(sonKontrol, 16);
	}
	else{
		uartWrite(GelenData + i, 1);
		i += 1;
	}
	*/
}
