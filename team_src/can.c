/*
 * can.c
 *
 *  Created on: Nov 12, 2013
 *      Author: Nathan
 */
#include "all.h"

void CANSetup()
{
	struct ECAN_REGS ECanaShadow;
	InitECanaGpio();
	InitECana();

	ECanaShadow.CANMIM.all = 0;
	ECanaShadow.CANMIL.all = 0;
	ECanaShadow.CANGIM.all = 0;
	ECanaShadow.CANGAM.bit.AMI = 0; //must be standard
	ECanaShadow.CANGIM.bit.I1EN = 1;  // enable I1EN
	ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;

	//todo USER: Node specifc CAN setup

	// create mailbox for all Receive and transmit IDs
	// MBOX0 - MBOX31

	//Command RECEIVE
	ECanaShadow.CANMIM.bit.MIM0  = 1; 		//int enable
	ECanaShadow.CANMIL.bit.MIL0  = 1;  		// Int.-Level MB#1  -> I1EN
	ECanaMboxes.MBOX0.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX0.MSGID.bit.AME = 0;	// all bit must match
	ECanaMboxes.MBOX0.MSGID.bit.AAM = 0; 	// no RTR AUTO TRANSMIT
	ECanaRegs.CANMD.bit.MD0 = 1;			//receive
	ECanaRegs.CANME.bit.ME0 = 1;			//enable
	ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX0.MSGID.bit.STDMSGID = COMMAND_ID;

	//Heart TRANSMIT
	ECanaMboxes.MBOX1.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX1.MSGID.bit.AME = 0; 	// all bit must match
	ECanaMboxes.MBOX1.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT
	ECanaRegs.CANMD.bit.MD1 = 0; 			//transmit
	ECanaRegs.CANME.bit.ME1 = 1;			//enable
	ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX1.MSGID.bit.STDMSGID = HEARTBEAT_ID;



	EALLOW;
	ECanaRegs.CANGAM.all = ECanaShadow.CANGAM.all;
	ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;
	ECanaRegs.CANMIM.all = ECanaShadow.CANMIM.all;
	ECanaRegs.CANMIL.all = ECanaShadow.CANMIL.all;
    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.STM = 0;    // No self-test mode
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
    EDIS;

    //ENABLE PIE INTERRUPTS
    IER |= M_INT9;
    PieCtrlRegs.PIEIER9.bit.INTx6= 1;
}

// INT9.6
__interrupt void ECAN1INTA_ISR(void)  // eCAN-A
{
	Uint32 ops_id;
  	unsigned int mailbox_nr;
  	mailbox_nr = ECanaRegs.CANGIF1.bit.MIV1;
  	//todo USER: Setup ops command
  	if(mailbox_nr == 0)
  	{
  		//todo Nathan: Define Command frame
  		//proposed:
  		ops_id = ECanaMboxes.MBOX0.MDH.all;
  		dummy = ECanaMboxes.MBOX0.MDL.all;
		switch (ops_id)
		{
		case OPS_ID_STATE:
			memcpy(&ops.State,&dummy,sizeof ops.State);
			ops.Change.bit.State = 1;
			break;
		case OPS_ID_STOPWATCHERROR:
			memcpy(&ops.StopWatchError,&dummy,sizeof ops.StopWatchError);
			ops.Change.bit.StopWatchError = 1;
			break;
		default:
		}
  	}
  	//todo USER: Setup other reads

  	//To receive more interrupts from this PIE group, acknowledge this interrupt
  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

void SendFillCAN(struct CANmsg *msg, char issend)
{
	//todo USER: setup for all transmit MBOXs
	switch (msg->MBox)								//choose mailbox
	{
	case 1:
		ECanaMboxes.MBOX1.MSGCTRL = msg->MSGCTRL;	//set control
		ECanaMboxes.MBOX1.MDH = msg->MDH;			//set high
		ECanaMboxes.MBOX1.MDL = msg->MDL;			//set low
		if (issend == 1)
		{
			ECanaRegs.CANTRS.bit.TRS1 = 1; 				//send if flagged
			//todo Nathan: Stopwatch?
			while(ECanaRegs.CANTA.bit.TA1 != 1) {}		//wait to send
			ECanaRegs.CANTA.bit.TA1 = 1;				//clear flog
		}
		break;
	default:
	}
}
