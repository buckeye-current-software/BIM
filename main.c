/*
 * main.c
 */


#include "all.h"

extern ops_struct ops;

int main(void)
{
	StartUp();
	BootISRSetup();
	PowerDownISRSetup();
	ops.State = STATE_INIT;
	while(1)
	{
		NextState();
	}
}

void NextState()
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
		Boot();
		break;
	case STATE_POWER_DOWN:
		PowerDown();
		break;
	default:
		Initilize();
	}
}

void StartUp()
{
	// Step 1. Initialize System Control:
		// PLL, WatchDog, enable Peripheral Clocks
		// This example function is found in the DSP2803x_SysCtrl.c file.
		InitSysCtrl();

		// Step 2. Initalize GPIO:
		// This example function is found in the DSP2803x_Gpio.c file and
		// illustrates how to set the GPIO to it's default state.
		InitGpio();

		// Step 3. Clear all interrupts and initialize PIE vector table:
		// Disable CPU interrupts
		DINT;

		// Initialize PIE control registers to their default state.
		// The default state is all PIE interrupts disabled and flags
		// are cleared.
		// This function is found in the DSP2803x_PieCtrl.c file.
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

void BootISRSetup()
{
	   PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Group 1 INT4
	   IER |= M_INT1;                              // Enable CPU INT1


	// GPIO0 and GPIO1 are inputs
	   EALLOW;
	   GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;         // GPIO
	   GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;          // input
	   GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 0;        // XINT1 Synch to SYSCLKOUT only
	   EDIS;

	// GPIO28 is XINT1, GPIO1 is XINT2
	   EALLOW;
	   GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 28;   // XINT1 is GPIO28
	   EDIS;

	// Configure XINT1
	   XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // Falling edge interrupt

	// Enable XINT1
	   XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // Enable XINT1
}


// INT1.4
__interrupt void  XINT1_ISR(void)
{
  // Insert ISR Code here
	Restart();
  // To receive more interrupts from this PIE group, acknowledge this interrupt
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
