/*
 * clock.c
 *
 *  Created on: Nov 12, 2013
 *      Author: Nathan
 */
#include "all.h"


clock_struct Clock_Ticks = CLOCK_TICKS_CLEAR;

void ClockSetup()
{
	SystemClockSetup();
	InitializeCpuTimer2(CLOCK_PERIOD);
	StartCpuTimer2();
}

// Connected to INT13 of CPU (use MINT13 mask):
__interrupt void INT13_ISR(void)     // INT13 or CPU-Timer1
{
	 //***********************************WARNING!!********************************************\\
	//BE CAREFUL YOU NEED TO ALLOW NESTING FOR ANY INTERRUPT THAT MIGHT HAPPEN IN THIS INTERRUPT\\

	EINT;		//enable all interrupts

	ClockHeartbeat(user_ops.UserFlags.all);

	RestartCPUTimer1();
	DINT;
}

// Connected to INT14 of CPU (use MINT14 mask):
// ISR can be used by the user.
__interrupt void INT14_ISR(void)     // INT14 or CPU-Timer2
{
	//***********************************WARNING!!********************************************\\
	//BE CAREFUL YOU NEED TO ALLOW NESTING FOR ANY INTERRUPT THAT MIGHT HAPPEN IN THIS INTERRUPT\\

	 EINT;		//enable all interrupts

	//todo USER: Define Clock ISR

	Clock_Ticks.DataOut++;

	if (Clock_Ticks.DataOut >= DATAOUT_TICKS)
	{
		//send data or fill data
		SendCAN(COOLANT_FLOW_BOX);
		SendCAN(MOTOR_TEMP_BOX);
		SendCAN(MOTOR_CONT_TEMP_BOX);
		SendCAN(RADIATOR_TEMP_BOX);
		SendCAN(COOLANT_PRESSURES_BOX);
		SendCAN(MOTOR_PLATE_TEMP_BOX);
		SendCAN(AMBIENT_TEMP_BOX);
		SendCAN(EMRAX_TEMP_BOX);
		Clock_Ticks.DataOut = 0;
	}

	RestartCpuTimer2();
	DINT;

}

