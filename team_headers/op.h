/*
 * op.h
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#ifndef OP_H_
#define OP_H_

//todo USER: define operation struct
//State defines. User states are greater than 3
#define STATE_USERSTATE      4;

//todo User: add user flags
typedef struct USER_FLAGS
{
	char BIM_init  : 1;
	char SPI_error: 1;
	char BQ_error : 4;
	char Temp_disconnect: 8;
} user_flags_struct;


typedef struct USER_OPERATIONS
{
  char user_op;

  union UserFlags
  {
	  int						all;
	  user_flags_struct			bit;
  }								UserFlags;
  unsigned long BIM_State;
} user_ops_struct;

#endif /* OP_H_ */
