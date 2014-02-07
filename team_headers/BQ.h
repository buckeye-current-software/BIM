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
void Check_Bal_Button();
void Flash_Bal_LED();

//pin stuff
//inputs
#define		READBQDRDY()			GpioDataRegs.GPBDAT.bit.GPIO34
#define		READBALBUTTON()			GpioDataRegs.GPADAT.bit.GPIO27
//outputs
#define 	EnableISO()				GpioDataRegs.GPASET.bit.GPIO20 = 1
#define 	DisableISO()			GpioDataRegs.GPACLEAR.bit.GPIO20 = 1
#define 	SLAVEENSET()			GpioDataRegs.GPASET.bit.GPIO22 = 1
#define 	SLAVEENCLEAR()			GpioDataRegs.GPACLEAR.bit.GPIO22 = 1
#define 	BALLEDSET()				GpioDataRegs.GPASET.bit.GPIO4 = 1;
#define 	BALLEDCLEAR()			GpioDataRegs.GPACLEAR.bit.GPIO4 = 1
#define 	BALLEDTOGGLE()			GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1;

#define 	VALID 					1
#define 	INVALID					0

#define 	SPI_STOPWATCH_TIME		200

#endif /* BQ_H_ */
