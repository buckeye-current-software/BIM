/*
 * clock.h
 *
 *  Created on: Nov 12, 2013
 *      Author: Nathan
 */

#ifndef CLOCK_H_
#define CLOCK_H_

void ClockSetup();
void HeartBeat();

//todo USER: define clock
#define CLOCK_PERIOD 2500 //uS

#define HEARTBEAT_TICKS		10
#define DATAOUT_TICKS		1

//todo USER: CLOCK_TICKS_CLEAR should have the same number of zeros as clock_struct has elements (as seen below)
#define CLOCK_TICKS_CLEAR	{0,0}

typedef struct CLOCK_TICKS
{
	int HeartBeat;
	int DataOut;
}clock_struct;


#endif /* CLOCK_H_ */
