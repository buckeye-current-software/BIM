/*
 * op.h
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#ifndef OP_H_
#define OP_H_


//State defines
#define STATE_INIT 			0
#define STATE_SENSOR_COV 	1
#define STATE_BOOT			2
#define STATE_POWER_DOWN	3

typedef struct CHANGE_OPS
{
	int State;
}change_struct;


typedef struct OPERATIONS
{
  unsigned long State;
  union CHANGE
  {
  	long 			all;
  	change_struct 	bit;
  }					Change;
} ops_struct;


#endif /* OP_H_ */
