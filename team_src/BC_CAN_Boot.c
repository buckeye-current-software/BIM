// Buckeye Current Version 11/23/13
// Updated Nov. 29, Julia
//
// TITLE: Our Special 2803x CAN Boot mode routines
//
// Functions:
//
//   X Uint32 CAN_Boot(void) <--Omitted
//     void CAN_Init(void)
//     Uint32 CAN_GetWordData(void)
//
// Notes:
// BRP = 1, Bit time = 15. This would yield the following bit rates with the
// default PLL setting:
// OSCCLK = 10 MHz  SYSCLKOUT = 10 MHz  CAN module clock = 5 MHz Bit rate = 100kbps


#include "Boot.h"

// All are internal functions
void BC_CAN_Init(Uint16);
Uint16 BC_CAN_GetWordData(void);

// All are external functions
extern void CopyData(void);
extern void ReadReservedFn(void);


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

	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do
	{
	    ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 1 );  		// Wait for CCE bit to be set..

    ECanaShadow.CANBTC.all = 0;

	ECanaShadow.CANBTC.bit.BRPREG = 1;
	ECanaShadow.CANBTC.bit.TSEG2REG = 7;
	ECanaShadow.CANBTC.bit.TSEG1REG = 15;

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

