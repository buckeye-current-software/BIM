/*
 * op.h
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#ifndef OP_H_
#define OP_H_

//todo USER: define operation struct
//State defines
#define STATE_INIT 			0
#define STATE_SENSOR_COV 	1
#define STATE_BOOT			2
#define STATE_POWER_DOWN	3

#define OPS_ID_STATE 			0
#define OPS_ID_STOPWATCHERROR 	1

typedef struct CHANGE_OPS
{
	char State;
	char StopWatchError;
}change_struct;

//todo User: stopwatch errors
typedef struct STOPWATCHERROR
{
	char cov_error;
	char can_error;
}stopwatch_error_struct;

typedef struct OPERATIONS
{
  unsigned long State;
  union StopWatchError
  {
	  char						all;
	  stopwatch_error_struct 	bit;
  }								Stopwatch;
  union CHANGE
  {
  	long 			all;
  	change_struct 	bit;
  }					Change;
} ops_struct;


#endif /* OP_H_ */
