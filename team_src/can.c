/*
 * can.c
 *
 *  Created on: Nov 12, 2013
 *      Author: Nathan
 */
#include "all.h"

extern data_struct data;
unsigned int mask;
stopwatch_struct* can_watch;
struct ECAN_REGS ECanaShadow;

void CANSetup()
{

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
	EALLOW;

	// create mailbox for all Receive and transmit IDs
	// MBOX0 - MBOX31

	//Command RECEIVE
	ECanaMboxes.MBOX0.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX0.MSGID.bit.AME = 0;	// all bit must match
	ECanaMboxes.MBOX0.MSGID.bit.AAM = 0; 	// no RTR AUTO TRANSMIT
	ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX0.MSGID.bit.STDMSGID = COMMAND_ID;
	ECanaShadow.CANMD.bit.MD0 = 1;			//receive
	ECanaShadow.CANME.bit.ME0 = 1;			//enable
	ECanaShadow.CANMIM.bit.MIM0  = 1; 		//int enable
	ECanaShadow.CANMIL.bit.MIL0  = 1;  		// Int.-Level MB#0  -> I1EN

	//Heart TRANSMIT
	ECanaMboxes.MBOX1.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX1.MSGID.bit.AME = 0; 	// all bit must match
	ECanaMboxes.MBOX1.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT
	ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 2;
	ECanaMboxes.MBOX1.MSGID.bit.STDMSGID = HEARTBEAT_ID;
	ECanaShadow.CANMD.bit.MD1 = 0; 			//transmit
	ECanaShadow.CANME.bit.ME1 = 1;			//enable

	//Cell Stats 1 TRANSMIT
	ECanaMboxes.MBOX2.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX2.MSGID.bit.AME = 0; 	// all bit must match
	ECanaMboxes.MBOX2.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT
	ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 7;
	ECanaMboxes.MBOX2.MSGID.bit.STDMSGID = Stats1_ID;
	ECanaShadow.CANMD.bit.MD2 = 0; 			//transmit
	ECanaShadow.CANME.bit.ME2 = 1;			//enable

	//Cell Stats 2 TRANSMIT
	ECanaMboxes.MBOX3.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX3.MSGID.bit.AME = 0; 	// all bit must match
	ECanaMboxes.MBOX3.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT
	ECanaMboxes.MBOX3.MSGCTRL.bit.DLC = 5;
	ECanaMboxes.MBOX3.MSGID.bit.STDMSGID = Stats2_ID;
	ECanaShadow.CANMD.bit.MD3 = 0; 			//transmit
	ECanaShadow.CANME.bit.ME3 = 1;			//enable

	//Cell 1-4 TRANSMIT
	ECanaMboxes.MBOX4.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX4.MSGID.bit.AME = 0; 	// all bit must match
	ECanaMboxes.MBOX4.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT
	ECanaMboxes.MBOX4.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX4.MSGID.bit.STDMSGID = Cell_1_4_ID;
	ECanaShadow.CANMD.bit.MD4 = 0; 			//transmit
	ECanaShadow.CANME.bit.ME4 = 1;			//enable

	//Cell 5-8 TRANSMIT
	ECanaMboxes.MBOX5.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX5.MSGID.bit.AME = 0; 	// all bit must match
	ECanaMboxes.MBOX5.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT
	ECanaMboxes.MBOX5.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX5.MSGID.bit.STDMSGID = Cell_5_8_ID;
	ECanaShadow.CANMD.bit.MD5 = 0; 			//transmit
	ECanaShadow.CANME.bit.ME5 = 1;			//enable

	//Cell 9-12 TRANSMIT
	ECanaMboxes.MBOX6.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX6.MSGID.bit.AME = 0; 	// all bit must match
	ECanaMboxes.MBOX6.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT
	ECanaMboxes.MBOX6.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX6.MSGID.bit.STDMSGID = Cell_9_12_ID;
	ECanaShadow.CANMD.bit.MD6 = 0; 			//transmit
	ECanaShadow.CANME.bit.ME6 = 1;			//enable

	//Cell 13-16 TRANSMIT
	ECanaMboxes.MBOX7.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX7.MSGID.bit.AME = 0; 	// all bit must match
	ECanaMboxes.MBOX7.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT
	ECanaMboxes.MBOX7.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX7.MSGID.bit.STDMSGID = Cell_13_16_ID;
	ECanaShadow.CANMD.bit.MD7 = 0; 			//transmit
	ECanaShadow.CANME.bit.ME7 = 1;			//enable

	//Cell 17-20 TRANSMIT
	ECanaMboxes.MBOX8.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX8.MSGID.bit.AME = 0; 	// all bit must match
	ECanaMboxes.MBOX8.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT
	ECanaMboxes.MBOX8.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX8.MSGID.bit.STDMSGID = Cell_17_20_ID;
	ECanaShadow.CANMD.bit.MD8 = 0; 			//transmit
	ECanaShadow.CANME.bit.ME8 = 1;			//enable

	//Cell 21-24 TRANSMIT
	ECanaMboxes.MBOX9.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX9.MSGID.bit.AME = 0; 	// all bit must match
	ECanaMboxes.MBOX9.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT
	ECanaMboxes.MBOX9.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX9.MSGID.bit.STDMSGID = Cell_21_24_ID;
	ECanaShadow.CANMD.bit.MD9 = 0; 			//transmit
	ECanaShadow.CANME.bit.ME9 = 1;			//enable

	//Cell 25-28 TRANSMIT
	ECanaMboxes.MBOX10.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX10.MSGID.bit.AME = 0; 	// all bit must match
	ECanaMboxes.MBOX10.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT
	ECanaMboxes.MBOX10.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX10.MSGID.bit.STDMSGID = Cell_25_28_ID;
	ECanaShadow.CANMD.bit.MD10 = 0; 			//transmit
	ECanaShadow.CANME.bit.ME10 = 1;			//enable

	//Cell 29-32 TRANSMIT
	ECanaMboxes.MBOX11.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX11.MSGID.bit.AME = 0; 	// all bit must match
	ECanaMboxes.MBOX11.MSGID.bit.AAM = 1; 	//RTR AUTO TRANSMIT
	ECanaMboxes.MBOX11.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX11.MSGID.bit.STDMSGID = Cell_29_32_ID;
	ECanaShadow.CANMD.bit.MD11 = 0; 			//transmit
	ECanaShadow.CANME.bit.ME11 = 1;			//enable


	//BIM2 RECEIVE
	ECanaMboxes.MBOX12.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX12.MSGID.bit.AME = 0;	// all bit must match
	ECanaMboxes.MBOX12.MSGID.bit.AAM = 0; 	// no RTR AUTO TRANSMIT
	ECanaMboxes.MBOX12.MSGCTRL.bit.DLC = 6;
	ECanaMboxes.MBOX12.MSGID.bit.STDMSGID = BIM2_ID;
	ECanaShadow.CANMD.bit.MD12 = 1;			//receive
	ECanaShadow.CANME.bit.ME12 = 1;			//enable
	ECanaShadow.CANMIM.bit.MIM12  = 1; 		//int enable
	ECanaShadow.CANMIL.bit.MIL12  = 1;  	// Int.-Level MB#0  -> I1EN

	//BIM3 RECEIVE
	ECanaMboxes.MBOX13.MSGID.bit.IDE = 0; 	//standard id
	ECanaMboxes.MBOX13.MSGID.bit.AME = 0;	// all bit must match
	ECanaMboxes.MBOX13.MSGID.bit.AAM = 0; 	// no RTR AUTO TRANSMIT
	ECanaMboxes.MBOX13.MSGCTRL.bit.DLC = 6;
	ECanaMboxes.MBOX13.MSGID.bit.STDMSGID = BIM3_ID;
	ECanaShadow.CANMD.bit.MD13 = 1;			//receive
	ECanaShadow.CANME.bit.ME13 = 1;			//enable
	ECanaShadow.CANMIM.bit.MIM13  = 1; 		//int enable
	ECanaShadow.CANMIL.bit.MIL13  = 1;  	// Int.-Level MB#0  -> I1EN

	ECanaRegs.CANGAM.all = ECanaShadow.CANGAM.all;
	ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;
	ECanaRegs.CANMIM.all = ECanaShadow.CANMIM.all;
	ECanaRegs.CANMIL.all = ECanaShadow.CANMIL.all;
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;
    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.STM = 0;    // No self-test mode
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
    EDIS;

    //ENABLE PIE INTERRUPTS
    IER |= M_INT9;
    PieCtrlRegs.PIEIER9.bit.INTx6= 1;

    can_watch = StartStopWatch(SENDCAN_STOPWATCH);
}



char FillCAN(unsigned int Mbox)
{
	//todo USER: setup for all transmit MBOXs
	struct ECAN_REGS ECanaShadow;
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	switch (Mbox)								//choose mailbox
	{
	case HEARTBEAT_BOX:
		EALLOW;
		ECanaShadow.CANMC.bit.MBNR = Mbox;
		ECanaShadow.CANMC.bit.CDR = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		ECanaMboxes.MBOX1.MDH.all = 0;
		ECanaMboxes.MBOX1.MDL.all = 0;
		ECanaMboxes.MBOX1.MDL.word.LOW_WORD = ops.Flags.all;
		ECanaShadow.CANMC.bit.MBNR = 0;
		ECanaShadow.CANMC.bit.CDR = 0;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
		return 1;
	case Stats1_box:
		EALLOW;
		ECanaShadow.CANMC.bit.MBNR = Mbox;
		ECanaShadow.CANMC.bit.CDR = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		ECanaMboxes.MBOX2.MDL.word.LOW_WORD = data.bq_pack.highest_cell_volts;
		ECanaMboxes.MBOX2.MDL.byte.BYTE2 = data.bq_pack.highest_cell_num;
		ECanaMboxes.MBOX2.MDH.word.LOW_WORD = data.bq_pack.lowest_cell_volts;
		ECanaMboxes.MBOX2.MDH.byte.BYTE6 = data.bq_pack.lowest_cell_num;
		ECanaShadow.CANMC.bit.MBNR = 0;
		ECanaShadow.CANMC.bit.CDR = 0;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
		return 1;
	case Stats2_box:
		EALLOW;
		ECanaShadow.CANMC.bit.MBNR = Mbox;
		ECanaShadow.CANMC.bit.CDR = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		ECanaMboxes.MBOX3.MDL.word.LOW_WORD = data.bq_pack.average;
		ECanaMboxes.MBOX3.MDL.word.HI_WORD = data.bq_pack.std_dev;
		ECanaMboxes.MBOX3.MDH.byte.BYTE4 = data.bq_pack.bal_num;
		ECanaShadow.CANMC.bit.MBNR = 0;
		ECanaShadow.CANMC.bit.CDR = 0;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
		return 1;
	case Cell_1_4_box:
		EALLOW;
		ECanaShadow.CANMC.bit.MBNR = Mbox;
		ECanaShadow.CANMC.bit.CDR = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		ECanaMboxes.MBOX4.MDL.word.LOW_WORD = Cell_Send(1);
		ECanaMboxes.MBOX4.MDL.word.HI_WORD = Cell_Send(2);
		ECanaMboxes.MBOX4.MDH.word.LOW_WORD = Cell_Send(3);
		ECanaMboxes.MBOX4.MDH.word.HI_WORD = Cell_Send(4);
		ECanaShadow.CANMC.bit.MBNR = 0;
		ECanaShadow.CANMC.bit.CDR = 0;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
		return 1;
	case Cell_5_8_box:
		EALLOW;
		ECanaShadow.CANMC.bit.MBNR = Mbox;
		ECanaShadow.CANMC.bit.CDR = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		ECanaMboxes.MBOX5.MDL.word.LOW_WORD = Cell_Send(5);
		ECanaMboxes.MBOX5.MDL.word.HI_WORD = Cell_Send(6);
		ECanaMboxes.MBOX5.MDH.word.LOW_WORD = Cell_Send(7);
		ECanaMboxes.MBOX5.MDH.word.HI_WORD = Cell_Send(8);
		ECanaShadow.CANMC.bit.MBNR = 0;
		ECanaShadow.CANMC.bit.CDR = 0;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
		return 1;
	case Cell_9_12_box:
		EALLOW;
		ECanaShadow.CANMC.bit.MBNR = Mbox;
		ECanaShadow.CANMC.bit.CDR = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		ECanaMboxes.MBOX6.MDL.word.LOW_WORD = Cell_Send(9);
		ECanaMboxes.MBOX6.MDL.word.HI_WORD = Cell_Send(10);
		ECanaMboxes.MBOX6.MDH.word.LOW_WORD = Cell_Send(11);
		ECanaMboxes.MBOX6.MDH.word.HI_WORD = Cell_Send(12);
		ECanaShadow.CANMC.bit.MBNR = 0;
		ECanaShadow.CANMC.bit.CDR = 0;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
		return 1;
	case Cell_13_16_box:
		EALLOW;
		ECanaShadow.CANMC.bit.MBNR = Mbox;
		ECanaShadow.CANMC.bit.CDR = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		ECanaMboxes.MBOX7.MDL.word.LOW_WORD = Cell_Send(13);
		ECanaMboxes.MBOX7.MDL.word.HI_WORD = Cell_Send(14);
		ECanaMboxes.MBOX7.MDH.word.LOW_WORD = Cell_Send(15);
		ECanaMboxes.MBOX7.MDH.word.HI_WORD = Cell_Send(16);
		ECanaShadow.CANMC.bit.MBNR = 0;
		ECanaShadow.CANMC.bit.CDR = 0;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
		return 1;
	case Cell_17_20_box:
		EALLOW;
		ECanaShadow.CANMC.bit.MBNR = Mbox;
		ECanaShadow.CANMC.bit.CDR = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		ECanaMboxes.MBOX8.MDL.word.LOW_WORD = Cell_Send(17);
		ECanaMboxes.MBOX8.MDL.word.HI_WORD = Cell_Send(18);
		ECanaMboxes.MBOX8.MDH.word.LOW_WORD = Cell_Send(19);
		ECanaMboxes.MBOX8.MDH.word.HI_WORD = Cell_Send(20);
		ECanaShadow.CANMC.bit.MBNR = 0;
		ECanaShadow.CANMC.bit.CDR = 0;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
		return 1;
	case Cell_21_24_box:
		EALLOW;
		ECanaShadow.CANMC.bit.MBNR = Mbox;
		ECanaShadow.CANMC.bit.CDR = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		ECanaMboxes.MBOX9.MDL.word.LOW_WORD = Cell_Send(21);
		ECanaMboxes.MBOX9.MDL.word.HI_WORD = Cell_Send(22);
		ECanaMboxes.MBOX9.MDH.word.LOW_WORD = Cell_Send(23);
		ECanaMboxes.MBOX9.MDH.word.HI_WORD = Cell_Send(24);
		ECanaShadow.CANMC.bit.MBNR = 0;
		ECanaShadow.CANMC.bit.CDR = 0;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
		return 1;
	case Cell_25_28_box:
		EALLOW;
		ECanaShadow.CANMC.bit.MBNR = Mbox;
		ECanaShadow.CANMC.bit.CDR = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		ECanaMboxes.MBOX10.MDL.word.LOW_WORD = Cell_Send(25);
		ECanaMboxes.MBOX10.MDL.word.HI_WORD = Cell_Send(26);
		ECanaMboxes.MBOX10.MDH.word.LOW_WORD = Cell_Send(27);
		ECanaMboxes.MBOX10.MDH.word.HI_WORD = Cell_Send(28);
		ECanaShadow.CANMC.bit.MBNR = 0;
		ECanaShadow.CANMC.bit.CDR = 0;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
		return 1;
	case Cell_29_32_box:
		EALLOW;
		ECanaShadow.CANMC.bit.MBNR = Mbox;
		ECanaShadow.CANMC.bit.CDR = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		ECanaMboxes.MBOX11.MDL.word.LOW_WORD = Cell_Send(29);
		ECanaMboxes.MBOX11.MDL.word.HI_WORD = Cell_Send(30);
		ECanaMboxes.MBOX11.MDH.word.LOW_WORD = Cell_Send(31);
		ECanaMboxes.MBOX11.MDH.word.HI_WORD = Cell_Send(32);
		ECanaShadow.CANMC.bit.MBNR = 0;
		ECanaShadow.CANMC.bit.CDR = 0;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
		return 1;
	}
	return 0;
}

void FillSendCAN(unsigned Mbox)
{
	if (FillCAN(Mbox) == 1)
	{
		SendCAN(Mbox);
	}
}

void SendCAN(unsigned int Mbox)
{
	mask = 1 << Mbox;
	ECanaRegs.CANTRS.all = mask;

	//todo Nathan: calibrate sendcan stopwatch
	StopWatchRestart(can_watch);

	do{ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;}
	while(((ECanaShadow.CANTA.all & mask) != mask) && (isStopWatchComplete(can_watch) == 0)); //wait to send or hit stop watch

	ECanaRegs.CANTA.all = mask;						//clear flag
	if (isStopWatchComplete(can_watch) == 1)					//if stopwatch flag
	{
		ops.Flags.bit.can_error = 1;
	}
	else if (ops.Flags.bit.can_error == 1)		//if no stopwatch and flagged reset
	{
		ops.Flags.bit.can_error = 0;
	}
}


void FillCANData()
{
	//todo USER: use FillCAN to put data into correct mailboxes
	FillCAN(Stats1_box);
	FillCAN(Stats2_box);
	FillCAN(Cell_1_4_box);
	FillCAN(Cell_5_8_box);
	FillCAN(Cell_9_12_box);
	FillCAN(Cell_13_16_box);
	FillCAN(Cell_17_20_box);
	FillCAN(Cell_21_24_box);
	FillCAN(Cell_25_28_box);
	FillCAN(Cell_29_32_box);

}

// INT9.6
__interrupt void ECAN1INTA_ISR(void)  // eCAN-A
{
	Uint32 ops_id;
	Uint32 dummy;
  	unsigned int mailbox_nr;
  	ECanaShadow.CANGIF1.bit.MIV1 =  ECanaRegs.CANGIF1.bit.MIV1;
  	mailbox_nr = ECanaShadow.CANGIF1.bit.MIV1;
  	//todo USER: Setup ops command
  	switch (mailbox_nr)
  	{
  	case COMMAND_BOX:
  		ops_id = ECanaMboxes.MBOX0.MDH.all;
  		dummy = ECanaMboxes.MBOX0.MDL.all;
  		switch (ops_id)
  		{
  		case OPS_ID_STATE:
  			memcpy(&ops.State,&dummy,sizeof ops.State);
  			ops.Change.bit.State = 1;
  			break;
  		case OPS_ID_UPDATE_PERIOD:
  			memcpy(&ops.Update_period,&dummy,sizeof ops.Update_period);
  			ops.Change.bit.State = 1;
  			break;
  		}
  		ECanaRegs.CANRMP.bit.RMP0 = 1;
  		break;
  	case BIM2_box:
  		StopWatchRestart(ops.BIM[BIM2].Reset_stopwatch);
  		ops.BIM[BIM2].lowest_cell_volts = ECanaMboxes.MBOX12.MDH.word.LOW_WORD;
  		ECanaRegs.CANRMP.bit.RMP12 = 1;
  		break;
  	case BIM3_box:
  		StopWatchRestart(ops.BIM[BIM3].Reset_stopwatch);
  		ops.BIM[BIM3].lowest_cell_volts = ECanaMboxes.MBOX13.MDH.word.LOW_WORD;
  		ECanaRegs.CANRMP.bit.RMP13 = 1;
  		break;

  	}
  	//todo USER: Setup other reads

  	//To receive more interrupts from this PIE group, acknowledge this interrupt
  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}


signed int Cell_Send(int cell_num)
{
	cell_num--;
	if(cell_num <= 6)
	{
		if (data.bq_pack.bq_devs[0].cell_bal & (1 << cell_num) != 0)
		{
			return 0 - (data.bq_pack.bq_devs[0].cell_voltage[cell_num]);
		}
		else
		{
			return data.bq_pack.bq_devs[0].cell_voltage[cell_num];
		}
	}
	else if(cell_num <= 12)
	{
		if (data.bq_pack.bq_devs[0].cell_bal & (1 << (cell_num-12)) != 0)
		{
			return 0 - (data.bq_pack.bq_devs[0].cell_voltage[cell_num-12]);
		}
		else
		{
			return data.bq_pack.bq_devs[0].cell_voltage[cell_num-12];
		}
	}
	else if(cell_num <= 18)
	{
		if (data.bq_pack.bq_devs[0].cell_bal & (1 << (cell_num-18)) != 0)
		{
			return 0 - (data.bq_pack.bq_devs[0].cell_voltage[cell_num-18]);
		}
		else
		{
			return data.bq_pack.bq_devs[0].cell_voltage[cell_num-18];
		}
	}
	else
	{
		if (data.bq_pack.bq_devs[0].cell_bal & (1 << (cell_num-32)) != 0)
		{
			return 0 - (data.bq_pack.bq_devs[0].cell_voltage[cell_num-32]);
		}
		else
		{
			return data.bq_pack.bq_devs[0].cell_voltage[cell_num-32];
		}
	}
}

