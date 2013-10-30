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
	while (isNewState() == 0)
	{
		LatchStruct();
		SensorCovMeasure();
		UpdateStruct();
	}
	SensorCovDeInit();
}

void SensorCovInit()
{

}

char isNewState()
{

}

void LatchStruct()
{

}

void SensorCovMeasure()
{

}

void UpdateStruct()
{

}

void SensorCovDeInit()
{

}
