/*
 * Init.c
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#include "all.h"

ops_struct ops;
data_struct data;

void Initilize()
{
	BootISRSetup();
	OperationsSetup();
	DataSetup();
	CANSetup();
	ClockSetup();
}

void BootISRSetup()
{

}

void OperationsSetup()
{

}

void DataSetup()
{

}

void CANSetup()
{

}

void ClockSetup()
{

}
