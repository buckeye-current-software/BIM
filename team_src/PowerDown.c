/*
 * PowerDown.c
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#include "all.h"

void PowerDown()
{
	UserPowerDown();
	NodePowerDown();
	while(isPowerOn() == 0) {}
	Restart();
}

void UserPowerDown()
{
	//todo USER: special user power down
}

void NodePowerDown()
{
	//todo NATHAN: special node power down
	DINT;				// disable interrupts
}

char isPowerOn()
{
	return !Comp3Regs.COMPSTS.bit.COMPSTS; 	// 1 means cause interrupt = no power
											// 0 = no power
											//to correspond to True False invert
}

void PowerDownISRSetup()
{
	InitComp3Gpio();

	EALLOW;
	AdcRegs.ADCCTL1.bit.ADCBGPWD = 1;

	Comp3Regs.DACVAL.bit.DACVAL = (COMPVOLTAGE*1023)/(VDDA-VSSA);
	Comp3Regs.COMPCTL.bit.CMPINV = 1; //invert comp signal

	EPwm1Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP3OUT;
	EPwm1Regs.TZDCSEL.bit.DCAEVT2 = TZ_DCAH_HI;
	EPwm1Regs.DCACTL.bit.EVT2SRCSEL = DC_EVT2;
	EPwm1Regs.DCACTL.bit.EVT2FRCSYNCSEL = DC_EVT_SYNC;
	EPwm1Regs.TZEINT.bit.DCAEVT2 = 1;
	EPwm1Regs.TZCTL.bit.DCAEVT2 = TZ_NO_CHANGE;

	Comp3Regs.COMPCTL.bit.COMPDACEN = 1;

	EPwm1Regs.TZCLR.all = 0xFFFF;
	IER |= M_INT2;
	PieCtrlRegs.PIEIER2.bit.INTx1 = 1;

	EDIS;
}

// INT2.1
__interrupt void EPWM1_TZINT_ISR(void)    // EPWM-1
{
	ops.State = STATE_POWER_DOWN;
	ops.Change.bit.State = 1;
	// To receive more interrupts from this PIE group, acknowledge this interrupt
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}
