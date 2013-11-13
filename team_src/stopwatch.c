/*
 * stopwatch.c
 *
 *  Created on: Nov 12, 2013
 *      Author: Nathan
 */

#include "all.h"
#include "DSP2803x_CpuTimers.h"

struct CPUTIMER_VARS StopWatch;

void ConfigStopWatch(float time)
{
    // CPU Timer0
	// Initialize address pointers to respective timer registers:
	StopWatch.RegsAddr = &CpuTimer0Regs;
	// Initialize timer period to maximum:
	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
	CpuTimer0Regs.TPR.all  = 0;
	CpuTimer0Regs.TPRH.all = 0;
	// Make sure timer is stopped:
	CpuTimer0Regs.TCR.bit.TSS = 1;
	// Reload all counter register with period value:
	CpuTimer0Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	StopWatch.InterruptCount = 0;

	ConfigCpuTimer(&StopWatch,CPU_FREQ_MHZ , time);

	//pie interrupt
	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}

void StartStopWatch()
{
	StopWatch.InterruptCount = 0;
	ReloadCpuTimer0();
	StartCpuTimer0();
}

char isStopWatchComplete()
{
	return StopWatch.InterruptCount;
}

// INT1.7
__interrupt void  TINT0_ISR(void)      // CPU-Timer 0
{
	StopWatch.InterruptCount = 1;
   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
