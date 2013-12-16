/*
 * Boot.c
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 *      Updated on Nov. 29, Julia
 *
 */

#include "all.h"

// External Function
extern void Boot(Uint16);
extern void CopyData();
extern void BC_CAN_Boot(Uint16);
extern Uint32 GetLongData(void);

// Internal Functions
void BootInit(Uint16);
void Confirm(void);
void Retstart(void);
void SendConfirmationMsg(Uint16);


Uint16 ConfNbr = 0x00CC;

void Boot(Uint16 MesgID) {
	BootInit(MesgID);
	Restart(); // Restart() should be the same (copy it) as what is in common.c
}

void BootInit(Uint16 MesgID) {
	Uint32 EntryAddr;
	BC_CAN_Boot(MesgID);

	Confirm();

	ReadReservedFn();
	EntryAddr = GetLongData();
	CopyData();
}

// Handshake com protocol with CANcorder through bus
void Confirm() {
	//struct MBOX MboxS; Having issues typematching MBOXs to mbox signals/messageIDs, etc.
	//struct MBOX MboxR;

	//create 2 mailboxes (R and S) from those already set up (from can.CANsetup())
	// MBOX2 and MBOX3 used for this handshake

	ECanaMboxes.MBOX2.MSGID.bit.STDMSGID = COMMAND_ID;
	ECanaMboxes.MBOX2.MSGID.bit.AME = 0; 	// all bit must match (need this?)
	ECanaMboxes.MBOX2.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT (need this?)
	ECanaRegs.CANMD.bit.MD2 = 0;            //0 for transmit
	ECanaRegs.CANME.bit.ME2 = 1;            //enable
	ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 8;

	SendConfirmationMsg(ConfNbr);  // SendFill not FillSend (takes an INT)
}

/**
 * Confirm that node is online/onbus
 */
void SendConfirmationMsg(Uint16 confNbr) {
	struct CANmsg msg;
	msg.MBox = 2;
	msg.MDL.all = confNbr; //Send Confirmation number on MDL
	msg.MDH.all = 0; // no high data (MDH)
//	msg.MSGCTRL; //no change to Control

	ECanaMboxes.MBOX2.MSGCTRL = msg.MSGCTRL;	//set control
	ECanaMboxes.MBOX2.MDH = msg.MDH;			//set high
	ECanaMboxes.MBOX2.MDL = msg.MDL;			//set low

	ECanaRegs.CANTRS.bit.TRS2 = 1; 				//send if flagged
	while (ECanaRegs.CANTA.bit.TA2 != 1) {
	}		//wait to send
	ECanaRegs.CANTA.bit.TA2 = 1;				//clear flog (riblet! riblet!)e
}

void Flash() {

}
