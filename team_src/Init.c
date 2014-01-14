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
	int i;
	ops.Change.all = 0;
	ops.Flags.all = 0;
	ops.State = STATE_SENSOR_COV;
	ops.Update_period = 50000;
	for(i = 0; i<EX_BIM; i++)
	{
		ops.BIM[i].Reset_stopwatch = StartStopWatch(500000); //5 second
		ops.BIM[i].lowest_cell_volts = 4000;
	}
	ops.Balance = 0;
}

void DataSetup()
{

	//todo USER: DataSetup()
	//add here and data.h
	data.update = 0;

}

