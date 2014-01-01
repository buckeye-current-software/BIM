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
	StartPowerDownInt();			//start power down interrupt, this will change state to PowerDown if necessary
	StopWatchSetUp(10);
}



void OperationsSetup()
{
	//todo USER: OperationsSetup()
	//add here and ops.h
	ops.Change.all = 0;
	ops.Flags.all = 0;
	ops.State = STATE_SENSOR_COV;
	ops.Update_period = 50000;
}

void DataSetup()
{
	//todo USER: DataSetup()
	//add here and data.h
	data.update = 0;
}

