/*
 * spi.c
 *
 *  Created on: Dec 30, 2013
 *      Author: Nathan
 */

#include "all.h"

extern ops_struct ops_temp;
stopwatch_struct* SPI_watch;

unsigned int Send_SPI(unsigned char* a)
{
	SpibRegs.SPITXBUF=(*a)<<8;
	while((SpibRegs.SPIFFRX.bit.RXFFST !=1));
	SpiaRegs.SPIFFRX.bit.RXFFOVFCLR=1;  // Clear Overflow flag

	*a = SpibRegs.SPIRXBUF;
	SpibRegs.SPIRXBUF = 0;
	ops_temp.Flags.bit.SPI_error = 0;
	return VALID;
}
