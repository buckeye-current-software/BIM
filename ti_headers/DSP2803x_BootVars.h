//###########################################################################
//
// FILE:    DSP2803x_BootVars.h
//
// TITLE:   DSP2803x Boot Variable Definitions.
//
// NOTES:
//
//###########################################################################
// $TI Release: F2803x C/C++ Header Files and Peripheral Examples V127 $
// $Release Date: March 30, 2013 $
//###########################################################################

#ifndef DSP2803x_BOOT_VARS_H
#define DSP2803x_BOOT_VARS_H

#ifdef __cplusplus
extern "C" {
#endif



//---------------------------------------------------------------------------
// External Boot ROM variable definitions:
//
extern Uint16 EmuKey;
extern Uint16 EmuBMode;
extern Uint32 Flash_CPUScaleFactor;
extern void (*Flash_CallbackPtr) (void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2803x_BOOT_VARS_H definition

//===========================================================================
// End of file.
//===========================================================================

