/*
 * BQ.h
 *
 *  Created on: Sep 13, 2012
 *      Author: LordLaptop
 */

#ifndef BQ_H_
#define BQ_H_

#include "op.h"

void BQ_Setup();
void InitBQ76PL536ports();
void bq_spi_fifo_init();
void BQ_Disable();
void BQ_SpiGpio();
char BIM_lowest_cell(ops_struct* op_func,unsigned short* volts);

//pin stuff
#define		READBQDRDY()			GpioDataRegs.GPBDAT.bit.GPIO34
#define 	EnableISO()				GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;
#define 	DisableISO()			GpioDataRegs.GPASET.bit.GPIO20 = 1;
#define 	SLAVEENSET()			GpioDataRegs.GPASET.bit.GPIO22 = 1;
#define 	SLAVEENCLEAR()			GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;


#define 	VALID 					1
#define 	INVALID					0

#endif /* BQ_H_ */
