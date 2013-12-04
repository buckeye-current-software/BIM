/*
 * gpio.h
 *
 *  Created on: Dec 7, 2013
 *      Author: Nathan
 */

#ifndef GPIO_H_
#define GPIO_H_



#define 	CLEARLED0()				GpioDataRegs.GPACLEAR.bit.GPIO29 = 1
#define		SETLED0()				GpioDataRegs.GPASET.bit.GPIO29 = 1
#define 	TOGGLELED0()			GpioDataRegs.GPATOGGLE.bit.GPIO29 = 1

#define 	CLEARLED1()				GpioDataRegs.GPACLEAR.bit.GPIO9 = 1
#define		SETLED1()				GpioDataRegs.GPASET.bit.GPIO9 = 1
#define 	TOGGLELED1()			GpioDataRegs.GPATOGGLE.bit.GPIO9 = 1

#define 	CLEAR12V()				GpioDataRegs.GPACLEAR.bit.GPIO20 = 1
#define		SET12V()				GpioDataRegs.GPASET.bit.GPIO20 = 1
#define 	TOGGLE12V()				GpioDataRegs.GPATOGGLE.bit.GPIO20 = 1

#define		READGPBUTTON()			GpioDataRegs.GPADAT.bit.GPIO21

void ConfigGPButton();
void Config12V();
void ConfigLED1();
void ConfigLED0();





#endif /* GPIO_H_ */
