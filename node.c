/*
 * main.c
 */


#include "all.h"

extern ops_struct ops;
Uint16 MesgID = 5;


void node()
{
	BootISRSetup();
	PowerDownISRSetup();
	ops.State = STATE_INIT;
	while(1)
	{
		NextState(MesgID);
	}
}

void NextState(Uint16 MesgID)
{
	switch (ops.State)
	{
	case STATE_INIT:
		Initilize();
		break;
	case STATE_SENSOR_COV:
		SensorCov();
		break;
	case STATE_BOOT:
		Boot(MesgID);
		break;
	case STATE_POWER_DOWN:
		PowerDown();
		break;
	default:
		Initilize();
	}
}


void BootISRSetup()
{
                           // Enable CPU INT1


	// GPIO0 and GPIO1 are inputs
	   EALLOW;
	   GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;         // GPIO
	   GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;          // input
	   GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 2;        // XINT1 Synch to SYSCLKOUT only
	   EDIS;

	// GPIO28 is XINT1, GPIO1 is XINT2
	   EALLOW;
	   GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 28;   // XINT1 is GPIO28
	   EDIS;

	// Configure XINT1
	   XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // Falling edge interrupt

	// Enable XINT1
	   XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // Enable XINT1


	   PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Group 1 INT4
	   IFR &= ~M_INT1;
	   IER |= M_INT1;
}





// INT1.4
__interrupt void  XINT1_ISR(void)
{
	// Insert ISR Code here
	Boot(MesgID);
	// To receive more interrupts from this PIE group, acknowledge this interrupt
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
