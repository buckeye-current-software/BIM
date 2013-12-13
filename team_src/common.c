/*
 * common.c
 *
 *  Created on: Nov 6, 2013
 *      Author: Nathan
 */

#include "all.h"

void Restart()
{
	EALLOW;
	SysCtrlRegs.WDCR = 0x0028;               // Enable watchdog module
	SysCtrlRegs.WDKEY = 0x00;                // wrong key should restart
	SysCtrlRegs.WDKEY = 0x00;
	EDIS;
}

void* myMalloc(int size)
{
	//malloc is not reentrant so interrupts must be disabled to use it
	DINT;
	malloc(size);
	EINT;
}

void myFree(void* p)
{
	//free is not reentrant so interrupts must be disabled to use it
	DINT;
	free(p);
	EINT;
}
