/*
 * PowerDown.c
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#include "all.h"

void PowerDown()
{
	UserPowerDown();
	NodePowerDown();
	while(isPowerOn() == 0) {}
	Restart();
}

void UserPowerDown()
{

}

void NodePowerDown()
{

}

char isPowerOn()
{

}
