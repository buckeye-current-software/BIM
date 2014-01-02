

#ifndef BQ_PACK_H
#define BQ_PACK_H

#include "bq_data_flash.h"

/**
* @brief  BQ pack signals defines         .                     
*/

///SW flag that controls the charge/discharge mode
#define IN_HOST_CHG_OP     BIT0


//ADC_ON, TS2&TS1, GPAI_ON, Cells 1-2-3.....
#define ADC_CONTROL_VAL_6 ((1<<6)|(3<<4)|(1<<3)|(5<<0))
#define ADC_CONTROL_VAL_5 ((1<<6)|(3<<4)|(1<<3)|(4<<0))
#define ADC_CONTROL_VAL_4 ((1<<6)|(3<<4)|(1<<3)|(3<<0))

//AUX,GPIO_OUT,GPIO_IN,SLEEP,TS2_on,TS1_on, LED_ON
#define IO_CONTROL_VAL    ((0<<7)|(1<<6)|(0<<5)|(0<<2)|(1<<1)|(1<<0))

//cell ballancing defines
#define CB_CTRL_VAL       0
#define CB_TIME_VAL       0

//CONV START
#define ADC_CONVERT_VAL   (1<<0)

//Enables access to Group 3 protected registers
#define SHDW_CTRL_ACCESS_EN_VAL     0x35  
//Preloads Croup 3 registers from data in EPROM
#define SHDW_CTRL_REFRESH_EN_VAL    0x27  
//Enables access to USER block of EPROM
#define E_EN_OTP_EN_VAL             0x91 
 
//Conv time ~6us,GPAI_REF,GPAI_SRC_connected to batt,Series Cells, WTD
#define FUNC_CONFIG_VAL_6 ((0x1<<6)|(0<<5)|(1<<4)|(0x0<<2)|(0<<0))
#define FUNC_CONFIG_VAL_5 ((0x1<<6)|(0<<5)|(1<<4)|(0x1<<2)|(0<<0))
#define FUNC_CONFIG_VAL_4 ((0x1<<6)|(0<<5)|(1<<4)|(0x2<<2)|(0<<0))

//*CRC_DIS => CRC expected*/
#define IO_CONFIG_VAL     (0<<0)

#define DISCARD_CRC               (1)
#define RETURN_CRC                (0)

//Over voltage definitions
//Example: (30<<0)/*COV Threshold = 2.0V + 30*50mV = 3.5V*/)
#define CONFIG_COV_VAL    (0<<7)
//Example: (20<<0)/*COV Time delay = 20*100ms = 2000ms*/) 
#define CONFIG_COVT_VAL   (1<<7)/*ms*/        

//Under voltage definitions
//Example: (0<<0)/*CUV Threshold = 2.0V + 0*50mV = 2V*/)
#define CONFIG_CUV_VAL    (0<<7)
//Example: (20<<0)/*CUV Time delay = 20*100ms = 2000ms*/)
#define CONFIG_CUVT_VAL   (1<<7)/*ms*/ 
       
//Over temperature definitions
//Example: (3<<0)/*TS1 = 50 stC*/)
#define CONFIG_OT_VAL     (0<<0)/*TS2*/
#define CONFIG_OTT_VAL    (0<<0)/*Over Temp Time delay = 200*10ms = 2000ms*/

//Cell Voltage [mV] = (REGMSB * 256 + REGLSB) * 6250 / 16383
#define adc_step_mul 6250
#define adc_step_div 16383

//*BQ Pack special addresses*/
#define BROADCAST_ADDR            0x3F
#define DISCOVERY_ADDR            0x00
#define BQ76PL536_RESET           0xa5

//Definition of the BQ76PL536 registers
typedef enum BQ_DEV_REGS
{
  DEVICE_STATUS_REG=0x00,
  GPAI_L_REG=0x01, 
  GPAI_H_REG=0x02,
  VCELL1_L_REG=0x03, 
  VCELL1_H_REG=0x04,
  VCELL2_L_REG=0x05, 
  VCELL2_H_REG=0x06,
  VCELL3_L_REG=0x07, 
  VCELL3_H_REG=0x08,
  VCELL4_L_REG=0x09, 
  VCELL4_H_REG=0x0a,
  VCELL5_L_REG=0x0b, 
  VCELL5_H_REG=0x0c,
  VCELL6_L_REG=0x0d, 
  VCELL6_H_REG=0x0e,
  TEMPERATURE1_L_REG=0x0f, 
  TEMPERATURE1_H_REG=0x10,
  TEMPERATURE2_L_REG=0x11, 
  TEMPERATURE2_H_REG=0x12,
  ALERT_STATUS_REG=0x20,
  FAULT_STATUS_REG=0x21,
  COV_FAULT_REG=0x22,
  CUV_FAULT_REG=0x23,
  PRESULT_A_REG=0x24,
  PRESULT_B_REG=0x25,
  ADC_CONTROL_REG=0x30,
  IO_CONTROL_REG=0x31,
  CB_CTRL_REG=0x32,
  CB_TIME_REG=0x33,
  ADC_CONVERT_REG=0x34,
  SHDW_CTRL_REG=0x3a,
  ADDRESS_CONTROL_REG=0x3b,
  RESET_REG=0x3c,
  TEST_SELECT_REG=0x3d,
  E_EN_REG=0x3F,
  FUNCTION_CONFIG_REG=0x40,
  IO_CONFIG_REG=0x41,
  CONFIG_COV_REG=0x42,
  CONFIG_COVT_REG=0x43,
  CONFIG_CUV_REG=0x44,
  CONFIG_CUVT_REG=0x45,
  CONFIG_OT_REG=0x46,
  CONFIG_OTT_REG=0x47,
  USER1_REG=0x48,
  USER2_REG=0x49,
  USER3_REG=0x4A,
  USER4_REG=0x4B,
  BQ_SPI_REG_MAX=0x4F
} bq_dev_regs_t;

//Definition of the structure that stores the cell information
typedef struct CELL_DATA
{
  unsigned short voltage;
} cell_data_t;

//Definition of the structure that stores the BQ76PL536 information
typedef struct BQ_DEV
{
  unsigned short cell_count;
  unsigned char device_address;
  
  unsigned char device_status;

  unsigned short cell_voltage[MAX_CELLS_NUMBER_IN_BQ];  //[mV]
  
  unsigned short temperature1;
  unsigned short temperature2;
  float temperature1ratio;
  float temperature2ratio;

  unsigned char alert_status;
  unsigned char fault_status;
  unsigned char cov_fault;
  unsigned char cuv_fault;

  unsigned short num_cell_bal;
  unsigned short cell_bal;

  unsigned short crc_error_count;


} bq_dev_t;

//bit position of the COV,CUV,OT1 and OT2 flags
#define FAULT_COV_POS         0
#define FAULT_CUV_POS         1
#define ALERT_OT1_POS         0
#define ALERT_OT2_POS         1

//Define the battery pack operating modes



typedef struct BQ_PACK
{
  bq_dev_t bq_devs[NUMBER_OF_BQ_DEVICES];

  unsigned short error_status;
  long voltage;
  long std_dev;
  unsigned short average;
  unsigned short lowest_cell_volts;
  unsigned short lowest_cell_num;
  unsigned short highest_cell_volts;
  unsigned short highest_cell_num;
  unsigned short highest_crc;
  unsigned short highest_crc_num;
  unsigned short num_balance_cells;
  char bal_num;

} bq_pack_t;

//redefines the SPI read/write functions
#define bq_dev_write_reg(a,b,c) spi_write_reg(a,(unsigned char)b,c)
#define bq_dev_read_reg(a,b,c,d,e) spi_read_reg(a,(unsigned char)b,c,d,e)


/******************************************************************************/
/*                            Global Variables                                */
/******************************************************************************/


/******************************************************************************/
/*                       Global functions declaration                          */
/******************************************************************************/
extern void BMM_Sleep();
extern void BMM_Wake();
extern short bq_pack_address_discovery(void);
extern short bq_pack_init(void);
extern unsigned char update_bq_pack_data(void);
extern void BatteryPackManager(void);
extern unsigned char bq_pack_start_conv(void);
extern unsigned short get_bq_pack_voltage(void);
extern unsigned short get_bq_pack_timer(void);
extern void update_bq_pack_timer(void);
extern void CheckFaultConditions(void);
extern unsigned short find_imbalanced_cell(unsigned short in_dev_id,unsigned short low_cell);




 
/******************************************************************************/
/*                            Local functions                                 */
/******************************************************************************/
unsigned char conf_bq_dev(bq_dev_t* this, int cell_num);
unsigned char init_bq_dev_data_structure(bq_dev_t* this);
short bq_dev_read_cell_voltage(bq_dev_t* this);
short bq_dev_read_temps(bq_dev_t* this);
short bq_dev_read_errors(bq_dev_t* this);
unsigned char bq_dev_read_status(bq_dev_t* this);
unsigned char bq_dev_clear_alerts(bq_dev_t* this);
unsigned char bq_dev_clear_faults(bq_dev_t* this);
void copy_cell_voltage_values(void);
unsigned short check_for_discharge_op(void);
unsigned short check_for_charge_op(void);
void CellBalancing(void);
void CheckChargeDischargeModes(void);
void CheckEndOfChargeOrDischargeModes(void);
unsigned char enable_bypass_resistor(unsigned short in_dev_id, unsigned short in_value);
unsigned char disable_bypass_resistor(unsigned short in_dev_id, unsigned short in_value);
unsigned char disable_all_bypass_resistors(void);
unsigned short cell_imbalance_threshold_reached(void);
short check_for_timeout(void);

short bq_dev_read_gpai(bq_dev_t* this);
unsigned char bq_dev_set_gpio(bq_dev_t* this);
unsigned char bq_dev_clear_gpio(bq_dev_t* this);
unsigned char bq_dev_toggle_gpio(bq_dev_t* this);
unsigned char bq_dev_sleep(bq_dev_t* this);
unsigned char bq_dev_wake(bq_dev_t* this);



#endif

/*EOF*/
