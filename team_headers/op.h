/*
 * op.h
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#ifndef OP_H_
#define OP_H_

#include "stopwatch.h"

//todo USER: define operation struct
//State defines
#define STATE_INIT 			0
#define STATE_SENSOR_COV 	1
#define STATE_BOOT			2
#define STATE_POWER_DOWN	3

#define OPS_ID_STATE 			0
#define OPS_ID_STOPWATCHERROR 	1
#define OPS_ID_UPDATE_PERIOD	2

#define EX_BIM	2  	//NUMBER OF EXTERNAL BIMs
#define BIM2 	0	//index of BIM2
#define BIM3	1	//index of BIM3

typedef struct BIM
{
	stopwatch_struct* Reset_stopwatch;
	unsigned short lowest_cell_volts;
}bim_struct;


typedef struct CHANGE_OPS
{
	char State : 1;
	char Flags : 1;
	char BIM_State : 1;
	char Balance : 1;
	char Update_Period : 1;
}change_struct;

//todo User: stopwatch errors
typedef struct FLAGS
{
	char BIM_init  : 1;
	char SPI_error: 1;
	char can_error : 1;
	char BQ_error : 6;
}flags_struct;

typedef struct OPERATIONS
{
  unsigned long State;
  union Flag
  {
	  Uint16					all;
	  flags_struct 				bit;
  }								Flags;
  union CHANGE
  {
  	long 						all;
  	change_struct 				bit;
  }								Change;
  unsigned long BIM_State;
  unsigned long Balance;
  unsigned long Update_period;   //10 microseconds
  bim_struct BIM[EX_BIM];
  char Bal_Button;
} ops_struct;


#endif /* OP_H_ */
