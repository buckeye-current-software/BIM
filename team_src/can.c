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
	//MBOX 0 - 1
	CommandBoxInit(COMMAND_ID);   // Mbox 0
	HeartbeatBoxInit(HEARTBEAT_ID); // Mbox 1
	/*
	* todo USER: Node specifc CAN setup
	* create mailbox for all Receive and transmit IDs
	* MBOX2 - MBOX31
    *
	* CreateCANMailbox(int mailboxNum, int IDE, int AME, int AAM, int DLC, int STDMSGID, int Mode)
	*/

	//adc TRANSMIT
	//CreateCANMailbox(2,0,0,1,8,ADC_ID,0);
	//gp_button TRANSMIT
	//CreateCANMailbox(3,0,0,1,8,GP_BUTTON_ID,0);

	CreateCANMailbox(COOLANT_FLOW_BOX,0,0,1,4,COOLANT_FLOW_ID,0); //CHECK AAM
	CreateCANMailbox(MOTOR_TEMP_BOX,0,0,1,4,MOTOR_TEMP_ID,0);
	CreateCANMailbox(MOTOR_CONT_TEMP_BOX,0,0,1,4,MOTOR_CONT_TEMP_ID, 0);
	CreateCANMailbox(RADIATOR_TEMP_BOX,0,0,1,4,RADIATOR_TEMP_ID,0);
	CreateCANMailbox(COOLANT_PRESSURES_BOX,0,0,1,8,COOLANT_PRESSURES_ID,0);
	CreateCANMailbox(EMRAX_TEMP_BOX,0,0,1,4,EMRAX_TEMP_ID,0);
	CreateCANMailbox(AMBIENT_TEMP_BOX,0,0,1,4,AMBIENT_TEMP_ID,0);
	CreateCANMailbox(MOTOR_PLATE_TEMP_BOX,0,0,1,8,MOTOR_PLATE_TEMP_ID,0);

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
		case COOLANT_FLOW_BOX:
			InsertCANMessage(COOLANT_FLOW_BOX, 0, user_data.coolant_flow.U32);
			return 1;
		case MOTOR_TEMP_BOX:
			InsertCANMessage(MOTOR_TEMP_BOX, user_data.motor_coolant_temp.U32, user_data.motor_coolant_temp.U32);
			return 1;
		case MOTOR_CONT_TEMP_BOX:
			InsertCANMessage(MOTOR_CONT_TEMP_BOX, user_data.motor_control_coolant_temp.U32, user_data.motor_control_coolant_temp.U32);
			return 1;
		case RADIATOR_TEMP_BOX:
			InsertCANMessage(RADIATOR_TEMP_BOX, user_data.radiator_coolant_temp.U32, user_data.radiator_coolant_temp.U32);
			return 1;
		case COOLANT_PRESSURES_BOX:
			InsertCANMessage(COOLANT_PRESSURES_BOX, user_data.coolant_pressure_2.U32, user_data.coolant_pressure_1.U32);
			return 1;
		case EMRAX_TEMP_BOX:
			InsertCANMessage(EMRAX_TEMP_BOX, 0, user_data.motor_temp.U32);
			return 1;
		case AMBIENT_TEMP_BOX:
			InsertCANMessage(AMBIENT_TEMP_BOX, 0, user_data.ambient_temp.U32);
			return 1;
		case MOTOR_PLATE_TEMP_BOX:
			InsertCANMessage(MOTOR_PLATE_TEMP_BOX, user_data.motor_plate_temp_2.U32, user_data.motor_plate_temp_1.U32);
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
	FillCAN(COOLANT_FLOW_BOX);
	FillCAN(MOTOR_TEMP_BOX);
	FillCAN(MOTOR_CONT_TEMP_BOX);
	FillCAN(RADIATOR_TEMP_BOX);
	FillCAN(COOLANT_PRESSURES_BOX);
	FillCAN(EMRAX_TEMP_BOX);
	FillCAN(AMBIENT_TEMP_BOX);
	FillCAN(MOTOR_PLATE_TEMP_BOX);
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
