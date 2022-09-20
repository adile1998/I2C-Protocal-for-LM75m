#include "app.h"
#include "clock.h"
#include "delay.h"

void AppInit(void)
{

	ClocksInit();
	
	// Assign LED0 as OUTPUT
	REG_PORT_DIR0 = LED0_PIN_MASK;
	
	// Set LED0 OFF
	REG_PORT_OUTCLR0 = LED0_PIN_MASK;
	
	PORT->Group[0].PINCFG[22].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[11].bit.PMUXE = 0x2; // pa22 is config as sercom3 pad[0] as UART
	
	PORT->Group[0].PINCFG[23].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[11].bit.PMUXO = 0x2; // pa23 is config as sercom3 pad[1] as UART
	
	PORT->Group[0].PINCFG[16].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[8].bit.PMUXE = 0x2; // pa16 is config as sercom1 pad[0] as i2c SDA
	
	PORT->Group[0].PINCFG[17].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[8].bit.PMUXO = 0x2; // pa17 is config as sercom1 pad[1] as i2c SCL
	
} // AppInit()


/*******************************************************************************
 * Function:        void AppRun(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function contains your main application
 *                  
 *
 * Note:
 *
 ******************************************************************************/
void AppRun(void)
{
	
	// Set the drive strength to strong
	PORT->Group[LED0_PORT].PINCFG[LED0_PIN_NUMBER].bit.DRVSTR = 1;
	
	while(1)
	{	
		// Turn the LED on PA17 ON

	}

} // Apprun()

