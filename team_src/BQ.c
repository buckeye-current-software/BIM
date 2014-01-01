 /*
 *  ======== main.c ========
 */
#include "all.h"


char fualt;
char first_bq = 0;


void BQ_Setup()
{
    BQ_SpiGpio();

    bq_spi_fifo_init();

    InitBQ76PL536ports();

    EnableISO();
}

void BQ_Disable()
{
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0; // Configure GPIO12 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0; // Configure GPIO13 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0; // Configure GPIO14 as GPIO
}


void InitBQ76PL536ports()
{

	EALLOW;


	//Inputs
	GpioCtrlRegs.GPBPUD.bit.GPIO34 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 0;

	//Outputs
	//I/O port control pad
	//SLAVE SELECT
	GpioDataRegs.GPASET.bit.GPIO22 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;	// output
	GpioDataRegs.GPASET.bit.GPIO22 = 1;

	//ISO ENABLE
	GpioDataRegs.GPASET.bit.GPIO20 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;	// output
	GpioDataRegs.GPASET.bit.GPIO20 = 1;


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

void BQ_SpiGpio()
{

	EALLOW;

	/* Enable internal pull-up for the selected pins */
	// Pull-ups can be enabled or disabled disabled by the user.
	// This will enable the pullups for the specified pins.
	// Comment out other unwanted lines.

	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;     // Enable pull-up on GPIO12 (SPISIMOB)
	//  GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;     // Enable pull-up on GPIO24 (SPISIMOB)

	GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;     // Enable pull-up on GPIO13 (SPISOMIB)
	//  GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;     // Enable pull-up on GPIO25 (SPISOMIB)

	GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;     // Enable pull-up on GPIO14 (SPICLKB)
	//  GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;     // Enable pull-up on GPIO26 (SPICLKB)

	//    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;     // Enable pull-up on GPIO15 (SPISTEB)
	//  GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0;     // Enable pull-up on GPIO27 (SPISTEB)


	/* Set qualification for selected pins to asynch only */
	// This will select asynch (no qualification) for the selected pins.
	// Comment out other unwanted lines.

	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;   // Asynch input GPIO12 (SPISIMOB)
	//  GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 3;   // Asynch input GPIO24 (SPISIMOB)

	GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;   // Asynch input GPIO13 (SPISOMIB)
	//  GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 3;   // Asynch input GPIO25 (SPISOMIB)

	GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;   // Asynch input GPIO14 (SPICLKB)
	//  GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 3;   // Asynch input GPIO26 (SPICLKB)

	GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;   // Asynch input GPIO15 (SPISTEB)
	//  GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 3;   // Asynch input GPIO27 (SPISTEB)

	/* Configure SPI-B pins using GPIO regs*/
	// This specifies which of the possible GPIO pins will be SPI functional pins.
	// Comment out other unwanted lines.

	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 3;    // Configure GPIO12 as SPISIMOB
	//  GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 3;    // Configure GPIO24 as SPISIMOB

	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 3;    // Configure GPIO13 as SPISOMIB
	//  GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 3;    // Configure GPIO25 as SPISOMIB

	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 3;    // Configure GPIO14 as SPICLKB
	//  GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 3;    // Configure GPIO26 as SPICLKB

	//    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 3;    // Configure GPIO15 as SPISTEB
	//  GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 3;    // Configure GPIO27 as SPISTEB


	EDIS;
}



