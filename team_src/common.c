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
