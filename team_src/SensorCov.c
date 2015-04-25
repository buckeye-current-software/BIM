/*
 * SensorCov().c
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#include "all.h"

user_ops_struct ops_temp;
user_data_struct data_temp;
stopwatch_struct* BIM_watch;
stopwatch_struct* cov_watch;


char led;

void SensorCov()
{
	SensorCovInit(4);
	while (sys_ops.State == STATE_SENSOR_COV)
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
	user_ops.BIM_State = INIT;
	user_ops.UserFlags.bit.BIM_init = 0;
	BQ_Setup();

	EALLOW;
	GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;
	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;          // output
	GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 0;        //Synch to SYSCLKOUT only
	GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1; 		//disable pull up
	GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;
	EDIS;

	EALLOW;
	GpioDataRegs.GPASET.bit.GPIO4 = 1;
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;          // output
	GpioCtrlRegs.GPAQSEL1.bit.GPIO4 = 0;        //Synch to SYSCLKOUT only
	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1; 		//disable pull up
	GpioDataRegs.GPASET.bit.GPIO4 = 1;
	EDIS;

	EALLOW;
	GpioDataRegs.GPASET.bit.GPIO5 = 1;
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;          // output
	GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 0;        //Synch to SYSCLKOUT only
	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1; 		//disable pull up
	GpioDataRegs.GPASET.bit.GPIO5 = 1;
	EDIS;

	led = 1;
	cov_watch = StartStopWatch(100);					//delay for cov
	BIM_watch = StartStopWatch(100);
	while(isStopWatchComplete(BIM_watch) != 1);		//delay voltage regulator to start up
}



void SensorCovMeasure()
{
	int i  = 0;
	unsigned char reg_val[2];

	SensorCovSystemInit();
	switch (ops_temp.BIM_State)
	{
	case INIT:
		data_temp.update = 1; //cause an update to clear mailboxes
		BALLEDCLEAR();
		ops_temp.UserFlags.bit.BIM_init = 0;
		if(NUMBER_OF_BQ_DEVICES != bq_pack_address_discovery())
		{
			ops_temp.BIM_State = INIT_DELAY;
		}
		else
		{
			bq_pack_init();
			ops_temp.BIM_State = COV;
			StopWatchRestart(BIM_watch);
			ops_temp.UserFlags.bit.BIM_init = 1;
			  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
			  {
				  data_temp.bq_pack.bq_devs[i].cell_count = MAX_CELLS_NUMBER_IN_BQ;
				  bq_dev_read_status(&data_temp.bq_pack.bq_devs[i]);
				  bq_dev_read_reg(data_temp.bq_pack.bq_devs[i].device_address, ALERT_STATUS_REG, 1, DISCARD_CRC,(unsigned char*) &data_temp.bq_pack.bq_devs[i].alert_status);
				  bq_dev_read_reg(data_temp.bq_pack.bq_devs[i].device_address, FAULT_STATUS_REG, 1, DISCARD_CRC,(unsigned char*) &data_temp.bq_pack.bq_devs[i].fault_status);
				  bq_dev_read_reg(data_temp.bq_pack.bq_devs[i].device_address, ADDRESS_CONTROL_REG, 1, DISCARD_CRC, reg_val);
			  }
		}

		StopWatchRestartSetTime(BIM_watch,1000);	// half second delay
		break;
	case INIT_DELAY:
		BALLEDCLEAR();
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
				StopWatchRestartSetTime(BIM_watch,BIMUpdatePeriod);
				ops_temp.UserFlags.bit.BIM_init = 1;
				  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
				  {
					  bq_dev_read_status(&data_temp.bq_pack.bq_devs[i]);
					  bq_dev_read_reg(data_temp.bq_pack.bq_devs[i].device_address, ALERT_STATUS_REG, 1, DISCARD_CRC,(unsigned char*) &data_temp.bq_pack.bq_devs[i].alert_status);
					  bq_dev_read_reg(data_temp.bq_pack.bq_devs[i].device_address, FAULT_STATUS_REG, 1, DISCARD_CRC,(unsigned char*) &data_temp.bq_pack.bq_devs[i].fault_status);
					  bq_dev_read_reg(data_temp.bq_pack.bq_devs[i].device_address,ADDRESS_CONTROL_REG , 1, DISCARD_CRC, reg_val);
				  }
			}
		}
		break;
	case COV:
		if (isStopWatchComplete(BIM_watch) == 1)					// if delayed conversion
		{
			  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
			  {
				  bq_dev_read_status(&data_temp.bq_pack.bq_devs[i]);

			  }
			BMM_Wake();
			  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
			  {
				  bq_dev_read_status(&data_temp.bq_pack.bq_devs[i]);
				  bq_dev_read_reg(data_temp.bq_pack.bq_devs[i].device_address, ALERT_STATUS_REG, 1, DISCARD_CRC,(unsigned char*) &data_temp.bq_pack.bq_devs[i].alert_status);
				  bq_dev_read_reg(data_temp.bq_pack.bq_devs[i].device_address, FAULT_STATUS_REG, 1, DISCARD_CRC,(unsigned char*) &data_temp.bq_pack.bq_devs[i].fault_status);
				  bq_dev_read_reg(data_temp.bq_pack.bq_devs[i].device_address, ADDRESS_CONTROL_REG, 1, DISCARD_CRC, reg_val);
			  }
			bq_pack_start_conv();
			StopWatchRestart(cov_watch);
			ops_temp.BIM_State = MEASURE;
		}
		break;
	case MEASURE:
		if(isStopWatchComplete(cov_watch) == 1)
		{
			if (DRDY() == 1)										//wait until data is ready
			{
				update_bq_pack_data(); // update data
				  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
				  {
					  bq_dev_read_status(&data_temp.bq_pack.bq_devs[i]);
					  bq_dev_read_reg(data_temp.bq_pack.bq_devs[i].device_address, ALERT_STATUS_REG, 1, DISCARD_CRC,(unsigned char*) &data_temp.bq_pack.bq_devs[i].alert_status);
					  bq_dev_read_reg(data_temp.bq_pack.bq_devs[i].device_address, FAULT_STATUS_REG, 1, DISCARD_CRC,(unsigned char*) &data_temp.bq_pack.bq_devs[i].fault_status);
					  bq_dev_read_reg(data_temp.bq_pack.bq_devs[i].device_address, ADDRESS_CONTROL_REG, 1, DISCARD_CRC, reg_val);

				  }
				BMM_Sleep();
				//BIM_LED_Clear();
				data_temp.update = 1;									//actually latch data
				ops_temp.BIM_State = COV;
				StopWatchRestartSetTime(BIM_watch,BIMUpdatePeriod);

			}
		}
		break;
	default:
		ops_temp.BIM_State = INIT;
	}
	if (data_temp.bq_pack.highest_crc > 5)
	{
		ops_temp.BIM_State = INIT;
		data_temp.bq_pack.highest_crc = 0;
		ops_temp.UserFlags.bit.BIM_init = 0;
		data_temp.update = 1;
	}

	PerformSystemChecks();
}

void LatchStruct()
{
	LatchSystemStruct();
	ops_temp = user_ops;
	data_temp = user_data;
}

void UpdateStruct()
{
	SaveOpStates();
	user_data = data_temp;
	user_ops = ops_temp;
	//todo USER: UpdateStruct
	//update with node specific op changes

	//if ops is not changed outside of sensor conversion copy temp over, otherwise don't change

	//Change bit is only set by ops changes outside of SensorCov.


	DetermineOpStates();
}

void SensorCovDeInit()
{
	//todo USER: SensorCovDeInit()
	SystemSensorDeInit();
}
