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
	while (ops.State == STATE_SENSOR_COV)
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
	//CONFIG ADC
	adcinit();

	//CONFIG GP_BUTTON
	ConfigGPButton();

	//CONFIG LEDS
	//led 0
	ConfigLED0();
	//led 1
	ConfigLED1();
	//CONFIG 12V SWITCH
	Config12V();
}


void LatchStruct()
{
	memcpy(&ops_temp, &ops, sizeof(struct OPERATIONS));
	memcpy(&data_temp, &data, sizeof(struct DATA));
	ops.Change.all = 0;	//clear change states
}

void SensorCovMeasure()
{
	stopwatch_struct* watch = StartStopWatch(1);

	//todo USER: Sensor Conversion
	//update data_temp and ops_temp
	//use stopwatch to catch timeouts
	//waiting should poll isStopWatchComplete() to catch timeout and throw StopWatchError

	readADC();
	data_temp.adc = A0RESULT;

	data_temp.gp_button = READGPBUTTON();

	if (data_temp.gp_button == 0) 			//if pushed cause stopwatch
	{
		SETLED0();
		int i = 0;
		while (i < 100)
		{
			i++;
		}
	}
	else
	{
		CLEARLED0();
	}

	if (data_temp.adc > 500)
	{
		SETLED1();
	}
	else
	{
		CLEARLED0();
	}

	//exit and stopwatch error if timeout
	if (isStopWatchComplete(watch) == 1)
	{
		ops_temp.Stopwatch.bit.cov_error = 1;
	}
	else
	{
		ops_temp.Stopwatch.bit.cov_error = 0;
	}

	StopStopWatch(watch);

	if (ops_temp.Stopwatch.all != 0)
	{
		SET12V();
	}
	else
	{
		CLEAR12V();
	}


}

void UpdateStruct()
{
	memcpy(&data, &data_temp, sizeof(struct DATA));

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
		//only cov error happens inside of conversion so all other changes are considered correct.
		//update accordingly to correct cov_errors
		ops.Stopwatch.bit.cov_error = ops_temp.Stopwatch.bit.cov_error;
	}
	ops.Change.all = 0;	//clear change states
}

void SensorCovDeInit()
{
	//todo USER: SensorCovDeInit()
	CLEARLED0();
	CLEARLED1();
	CLEAR12V();
}
