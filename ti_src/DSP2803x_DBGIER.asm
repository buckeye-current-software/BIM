;// TI File $Revision: /main/1 $
;// Checkin $Date: December 5, 2008   18:00:49 $
;//###########################################################################
;//
;// FILE:  DSP2803x_DBGIER.asm
;//
;// TITLE: Set the DBGIER register
;//
;// DESCRIPTION:
;//  
;//  Function to set the DBGIER register (for realtime emulation).
;//  Function Prototype: void SetDBGIER(Uint16)
;//  Useage: SetDBGIER(value);
;//  Input Parameters: Uint16 value = value to put in DBGIER register. 
;//  Return Value: none          
;//
;//###########################################################################
;// $TI Release: F2803x C/C++ Header Files and Peripheral Examples V127 $
;// $Release Date: March 30, 2013 $
;//###########################################################################	
		.global _SetDBGIER
		.text
		
_SetDBGIER:
		MOV 	*SP++,AL
		POP 	DBGIER
		LRETR
		
