/*
 * stopwatch.h
 *
 *  Created on: Nov 12, 2013
 *      Author: Nathan
 */

#ifndef STOPWATCH_H_
#define STOPWATCH_H_

typedef struct StopWatch
{
	Uint32 Start;
	unsigned int Time;
}stopwatch_struct;

void StopWatchSetUp(float time);
stopwatch_struct* StartStopWatch(unsigned int time);
char isStopWatchComplete(stopwatch_struct*);
void StopStopWatch(stopwatch_struct* watch);
void StopWatchRestart(stopwatch_struct* watch);


#define SENDCAN_STOPWATCH 	50 //100 uS

#endif /* STOPWATCH_H_ */
