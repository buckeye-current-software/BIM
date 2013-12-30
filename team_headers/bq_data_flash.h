
#ifndef DATA_FLASH_H
#define DATA_FLASH_H



//Battery pack definition
#define NUMBER_OF_BQ_DEVICES      	6 //BQ76PL536 devices are connected
#define NUMBER_OF_CELLS           	36//108 //Total number of cells in the system
#define MAX_CELLS_NUMBER_IN_BQ    	6  //MAX number of cells per BQ76PL536 device
#define BALANCE_CELL_MIN			3600 // mV

//Battery pack information and threshold values
#define dCOV_THRESHOLD                 4500  //COV_THRESHOLD           [mV]
#define dCOV_RECOVERY_THRESHOLD        4500  //COV_RECOVERY_THRESHOLD  [mV]
#define dCOV_TIME                      32 //20 //COV_TIME (max value 32) [100ms]
#define dCUV_THRESHOLD                 2000  //CUV_THRESHOLD           [mV]
#define dCUV_RECOVERY_THRESHOLD        2000  //CUV_RECOVERY_THRESHOLD  [mV]
#define dCUV_TIME                      32 //20 //CUV_TIME (max value 32) [100ms]

/*
CONFIG_OT	VTS_SET	VTS_CLEAR	TSET		TCLEAR
0 disable
1			0.2		0.1766		39.55 ºC	34.32 ºC
2			0.2244	0.2			44.89 ºC	39.55 ºC
3			0.2488	0.227		50.19 ºC	45.46 ºC
4			0.2712	0.2498		55.09 ºC	50.41 ºC
5			0.2956	0.275		60.54 ºC	55.93 ºC
6			0.3156	0.2956		65.16 ºC	60.54 ºC
7			0.3356	0.3162		69.97 ºC	65.3 ºC
8			0.3556	0.3368		75.02 ºC	70.26 ºC
9			0.3712	0.3528		79.19 ºC	74.3 ºC
10			0.3866	0.3688		83.54 ºC	78.53 ºC
11			0.4		0.3824		87.56 ºC	82.32 ºC

*/

#define dPACK_OVER_TEMP1               	0    //3 PACK_OVER_TEMP1   [st C]
#define dPACK_OVER_TEMP1_Config			0
#define dPACK_OVER_TEMP1_RATIO_CLEAR   	0 //USE TABLE ABOVE TO SET VALUE
#define dPACK_OVER_TEMP1_RATIO_SET   	0 //USE TABLE ABOVE TO SET VALUE
#define dPACK_OT_TIME1                 	200  //PACK_OT_TIME1       [10ms]
#define dPACK_OVER_TEMP2              	0   //3 PACK_OVER_TEMP1   [st C]
#define dPACK_OVER_TEMP2_Config			0
#define dPACK_OVER_TEMP2_RATIO_CLEAR   	0 //USE TABLE ABOVE TO SET VALUE
#define dPACK_OVER_TEMP2_RATIO_SET   	0 //USE TABLE ABOVE TO SET VALUE
#define dPACK_OT_TIME2                 	200  //PACK_OT_TIME1       [10ms]





#define dBALANCE_TIME                  (unsigned long)10    //BALANCE_TIME A.K.A CB_TIME[s]
#define dBALANCE_VOLTS_THRESHOLD       10    //BALANCE_VOLTS_THRESHOLD      [mV]



/**
* @brief  Global defines         .                     
*/

//definition of the Parameters structure
typedef enum PARAM_ID
{
	  /*Cell Voltage*/
	  COV_THRESHOLD,                  //COV_THRESHOLD           [mV]
	  COV_RECOVERY_THRESHOLD,         //COV_RECOVERY_THRESHOLD  [mV]
	  COV_TIME,                       //COV_TIME                [100ms]
	  CUV_THRESHOLD,                  //CUV_THRESHOLD           [mV]
	  CUV_RECOVERY_THRESHOLD,         //CUV_RECOVERY_THRESHOLD  [mV]
	  CUV_TIME,                       //CUV_TIME                [100ms]

	  /*Temperature*/
	  PACK_OVER_TEMP1, //3 PACK_OVER_TEMP1   [st C]
	  PACK_OT_TIME1,   //PACK_OT_TIME1       [ms]
	  PACK_OVER_TEMP2,  //3 PACK_OVER_TEMP2   [st C]
	  PACK_OT_TIME2,   //PACK_OT_TIME2       [ms]

	  /*Balancing*/
	  BALANCE_TIME,                     //BALANCE_TIME     [s] max value is 63
	  BALANCE_VOLTS_THRESHOLD          //BALANCE_VOLTS_THRESHOLD       [mV]

  
} param_id_t;


/**
* @brief  Global functions declaration         .                     
*/

extern unsigned short get_u16_value(param_id_t param_id);
extern unsigned long get_u32_value(param_id_t param_id);

#endif

/*EOF*/
