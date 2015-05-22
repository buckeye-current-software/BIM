/*
 * can.h
 *
 *  Created on: Nov 13, 2013
 *      Author: Nathan
 */

#ifndef CAN_H_
#define CAN_H_

void CANSetup();
char FillCAN(unsigned int Mbox);
void FillCANData();
void FillSendCAN(unsigned int Mbox);
void ClearMailBoxes();
void BUS_OFF();
unsigned int Cell_Send(int cell_num);

//todo USER: DEFINE IDs for all output
//           DEFINE mailboxes for all output except heartbeat and command
//			 Mailboxes and ID's 0 or 1 are reserved for system use.



#define HEARTBEAT_BOX 	1


#define Stats1_box		2


#define Stats2_box		3


#define Cell_1_4_box		4


#define Cell_5_8_box		5



#define Cell_9_12_box		6


#define Cell_13_16_box		7


#define Cell_17_20_box		8


#define Cell_21_24_box		9



#define Temp1_box			14


#define Temp2_box			15


#define Temp3_box			16


#define Temp4_box			17



#endif /* CAN_H_ */
