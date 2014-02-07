/*
 * can.h
 *
 *  Created on: Nov 13, 2013
 *      Author: Nathan
 */

#ifndef CAN_H_
#define CAN_H_




struct CANmsg {
   char MBox;
   union CANMSGCTRL_REG   MSGCTRL;
   union CANMDL_REG       MDL;
   union CANMDH_REG       MDH;
};

struct TRS_REG {
	union CANTRS_REG	TRS;
};

void CANSetup();
char FillCAN(unsigned int Mbox);
void SendCAN(unsigned int Mbox);
void FillCANData();
void FillSendCAN(unsigned int Mbox);
void ClearMailBoxes();

signed int Cell_Send(int cell_num);

//todo USER: DEFINE IDs and mailboxes for output
#define COMMAND_ID 		0xB0
#define COMMAND_BOX 	0

#define HEARTBEAT_ID 	0x3A
#define HEARTBEAT_BOX 	1

#define Stats1_ID		0x300
#define Stats1_box		2

#define Stats2_ID		0x301
#define Stats2_box		3

#define Cell_1_4_ID			0x310
#define Cell_1_4_box		4

#define Cell_5_8_ID			0x311
#define Cell_5_8_box		5


#define Cell_9_12_ID		0x312
#define Cell_9_12_box		6

#define Cell_13_16_ID		0x313
#define Cell_13_16_box		7

#define Cell_17_20_ID		0x314
#define Cell_17_20_box		8

#define Cell_21_24_ID		0x315
#define Cell_21_24_box		9

#define Cell_25_28_ID		0x316
#define Cell_25_28_box		10

#define Cell_29_32_ID		0x317
#define Cell_29_32_box		11

#define BIM2_ID				0x302
#define BIM2_box			12

#define BIM3_ID				0x304
#define BIM3_box			13

#define Temp1_ID			0x335
#define Temp1_box			14

#define Temp2_ID			0x336
#define Temp2_box			15

#define Temp3_ID			0x337
#define Temp3_box			16

#endif /* CAN_H_ */
