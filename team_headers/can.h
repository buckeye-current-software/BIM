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
void SendCANBatch(struct TRS_REG *TRS);
void FillSendCAN(unsigned Mbox);

//todo USER: DEFINE IDs and mailboxes for output
#define COMMAND_ID 0x01
#define COMMAND_BOX 0

#define HEARTBEAT_ID 0x00
#define HEARTBEAT_BOX 1

#endif /* CAN_H_ */
