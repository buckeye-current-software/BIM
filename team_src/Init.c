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
	OperationsSetup();
	DataSetup();
	CANSetup();
	ClockSetup();
	ops.State = STATE_SENSOR_COV;
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
