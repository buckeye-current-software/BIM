/*
 * PowerDown.h
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#ifndef POWERDOWN_H_
#define POWERDOWN_H_


void PowerDown();
void UserPowerDown();
void NodePowerDown();
char isPowerOn();
void PowerDownISRSetup();
void StartPowerDownInt();

#define COMPVOLTAGE 	1.95
#define VDDA			3.0
#define VSSA			0.0

#endif /* POWERDOWN_H_ */
