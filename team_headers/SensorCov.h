/*
 * SensorCov.h
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#ifndef SENSORCOV_H_
#define SENSORCOV_H_


#define INIT 		0
#define INIT_DELAY 	1
#define COV		 	2
#define MEASURE		3


void SensorCov();

void SensorCovInit();
void LatchStruct();
void SensorCovMeasure();
void UpdateStruct();
void SensorCovDeInit();




#endif /* SENSORCOV_H_ */
