/*
 * spi.c
 *
 *  Created on: Dec 30, 2013
 *      Author: Nathan
 */

#include "all.h"

extern user_ops_struct ops_temp;
stopwatch_struct* SPI_watch;

unsigned int Send_SPI(unsigned char* a)
{
	SpibRegs.SPITXBUF=(*a)<<8;
	StopWatchRestart(SPI_watch);
	while((SpibRegs.SPIFFRX.bit.RXFFST !=1) && (isStopWatchComplete(SPI_watch) == 0));
	SpiaRegs.SPIFFRX.bit.RXFFOVFCLR=1;  // Clear Overflow flag
	if (isStopWatchComplete(SPI_watch) == 1)
	{
		ops_temp.UserFlags.bit.SPI_error = 1;
		return INVALID;
	}
	else
	{
		*a = SpibRegs.SPIRXBUF;
		SpibRegs.SPIRXBUF = 0;
		ops_temp.UserFlags.bit.SPI_error = 0;
		return VALID;
	}
}
