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


void CANSetup();
void SendFillCAN(struct CANmsg *msg, char issend);

#define COMMAND_ID 0x01
#define COMMAND_BOX 0

#define HEARTBEAT_ID 0x00
#define HEARTBEAT_BOX 1

#endif /* CAN_H_ */
