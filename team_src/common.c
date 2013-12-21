/*
 * common.c
 *
 *  Created on: Nov 6, 2013
 *      Author: Nathan
 */

#include "all.h"



void* myMalloc(int size)
{
	//malloc is not reentrant so interrupts must be disabled to use it
	DINT;
	malloc(size);
	EINT;
}

void myFree(void* p)
{
	//free is not reentrant so interrupts must be disabled to use it
	DINT;
	free(p);
	EINT;
}
