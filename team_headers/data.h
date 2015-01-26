/*
 * data.h
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#ifndef DATA_H_
#define DATA_H_

#include "bq_pack.h"

typedef union CANFLOAT
{
	float F32;
	unsigned long U32;
} canfloat;

typedef struct USER_DATA
{
	  char update;
	  bq_pack_t bq_pack;
} user_data_struct;

#endif /* DATA_H_ */
