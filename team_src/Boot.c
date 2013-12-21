/*
 * Boot.c
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 *      Updated on Nov. 29, Julia
 *
 */



#include "Boot.h"
#include "Shared_Boot.h"
#include "main.h"
#include "Flash2803x_API_Library.h"

// External Function
extern void Boot(Uint16);
extern void CopyData();
extern void BC_CAN_Boot(Uint16);
extern Uint32 GetLongData(void);

// Internal Functions


Uint16 ConfNbr1 = 0x00CC;
Uint16 ConfNbr2 = 0x00AA;



void Boot(Uint16 MesgID) {
	DINT;
	BootInit(MesgID);
	Restart();
}

void BootInit(Uint16 MesgID) {
	FLASH_ST FlashStatus;
	Uint32 EntryAddr;

	EALLOW;
	Flash_CPUScaleFactor = SCALE_FACTOR;
	Flash_CallbackPtr = 0;
	EDIS;

	BC_CAN_Boot(MesgID);

	Confirm(MesgID,ConfNbr1);

	if (((*GetWordData)()) != 0x08AA)
	{
		Restart();
	}

	Flash_Erase((SECTORB|SECTORC|SECTORD|SECTORE|SECTORF|SECTORG|SECTORH),&FlashStatus);



	Confirm(MesgID,ConfNbr2);

	if (((*GetWordData)()) != 0x08AA)
	{
		Restart();
	}
	ReadReservedFn();
	EntryAddr = GetLongData();
	CopyData();
}

// Handshake com protocol with CANcorder through bus
void Confirm(Uint16 MesgID, Uint16 conf) {
	//struct MBOX MboxS; Having issues typematching MBOXs to mbox signals/messageIDs, etc.
	//struct MBOX MboxR;

	//create 2 mailboxes (R and S) from those already set up (from can.CANsetup())
	// MBOX2 and MBOX3 used for this handshake

	EALLOW;
	ECanaMboxes.MBOX2.MSGCTRL.all = 0;

	ECanaRegs.CANME.bit.ME2 = 0;            //disable to change settings
	ECanaMboxes.MBOX2.MSGID.bit.STDMSGID = MesgID;
	ECanaMboxes.MBOX2.MSGID.bit.AAM = 0;
	ECanaMboxes.MBOX2.MSGID.bit.AME = 0; 	// all bit must match (need this?)
	ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 8;
	ECanaRegs.CANMD.bit.MD2 = 0;            //0 for transmit
	ECanaRegs.CANME.bit.ME2 = 1;            //enable

	EDIS;

	ECanaMboxes.MBOX2.MDH.all = conf;			//set high
	ECanaMboxes.MBOX2.MDL.all = 0;					//set low

	ECanaRegs.CANTRS.bit.TRS2 = 1; 				//send if flagged
	while (ECanaRegs.CANTA.bit.TA2 != 1) {}		//wait to send
	ECanaRegs.CANTA.bit.TA2 = 1;				//clear flog (riblet! riblet!)e
}


//#################################################
// void CAN_Init(void)
//----------------------------------------------
// Initialize the CAN-A port for communications
// with the host.
//----------------------------------------------

void BC_CAN_Boot(Uint16 Node_ID)
{

	   // If the missing clock detect bit is set, just
	   // loop here.
	   if(SysCtrlRegs.PLLSTS.bit.MCLKSTS == 1)
	   {
	      for(;;);
	   }

	   // Asign GetWordData to the CAN-A version of the
	   // function.  GetWordData is a pointer to a function.
	   GetWordData = BC_CAN_GetWordData;

	   BC_CAN_Init(Node_ID);

	   // TODO: Adjust the returning of entry point?
	   // If the KeyValue was invalid, abort the load
	   // and return the flash entry point.
	  // if (BC_CAN_GetWordData() != 0x08AA) return FLASH_ENTRY_POINT;

//	   ReadReservedFn();

//	   EntryAddr = GetLongData();

//	   CopyData(); Moved to after calling confirm()


}

void BC_CAN_Init(Uint16 Node_ID)
{

/* Create a shadow register structure for the CAN control registers. This is
 needed, since only 32-bit access is allowed to these registers. 16-bit access
 to these registers could potentially corrupt the register contents or return
 false data. This is especially true while writing to/reading from a bit
 (or group of bits) among bits 16 - 31 */

// Passed param (Node_ID) is used in line 137 ///

   struct ECAN_REGS ECanaShadow;

   EALLOW;

/* Enable CAN clock  */

   SysCtrlRegs.PCLKCR0.bit.ECANAENCLK=1;

/* Configure eCAN-A pins using GPIO regs*/

   // GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1; // GPIO30 is CANRXA
   // GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1; // GPIO31 is CANTXA
   GpioCtrlRegs.GPAMUX2.all |= 0x50000000;

/* Enable internal pullups for the CAN pins  */

   // GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;
   // GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;
   GpioCtrlRegs.GPAPUD.all &= 0x3FFFFFFF;

/* Asynch Qual */

   GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;

/* Configure eCAN RX and TX pins for CAN operation using eCAN regs*/

    ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;

    ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
    ECanaShadow.CANRIOC.bit.RXFUNC = 1;
    ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;


/* Initialize all bits of 'Message Control Register' to zero */
// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
// all bits (including reserved bits) of MSGCTRL must be initialized to zero

    ECanaMboxes.MBOX1.MSGCTRL.all = 0x0000000;

/* Clear all RMPn, GIFn bits */
// RMPn, GIFn bits are zero upon reset and are cleared again as a precaution.

   ECanaRegs.CANRMP.all = 0xFFFFFFFF;

/* Clear all interrupt flag bits */

   ECanaRegs.CANGIF0.all = 0xFFFFFFFF;
   ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

/* Configure bit timing parameters for eCANA*/

	ECanaShadow.CANMC.all = 0;
	ECanaShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do
	{
	    ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 1 );  		// Wait for CCE bit to be set..

    ECanaShadow.CANBTC.all = 0;

    ECanaShadow.CANBTC.bit.BRPREG = 3;
    ECanaShadow.CANBTC.bit.TSEG1REG = 10;
    ECanaShadow.CANBTC.bit.TSEG2REG = 2;
    ECanaShadow.CANBTC.bit.SJWREG = 1;

    ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do
    {
       ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 0 ); 		// Wait for CCE bit to be  cleared..

/* Disable all Mailboxes  */

   ECanaRegs.CANME.all = 0;     // Required before writing the MSGIDs

/* Assign MSGID to MBOX1 */

//   ECanaMboxes.MBOX1.MSGID.all = 0x00040000;	// Standard ID of 1, Acceptance mask disabled
   ECanaMboxes.MBOX1.MSGID.bit.STDMSGID = Node_ID;	// Team's first test-node address (not for specific node)
   ECanaMboxes.MBOX1.MSGID.bit.AME = 0; //Acceptance mask disabled (See p. 33 of CAN Manual)
   	   /* AME (bit 31) = 0 means the identifier extension bit stored in the mailbox
   	    * determines which messages shall be received.
   	    * The IDE bit of the receive mailbox determines the number of bits to be
   	    * compared. Filtering is not applicable. The MSGIDs must match bit-for-bit
   	    * in order to receive a message
   	    */

/* Configure MBOX1 to be a receive MBOX */

   ECanaRegs.CANMD.all = 0x0002;

/* Enable MBOX1 - this will receive data (including confirmation handshake)*/

   ECanaRegs.CANME.all = 0x0002;

   EDIS;

    return;
}


//#################################################
// Uint16 CAN_GetWordData(void)
//-----------------------------------------------
// This routine fetches two bytes from the CAN-A
// port and puts them together to form a single
// 16-bit value.  It is assumed that the host is
// sending the data in the order LSB followed by MSB.
//-----------------------------------------------


Uint16 BC_CAN_GetWordData()
{
   Uint16 wordData;
   Uint16 byteData;

   wordData = 0x0000;
   byteData = 0x0000;

// Fetch the LSB
   while(ECanaRegs.CANRMP.all == 0) { }
   wordData =  (Uint16) ECanaMboxes.MBOX1.MDL.byte.BYTE0;   // LS byte

   // Fetch the MSB

   byteData =  (Uint16)ECanaMboxes.MBOX1.MDL.byte.BYTE1;    // MS byte

   // form the wordData from the MSB:LSB
   wordData |= (byteData << 8);

/* Clear all RMPn bits */

    ECanaRegs.CANRMP.all = 0xFFFFFFFF;

   return wordData;
}

