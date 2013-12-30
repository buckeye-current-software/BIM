/*
 * BQ.h
 *
 *  Created on: Sep 13, 2012
 *      Author: LordLaptop
 */

#ifndef BQ_H_
#define BQ_H_


void BQ_Setup();
void InitBQ76PL536ports();
void bq_spi_fifo_init();
void BQ_Disable();
void BQ_SpiaGpio();

#define		READBQDRDY()			GpioDataRegs.GPADAT.bit.GPIO20
#define 	VALID 					1
#define 	INVALID					0

#endif /* BQ_H_ */
