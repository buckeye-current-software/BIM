 /*
 *  ======== main.c ========
 */
#include "all.h"

/*
 * app that runs the BQ stack
 * uses bq_pack and spi_if for functions and spi COM
 *
 *EDIT:
 *change gpio pins to be correct
 *add hwi accordingly
 *
 * CONFIG FILE:
 * CREATE BQ_CLOCK
 * CREATE BQ_EVT
 */

extern bq_pack_t bq_pack;
char fualt;
char first_bq = 0;


void BQ_Setup()
{
    BQ_SpiaGpio();

    bq_spi_fifo_init();

    InitBQ76PL536ports();
}

void BQ_Disable()
{
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0; // Configure GPIO16 as GPIO
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0; // Configure GPIO17 as GPIO
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0; // Configure GPIO18 as GPIO
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0; // Configure GPIO19 as GPIO
}


void InitBQ76PL536ports()
{

	EALLOW;
	//Inputs
	// I/O port control pad

	// Set pull up/down resistor configuration


	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;



	// Set port direction as input


	GpioCtrlRegs.GPADIR.bit.GPIO5 = 0;



	//SET UP Dataready
	GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL=5;    // Connect GPIO 48 to XINT3
	XIntruptRegs.XINT3CR.bit.ENABLE=1;        // Enable XINT3
	XIntruptRegs.XINT3CR.bit.POLARITY = 1; //Raising edge



	//Output
	//I/O port control pad
	GpioDataRegs.GPASET.bit.GPIO4 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;	// output
	GpioDataRegs.GPASET.bit.GPIO4 = 1;
	//Set pin to Logic Level 0


	EDIS;

}

void bq_spi_fifo_init()
{
// Initialize SPI FIFO registers
   SpiaRegs.SPICCR.bit.SPISWRESET=0; 	// Reset SPI

   SpiaRegs.SPICCR.all=0x0007; 			//8-bit no loopback

   SpiaRegs.SPICTL.all=0x0006;       	//// Enable master mode, normal phase,enable talk, and SPI int disabled.

   SpiaRegs.SPISTS.all=0x0000;
   SpiaRegs.SPIBRR = 127; 				//Baudrate is slow as possible
  //SpiaRegs.SPIBRR=0x0063;           	// Baud rate

   SpiaRegs.SPIFFTX.all=0xC021;      	// Enable FIFO's, set TX FIFO level to 1 CHOOSE LEVEL ACCORDING TO APPLICATION

   SpiaRegs.SPIFFRX.all=0x0021;      	// Set RX FIFO level to 1
   SpiaRegs.SPIFFCT.all=0x00;

   SpiaRegs.SPIPRI.bit.FREE=1;

   SpiaRegs.SPICCR.bit.SPISWRESET=1;  	// Enable SPI

  SpiaRegs.SPIFFTX.bit.TXFIFO=1;
  SpiaRegs.SPIFFTX.bit.SPIRST=1;
  SpiaRegs.SPIFFRX.bit.RXFIFORESET=1;

}

void BQ_SpiaGpio()
{

   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up on GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up on GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO18 (SPICLKA)
//  GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO19 (SPISTEA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

/* Configure SPI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA
//  GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO19 as SPISTEA

    EDIS;
}

