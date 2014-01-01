/*
 * SensorCov().c
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#include "all.h"


ops_struct ops_temp;
data_struct data_temp;
stopwatch_struct* BIM_watch;

void SensorCov()
{
	SensorCovInit();
	while (ops.State == STATE_SENSOR_COV)
	{
		LatchStruct();
		SensorCovMeasure();
		UpdateStruct();
		if(data.update == 1)
		{
			FillCANData();
			data.update = 0;
		}
	}
	SensorCovDeInit();
}

void SensorCovInit()
{
	//todo USER: SensorCovInit()
	ops.BIM_State = INIT;
	ops.Flags.bit.BIM_init = 0;
	BQ_Setup();
}


void LatchStruct()
{
	memcpy(&ops_temp, &ops, sizeof(struct OPERATIONS));
	//memcpy(&data_temp, &data, sizeof(struct DATA));		//data is only changed in sensor measure
	ops.Change.all = 0;	//clear change states
}

void SensorCovMeasure()
{
	switch (ops_temp.BIM_State)
	{
	case INIT:
		ops_temp.Flags.bit.BIM_init = 0;
		if(NUMBER_OF_BQ_DEVICES != bq_pack_address_discovery())
		{
			ops_temp.BIM_State = INIT_DELAY;
		}
		else
		{
			bq_pack_init();
			ops_temp.BIM_State = COV;
			StopWatchRestart(BIM_watch);
			ops_temp.Flags.bit.BIM_init = 1;
		}
		BIM_watch = StartStopWatch(50000);	// half second delay
		break;
	case INIT_DELAY:
		if (isStopWatchComplete(BIM_watch) == 1) 	// if delayed enough
		{
			if(NUMBER_OF_BQ_DEVICES != bq_pack_address_discovery()) //try to init again
			{
				ops_temp.BIM_State = INIT_DELAY;					//if didn't work try again
				StopWatchRestart(BIM_watch);
			}
			else
			{
				bq_pack_init();
				ops_temp.BIM_State = COV;
				StopWatchRestart(BIM_watch);
				ops_temp.Flags.bit.BIM_init = 1;
			}
		}
		break;
	case COV:
		if (isStopWatchComplete(BIM_watch) == 1)					// if delayed conversion
		{
			BMM_Wake();
			bq_pack_start_conv();
			ops_temp.BIM_State = MEASURE;
		}
		break;
	case MEASURE:
		if (READBQDRDY() == 1)										//wait until data is ready
		{
			update_bq_pack_data();									//update data
			CellBalancing();										//balance if ops says so
			BMM_Sleep();
			data_temp.update = 1;									//actually latch data
			ops_temp.BIM_State = COV;
			StopWatchRestart(BIM_watch);
		}
		break;
	default:
		ops_temp.BIM_State = INIT;
	}
}

void UpdateStruct()
{
	if (data_temp.update == 1)
	{
		memcpy(&data, &data_temp, sizeof(struct DATA));
	}

	//todo USER: UpdateStruct
	//update with node specific op changes

	//if ops is not changed outside of sensor conversion copy temp over, otherwise don't change

	//Change bit is only set by ops changes outside of SensorCov.
	if (ops.Change.bit.State == 0)
	{
		ops.State = ops_temp.State;
	}

	if (ops.Change.bit.Balance == 0)
	{
		ops.State = ops_temp.Balance;
	}

	//don't change BIM State or Flags through CAN

	ops.BIM_State = ops_temp.BIM_State;
	ops.Flags.all = ops_temp.Flags.all;

	ops.Change.all = 0;	//clear change states
}

void SensorCovDeInit()
{
	//todo USER: SensorCovDeInit()
	StopStopWatch(BIM_watch);

}
