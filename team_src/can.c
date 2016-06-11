/*
 * can.c
 *
 *  Created on: Nov 12, 2013
 *      Author: Nathan
 */
#include "all.h"

struct ECAN_REGS ECanaShadow;

/*
 * Function responsible for initializing the CAN module.
 *
 * For simple CAN setup the function CreateCANMailbox can be used to specify user
 * mailboxes.
 */
void CANSetup()
{
	//System specific CAN setup
	SystemCANInit(&ECanaShadow);

	EALLOW;
	HeartbeatBoxInit(HEARTBEAT_ID); // Mbox 1
	CommandBoxInit(COMMAND_ID);   // Mbox 0

	/*
	* todo USER: Node specifc CAN setup
	* create mailbox for all Receive and transmit IDs
	* MBOX2 - MBOX31
    *
	* CreateCANMailbox(int mailboxNum, int IDE, int AME, int AAM, int DLC, int STDMSGID, int Mode)
	*/
	CreateCANMailbox(Stats1_box, 0, 0, 1, 7,Stats1_ID, 0);
	CreateCANMailbox(Stats2_box, 0, 0, 1, 8,Stats2_ID, 0);
	CreateCANMailbox(Cell_1_4_box, 0, 0, 1, 8,Cell_1_4_ID, 0);
	CreateCANMailbox(Cell_5_8_box, 0, 0, 1, 8,Cell_5_8_ID, 0);
	CreateCANMailbox(Cell_9_12_box, 0, 0, 1, 8,Cell_9_12_ID, 0);

	if (bim_cell_num > 12)
	{
		CreateCANMailbox(Cell_13_16_box, 0, 0, 1, 8,Cell_13_16_ID, 0);
		CreateCANMailbox(Cell_17_20_box, 0, 0, 1, 8,Cell_17_20_ID, 0);
		CreateCANMailbox(Cell_21_24_box, 0, 0, 1, 8,Cell_21_24_ID, 0);
	}

	CreateCANMailbox(Temp1_box, 0, 0, 1, 8,Temp1_ID, 0);
	CreateCANMailbox(Temp2_box, 0, 0, 1, 8,Temp2_ID, 0);

	if (bim_dev_num > 2)
	{
		CreateCANMailbox(Temp3_box, 0, 0, 1, 8,Temp3_ID, 0);
		CreateCANMailbox(Temp4_box, 0, 0, 1, 8,Temp4_ID, 0);
	}

    EDIS;
    FinishCANInit();
}

char FillCAN(unsigned int Mbox)
{
	CopyMCToShadow(&ECanaShadow);

	//Try to fill heartbeat. If not heartbeat mailbox, see if it's a user mailbox
	if(FillHeartbeat(Mbox, user_ops.UserFlags.all) != 1)
	{
		//todo USER: setup for all transmit MBOXs
		//InsertCANMessage(int Mbox, unsigned int MDH, unsigned int MDL)
		switch (Mbox)
		{
		case Stats1_box:
			EALLOW;
			ECanaShadow.CANMC.bit.MBNR = Mbox;
			ECanaShadow.CANMC.bit.CDR = 1;
			ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
			if (user_ops.UserFlags.bit.BIM_init == 1)//if init send data
			{
				ECanaMboxes.MBOX2.MDL.word.LOW_WORD = user_data.bq_pack.highest_cell_volts;
				ECanaMboxes.MBOX2.MDL.byte.BYTE1 = user_data.bq_pack.highest_cell_num + bim_cell_offset;
				ECanaMboxes.MBOX2.MDH.word.LOW_WORD = user_data.bq_pack.lowest_cell_volts;
				ECanaMboxes.MBOX2.MDH.byte.BYTE5 = user_data.bq_pack.lowest_cell_num + bim_cell_offset;
			}
			else//if not init send zeros
			{
				ECanaMboxes.MBOX2.MDH.all = 0;
				ECanaMboxes.MBOX2.MDL.all = 0;
			}
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
			if (user_ops.UserFlags.bit.BIM_init == 1)//if init send data
			{
				ECanaMboxes.MBOX3.MDL.word.LOW_WORD = user_data.bq_pack.average;
				ECanaMboxes.MBOX3.MDL.word.HI_WORD = user_data.bq_pack.std_dev;
				ECanaMboxes.MBOX3.MDH.all = user_data.bq_pack.voltage;
			}
			else//if not init send zeros
			{
				ECanaMboxes.MBOX3.MDH.all = 0;
				ECanaMboxes.MBOX3.MDL.all = 0;
			}
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
			if (user_ops.UserFlags.bit.BIM_init == 1)//if init send data
			{
				ECanaMboxes.MBOX4.MDL.word.LOW_WORD = Cell_Send(1);
				ECanaMboxes.MBOX4.MDL.word.HI_WORD = Cell_Send(2);
				ECanaMboxes.MBOX4.MDH.word.LOW_WORD = Cell_Send(3);
				ECanaMboxes.MBOX4.MDH.word.HI_WORD = Cell_Send(4);
			}
			else//if not init send zeros
			{
				ECanaMboxes.MBOX4.MDH.all = 0;
				ECanaMboxes.MBOX4.MDL.all = 0;
			}
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
			if (user_ops.UserFlags.bit.BIM_init == 1)//if init send data
			{
#if bim_num == 6
				ECanaMboxes.MBOX5.MDL.word.LOW_WORD = Cell_Send7);
				ECanaMboxes.MBOX5.MDL.word.HI_WORD = Cell_Send(8);
				ECanaMboxes.MBOX5.MDH.word.LOW_WORD = Cell_Send(9);
				ECanaMboxes.MBOX5.MDH.word.HI_WORD = Cell_Send(10);
#else
				ECanaMboxes.MBOX5.MDL.word.LOW_WORD = Cell_Send(5);
				ECanaMboxes.MBOX5.MDL.word.HI_WORD = Cell_Send(6);
				ECanaMboxes.MBOX5.MDH.word.LOW_WORD = Cell_Send(7);
				ECanaMboxes.MBOX5.MDH.word.HI_WORD = Cell_Send(8);
#endif
			}
			else//if not init send zeros
			{
				ECanaMboxes.MBOX5.MDH.all = 0;
				ECanaMboxes.MBOX5.MDL.all = 0;
			}
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
			if (user_ops.UserFlags.bit.BIM_init == 1)//if init send data
			{
#if bim_num == 6
				ECanaMboxes.MBOX6.MDL.word.LOW_WORD = Cell_Send(13);
				ECanaMboxes.MBOX6.MDL.word.HI_WORD = Cell_Send(14);
				ECanaMboxes.MBOX6.MDH.word.LOW_WORD = Cell_Send(15);
				ECanaMboxes.MBOX6.MDH.word.HI_WORD = Cell_Send(16);

#else
				ECanaMboxes.MBOX6.MDL.word.LOW_WORD = Cell_Send(9);
				ECanaMboxes.MBOX6.MDL.word.HI_WORD = Cell_Send(10);
				ECanaMboxes.MBOX6.MDH.word.LOW_WORD = Cell_Send(11);
				ECanaMboxes.MBOX6.MDH.word.HI_WORD = Cell_Send(12);
#endif
			}
			else//if not init send zeros
			{
				ECanaMboxes.MBOX6.MDH.all = 0;
				ECanaMboxes.MBOX6.MDL.all = 0;
			}
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
			if (user_ops.UserFlags.bit.BIM_init == 1)//if init send data
			{
#if bim_num == 6
				ECanaMboxes.MBOX7.MDL.word.LOW_WORD = Cell_Send(19);
				ECanaMboxes.MBOX7.MDL.word.HI_WORD = Cell_Send(20);
				ECanaMboxes.MBOX7.MDH.word.LOW_WORD = Cell_Send(21);
				ECanaMboxes.MBOX7.MDH.word.HI_WORD = Cell_Send(22);
#else
				ECanaMboxes.MBOX7.MDL.word.LOW_WORD = Cell_Send(13);
				ECanaMboxes.MBOX7.MDL.word.HI_WORD = Cell_Send(14);
				ECanaMboxes.MBOX7.MDH.word.LOW_WORD = Cell_Send(15);
				ECanaMboxes.MBOX7.MDH.word.HI_WORD = Cell_Send(16);
#endif
			}
			else//if not init send zeros
			{
				ECanaMboxes.MBOX7.MDH.all = 0;
				ECanaMboxes.MBOX7.MDL.all = 0;
			}
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
			if (user_ops.UserFlags.bit.BIM_init == 1)//if init send data
			{
				ECanaMboxes.MBOX8.MDL.word.LOW_WORD = Cell_Send(17);
				ECanaMboxes.MBOX8.MDL.word.HI_WORD = Cell_Send(18);
				ECanaMboxes.MBOX8.MDH.word.LOW_WORD = Cell_Send(19);
				ECanaMboxes.MBOX8.MDH.word.HI_WORD = Cell_Send(20);
			}
			else//if not init send zeros
			{
				ECanaMboxes.MBOX8.MDH.all = 0;
				ECanaMboxes.MBOX8.MDL.all = 0;
			}
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
			if (user_ops.UserFlags.bit.BIM_init == 1)//if init send data
			{
				ECanaMboxes.MBOX9.MDL.word.LOW_WORD = Cell_Send(21);
				ECanaMboxes.MBOX9.MDL.word.HI_WORD = Cell_Send(22);
				ECanaMboxes.MBOX9.MDH.word.LOW_WORD = Cell_Send(23);
				ECanaMboxes.MBOX9.MDH.word.HI_WORD = Cell_Send(24);
			}
			else//if not init send zeros
			{
				ECanaMboxes.MBOX9.MDH.all = 0;
				ECanaMboxes.MBOX9.MDL.all = 0;
			}
			ECanaShadow.CANMC.bit.MBNR = 0;
			ECanaShadow.CANMC.bit.CDR = 0;
			ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
			EDIS;
			return 1;
		case Temp1_box:
			EALLOW;
			ECanaShadow.CANMC.bit.MBNR = Mbox;
			ECanaShadow.CANMC.bit.CDR = 1;
			ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
			if (user_ops.UserFlags.bit.BIM_init == 1)//if init send data
			{
				ECanaMboxes.MBOX14.MDL.all= user_data.bq_pack.bq_devs[0].temperature1.U32;
				ECanaMboxes.MBOX14.MDH.all = user_data.bq_pack.bq_devs[0].temperature2.U32;
			}
			else//if not init send zeros
			{
				ECanaMboxes.MBOX14.MDL.all = 0;
				ECanaMboxes.MBOX14.MDH.all = 0;
			}
			ECanaShadow.CANMC.bit.MBNR = 0;
			ECanaShadow.CANMC.bit.CDR = 0;
			ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
			EDIS;
			return 1;
		case Temp2_box:
			EALLOW;
			ECanaShadow.CANMC.bit.MBNR = Mbox;
			ECanaShadow.CANMC.bit.CDR = 1;
			ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
			if (user_ops.UserFlags.bit.BIM_init == 1)//if init send data
			{
				ECanaMboxes.MBOX15.MDH.all  = user_data.bq_pack.bq_devs[1].temperature1.U32;
				ECanaMboxes.MBOX15.MDL.all  = user_data.bq_pack.bq_devs[1].temperature2.U32;
			}
			else//if not init send zeros
			{
				ECanaMboxes.MBOX15.MDH.all = 0;
				ECanaMboxes.MBOX15.MDL.all = 0;
			}
			ECanaShadow.CANMC.bit.MBNR = 0;
			ECanaShadow.CANMC.bit.CDR = 0;
			ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
			EDIS;
			return 1;
		case Temp3_box:
			EALLOW;
			ECanaShadow.CANMC.bit.MBNR = Mbox;
			ECanaShadow.CANMC.bit.CDR = 1;
			ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
			if (user_ops.UserFlags.bit.BIM_init == 1)//if init send data
			{
				ECanaMboxes.MBOX16.MDH.all  = user_data.bq_pack.bq_devs[2].temperature1.U32;
				ECanaMboxes.MBOX16.MDL.all  = user_data.bq_pack.bq_devs[2].temperature2.U32;
			}
			else//if not init send zeros
			{
				ECanaMboxes.MBOX16.MDH.all = 0;
				ECanaMboxes.MBOX16.MDL.all = 0;
			}
			ECanaShadow.CANMC.bit.MBNR = 0;
			ECanaShadow.CANMC.bit.CDR = 0;
			ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
			EDIS;
			return 1;
		case Temp4_box:
			EALLOW;
			ECanaShadow.CANMC.bit.MBNR = Mbox;
			ECanaShadow.CANMC.bit.CDR = 1;
			ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
			if (user_ops.UserFlags.bit.BIM_init == 1)//if init send data
			{
				ECanaMboxes.MBOX17.MDH.all  = user_data.bq_pack.bq_devs[3].temperature1.U32;
				ECanaMboxes.MBOX17.MDL.all  = user_data.bq_pack.bq_devs[3].temperature2.U32;
			}
			else//if not init send zeros
			{
				ECanaMboxes.MBOX17.MDH.all = 0;
				ECanaMboxes.MBOX17.MDL.all = 0;
			}
			ECanaShadow.CANMC.bit.MBNR = 0;
			ECanaShadow.CANMC.bit.CDR = 0;
			ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
			EDIS;
			return 1;
		default:
			return 0;
		}
	}
	else
	{
		return 1;
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
	if (bim_cell_num > 12)
	{
		FillCAN(Cell_13_16_box);
		FillCAN(Cell_17_20_box);
		FillCAN(Cell_21_24_box);
	}

	FillCAN(Temp1_box);
	FillCAN(Temp2_box);

	if (bim_dev_num > 2)
	{
		FillCAN(Temp3_box);
		FillCAN(Temp4_box);
	}
}

unsigned int Cell_Send(int cell_num)
{
	cell_num--;
	int dev = cell_num / 6;
	cell_num = cell_num % 6;

	if (dev < NUMBER_OF_BQ_DEVICES)
	{

		return user_data.bq_pack.bq_devs[dev].cell_voltage[cell_num];

	}
	return 0;
}

// INT9.6
__interrupt void ECAN1INTA_ISR(void)  // eCAN-A
{
	Uint32 ops_id;
	Uint32 dummy;
  	unsigned int mailbox_nr;
  	mailbox_nr = getMailboxNR();
  	//todo USER: Setup ops command
  	if(mailbox_nr == COMMAND_BOX)
  	{
  		ReadCommand();
  	}
  	//todo USER: Setup other reads

  	//To receive more interrupts from this PIE group, acknowledge this interrupt
  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// INT9.5
__interrupt void ECAN0INTA_ISR(void)   // eCAN-A
{

	// To receive more interrupts from this PIE group, acknowledge this interrupt
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
