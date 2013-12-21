/*
 * main.c
 *
 *  Created on: Dec 21, 2013
 *      Author: Nathan
 */

#include "Boot.h"
#include "main.h"
#include "node.h"
#include "DSP2803x_GlobalPrototypes.h"

#define FlashStart		0x003f2000
#define Erased			0xFFFF

int main(void)
{
	StartUp();
	if (*(char *)FlashStart == Erased ) //if start of flash is 0xFFFF then it is erased
	{
		Boot(1);						//so go to boot and wait to be flashed
	}
	node();
}

void StartUp()
{
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart,  (unsigned long)&RamfuncsLoadSize);

	InitSysCtrl();

	InitGpio();
	DINT;

	InitPieCtrl();

	// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR).
	// This will populate the entire table, even if the interrupt
	// is not used in this example.  This is useful for debug purposes.
	// The shell ISR routines are found in DSP2803x_DefaultIsr.c.
	// This function is found in DSP2803x_PieVect.c.
	InitPieVectTable();

	//Initialize Flash
	//InitFlash();

	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM
}

void Restart()
{
	EALLOW;
	SysCtrlRegs.WDCR = 0x0028;               // Enable watchdog module
	SysCtrlRegs.WDKEY = 0x00;                // wrong key should restart
	SysCtrlRegs.WDKEY = 0x00;
	EDIS;

	while(1){}
}
