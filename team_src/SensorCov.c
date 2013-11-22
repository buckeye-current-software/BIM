/*
 * SensorCov().c
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#include "all.h"


ops_struct ops_temp;
data_struct data_temp;

void SensorCov()
{
	SensorCovInit();
	while (ops.State != STATE_SENSOR_COV)
	{
		LatchStruct();
		SensorCovMeasure();
		UpdateStruct();
		FillCANData();
	}
	SensorCovDeInit();
}

void SensorCovInit()
{
	//todo USER: SensorCovInit()
}


void LatchStruct()
{
	memcpy(&ops, &ops_temp, sizeof ops);
	memcpy(&data, &data_temp, sizeof data);
	ops.Change.all = 0;	//clear change states
}

void SensorCovMeasure()
{

	float time = 10000; //uS change according to operation
	ConfigStopWatch(time);

	StartStopWatch();

	//todo USER: Sensor Conversion
	//update data_temp and ops_temp
	//use stopwatch to catch timeouts
	//waiting should poll isStopWatchComplete() to catch timeout and throw StopWatchError

	//exit and stopwatch error if timeout
	if (isStopWatchComplete() == 1)
	{
		ops_temp.StopWatchError = 1;
		return;
	}

	//no stopwatch error
	ops_temp.StopWatchError = 0;
}

void UpdateStruct()
{
	memcpy(&data_temp, &data, sizeof data);

	//todo USER: UpdateStruct
	//update with node specific op changes

	//if ops is not changed outside of sensor conversion copy temp over, otherwise don't change
	//Change bit is only set by ops changes outside of SensorCov.
	if (ops.Change.bit.State == 0)
	{
		ops.State = ops_temp.State;
	}

	if (ops.Change.bit.StopWatchError == 0)
	{
		ops.StopWatchError = ops_temp.StopWatchError;
	}

	ops.Change.all = 0;	//clear change states
}

void SensorCovDeInit()
{
	//todo USER: SensorCovDeInit()
}
