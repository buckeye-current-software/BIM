

#include "bq_pack.h"
#include "bq_data_flash.h"



/**
* @brief  Local variables        .                     
*/
//Constant Array that stores the battery pack info on flash memory
const unsigned long DATA_FLASH[] =
{
  /*Cell Voltage*/
  dCOV_THRESHOLD,                  //COV_THRESHOLD           [mV]
  dCOV_RECOVERY_THRESHOLD,         //COV_RECOVERY_THRESHOLD  [mV]
  dCOV_TIME,                       //COV_TIME                [100ms]
  dCUV_THRESHOLD,                  //CUV_THRESHOLD           [mV]
  dCUV_RECOVERY_THRESHOLD,         //CUV_RECOVERY_THRESHOLD  [mV]
  dCUV_TIME,                       //CUV_TIME                [100ms]
  
  /*Temperature*/
  dPACK_OVER_TEMP1, //3 PACK_OVER_TEMP1   [st C]
  dPACK_OT_TIME1,   //PACK_OT_TIME1       [ms]
  dPACK_OVER_TEMP2,  //3 PACK_OVER_TEMP2   [st C]
  dPACK_OT_TIME2,   //PACK_OT_TIME2       [ms]
  
  /*Balancing*/
  dBALANCE_TIME,                     //BALANCE_TIME     [s] max value is 63
  dBALANCE_VOLTS_THRESHOLD,          //BALANCE_VOLTS_THRESHOLD       [mV]
  
};


/**
* @brief Function Name: get_u16_value.                                                 
* @brief Description  : Reads the flash memory and returns the value of the 
* parameter
* @param parameters   : battery pack parameter to be read                                                     
* @return Value       : value of the specific battery parameter                                                    
*/     
unsigned short get_u16_value(param_id_t param_id)
{

  return (unsigned short)DATA_FLASH[param_id];

}

/**
* @brief Function Name: get_u32_value.                                                 
* @brief Description  : Reads the flash memory and returns the value of the 
* parameter
* @param parameters   : battery pack parameter to be read                                                     
* @return Value       : value of the specific battery parameter
*/   
unsigned long get_u32_value(param_id_t param_id)
{
  return (unsigned long)DATA_FLASH[param_id];
}

/*EOF*/
