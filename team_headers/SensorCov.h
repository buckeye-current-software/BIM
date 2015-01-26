/*
 * SensorCov.h
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#ifndef SENSORCOV_H_
#define SENSORCOV_H_

void SensorCov();
void SensorCovInit();
void SensorCovMeasure();
void LatchStruct();
void UpdateStruct();
void SensorCovDeInit();

#define FULL_SCALE 		(4096*0.9)

#define BIMUpdatePeriod 50000;
#define INIT 		0
#define INIT_DELAY 	1
#define COV		 	2
#define MEASURE		3

#define SENSOR_COV_STOPWATCH 100000

#endif /* SENSORCOV_H_ */
