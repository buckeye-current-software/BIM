/*
 * gpio.c
 *
 *  Created on: Dec 7, 2013
 *      Author: Nathan
 */

#include "all.h"

void ConfigLED0()
{
	EALLOW;
	CLEARLED0();
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO29 = 1;          // output
	GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 0;        //Synch to SYSCLKOUT only
	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 1; 		//disable pull up
	EDIS;
}

void ConfigLED1()
{
	EALLOW;
	CLEARLED1();
	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;          // output
	GpioCtrlRegs.GPAQSEL1.bit.GPIO9 = 0;        //Synch to SYSCLKOUT only
	GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1; 		//disable pull up
	EDIS;
}

void Config12V()
{
	EALLOW;
	CLEAR12V();
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;          // output
	GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0;        //Synch to SYSCLKOUT only
	GpioCtrlRegs.GPAPUD.bit.GPIO20 = 1; 		//disable pull up
	EDIS;
}

void ConfigGPButton()
{
	EALLOW;
	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;          // input
	GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 0;        //Synch to SYSCLKOUT only
	GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0; 		//enable pull up
	EDIS;
}
