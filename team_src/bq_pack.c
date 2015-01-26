
#include "all.h"
  
extern user_data_struct data_temp;
extern user_ops_struct ops_temp;

/******************************************************************************/
/*                            Local variables                                 */
/******************************************************************************/


void BIM_LED_Set()
{
	  bq_dev_write_reg(BROADCAST_ADDR, IO_CONTROL_REG, IO_CONTROL_VAL);
}

void BIM_LED_Clear()
{
	bq_dev_write_reg(BROADCAST_ADDR, IO_CONTROL_REG, IO_CONTROL_VAL_CLEAR);
}

void BMM_Sleep()
{
	int i;
	 for (i=NUMBER_OF_BQ_DEVICES-1; i>=0; i--)			//put devices to sleep backwards
	 {
		 bq_dev_sleep(&data_temp.bq_pack.bq_devs[i]);
	 }
}

void BMM_Wake()
{
	int i;
	for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)				//wake them up forwards
	{
		bq_dev_wake(&data_temp.bq_pack.bq_devs[i]);
	}
}


short bq_pack_address_discovery(void)
{
  unsigned short i, n;
  unsigned char reg_val[2];

  ops_temp.UserFlags.bit.BQ_error = 0x3f; // all bqs are errors
  i=NUMBER_OF_BQ_DEVICES; //controls iteration loop
  while (i>0)
  {
    //*Send BROADCAST_RESET to address 0x00*/
    if(bq_dev_write_reg(BROADCAST_ADDR, RESET_REG, BQ76PL536_RESET) == INVALID)
    {
    	return INVALID;
    }

    DELAY_US(1000);

    if(bq_dev_write_reg(BROADCAST_ADDR, RESET_REG, BQ76PL536_RESET) == INVALID)
    {
    	return INVALID;
    }

    DELAY_US(1000);

    if(bq_dev_write_reg(BROADCAST_ADDR, RESET_REG, BQ76PL536_RESET) == INVALID)
    {
    	return INVALID;
    }
    DELAY_US(1000);
    n=0;  //controls number of discovered devices
    while (n<NUMBER_OF_BQ_DEVICES)
    {
    //*Read DEVICE_STATUS reg at address 0x00*/
    if (bq_dev_read_reg(DISCOVERY_ADDR, DEVICE_STATUS_REG, 1, DISCARD_CRC, reg_val) == INVALID)
    {
    	return INVALID;
    }
  
      //*Verify if MSB is equal to 0*/
      if (reg_val[0] & (1<<7))
      {
        n = NUMBER_OF_BQ_DEVICES; //break internal loop
      }
      else
      {
        //*Assign a new address*/        

        //Save assigned address
        n++;
        data_temp.bq_pack.bq_devs[n-1].device_address = n;
        
        //ADDR_CTRL = n;   
        if(bq_dev_write_reg(DISCOVERY_ADDR, ADDRESS_CONTROL_REG, n) == INVALID)
        {
        	return INVALID;
        }
        
        //read ADDR_CTRL
        if (bq_dev_read_reg(n, ADDRESS_CONTROL_REG, 1, DISCARD_CRC, reg_val) == INVALID)
        {
        	return INVALID;
		}

        if ((reg_val[0]&0x3F) == n)
        {
          //address next device or finish device detection
        	ops_temp.UserFlags.bit.BQ_error &= ~(1 << (n-1)); // correctly addressed clear error flag
          if (n==NUMBER_OF_BQ_DEVICES)
            return n;
        }
        else
        {
          //break internal loop
          n = NUMBER_OF_BQ_DEVICES;
        }
      }
    }

    i--;
  }
  
  return INVALID;
}

/**
* @brief Function Name: bq_pack_init.                                              
* @brief Description  : Configure each BQ device in the stack and initialize 
* data structures for whole BQ stack .
* @param parameters   :  none                                                  
* @return Value       :  return  if there isn't problems while init the pack                                                   
*/
short bq_pack_init(void)
{
	unsigned char i;

	for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
	{
		//*Init cell count for each BQ device*/
		data_temp.bq_pack.bq_devs[i].cell_count = MAX_CELLS_NUMBER_IN_BQ;

		//*Configure each BQ device*/
		if(conf_bq_dev(&data_temp.bq_pack.bq_devs[i],data_temp.bq_pack.bq_devs[i].cell_count) == INVALID)
		{
			return INVALID;
		}

		//*Initilize data structures*/
		if(init_bq_dev_data_structure(&data_temp.bq_pack.bq_devs[i]) ==  INVALID)
		{
			return INVALID;
		}

		//*Read cell voltage*/
		if(bq_dev_read_cell_voltage(&data_temp.bq_pack.bq_devs[i]) == INVALID)
		{
			return INVALID;
		}
	}
	update_bq_pack_data();
	return VALID;
}


/**
* @brief Function Name: update_bq_pack_data                                                  
* @brief Description  : Reads data from BQ DEV registers and updates BQ PACK 
* structure 
* @param parameters   : none                                                    
* @return Value       : none                                                    
*/     
unsigned char update_bq_pack_data(void)
{
  unsigned char i, cell_cnt;
  long stack_voltage = 0;
  double std_dev = 0;
  unsigned short high_cell;
  unsigned short low_cell;
  unsigned short high_cell_num = 0;
  unsigned short low_cell_num = 0;
  unsigned short total_cells = 1;
  unsigned short highest_crc = 0;
  unsigned short highest_crc_num = 0;
  //read the pack data
  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
  {

	  bq_dev_read_status(&data_temp.bq_pack.bq_devs[i]);

	  //*Read each cell voltage and calculate BQ Pack voltage*/
	  bq_dev_read_cell_voltage(&data_temp.bq_pack.bq_devs[i]);

	  //Identify the lowest and highest voltage in the pack
	  if (i == 0)
	  {
		  low_cell = data_temp.bq_pack.bq_devs[i].cell_voltage[0];
		  high_cell = data_temp.bq_pack.bq_devs[i].cell_voltage[0];
		  highest_crc =  data_temp.bq_pack.bq_devs[i].crc_error_count;
		  std_dev += (data_temp.bq_pack.bq_devs[i].cell_voltage[0] - data_temp.bq_pack.average)*(data_temp.bq_pack.bq_devs[i].cell_voltage[0] - data_temp.bq_pack.average);
		  highest_crc_num = 0;
	  }
	  if (data_temp.bq_pack.bq_devs[i].crc_error_count > highest_crc)
	  {
		  highest_crc = data_temp.bq_pack.bq_devs[i].crc_error_count;
		  highest_crc_num = i;
	  }
	  //calculate the pack voltage
	  for (cell_cnt=0; cell_cnt<data_temp.bq_pack.bq_devs[i].cell_count; cell_cnt++)
	  {
		  stack_voltage += data_temp.bq_pack.bq_devs[i].cell_voltage[cell_cnt];

		  if (data_temp.bq_pack.bq_devs[i].cell_voltage[cell_cnt] < low_cell)
		  {
			  low_cell = data_temp.bq_pack.bq_devs[i].cell_voltage[cell_cnt];
			  low_cell_num = total_cells;
		  }

		  if (data_temp.bq_pack.bq_devs[i].cell_voltage[cell_cnt] > high_cell)
		  {
			  high_cell = data_temp.bq_pack.bq_devs[i].cell_voltage[cell_cnt];
			  high_cell_num = total_cells;
		  }
		  total_cells++;
	  }

	  //*Read each device errors*/
	  bq_dev_read_errors(&data_temp.bq_pack.bq_devs[i]);

	  //*Read each device temperature*/
	  bq_dev_read_temps(&data_temp.bq_pack.bq_devs[i]);


	  total_cells++;

  }
  
  //*Save BQ Pack voltage*/
  data_temp.bq_pack.voltage = stack_voltage;
  data_temp.bq_pack.average = stack_voltage/(NUMBER_OF_CELLS);
  data_temp.bq_pack.std_dev = sqrt(std_dev);
  data_temp.bq_pack.lowest_cell_volts = low_cell;
  data_temp.bq_pack.highest_cell_volts = high_cell;
  data_temp.bq_pack.lowest_cell_num = low_cell_num;
  data_temp.bq_pack.highest_cell_num = high_cell_num;
  data_temp.bq_pack.highest_crc = highest_crc;
  data_temp.bq_pack.highest_crc_num = highest_crc_num;
  return VALID;
}

/**
* @brief Function Name: bq_pack_start_conv   .                                               
* @brief Description  : triggers the ADC on the BQ devices.
* @param parameters   : none                                                    
* @return Value       : none                                                    
*/     
unsigned char bq_pack_start_conv(void)
{
  //Should address all devices in the stack
  if(bq_dev_write_reg(BROADCAST_ADDR, ADC_CONVERT_REG, ADC_CONVERT_VAL) == INVALID)
  {
  	return INVALID;
  }
  return VALID;
}







/******************************************************************************/
/*                            Local functions                                 */
/******************************************************************************/
/**
* @brief Function Name:  conf_bq_dev                  .                              
* @brief Description  :  configures each BQ76PL536 included in the stack.
* @param parameters   :  number of BQ device in the stack                                                   
* @return Value       :  none                                                   
*/     
unsigned char conf_bq_dev(bq_dev_t* this, int cell_num)
{
  unsigned short temp;
  
  if(cell_num == 6)
  {
	  if(bq_dev_write_reg(this->device_address, ADC_CONTROL_REG, ADC_CONTROL_VAL_6) == INVALID)
	  {
		return INVALID;
	  }
  }
  else if(cell_num == 5)
  {
	  if(bq_dev_write_reg(this->device_address, ADC_CONTROL_REG, ADC_CONTROL_VAL_5) == INVALID)
	  	  {
	  		return INVALID;
	  	  }
  }
  else if(cell_num == 4)
  {
	  if(bq_dev_write_reg(this->device_address, ADC_CONTROL_REG, ADC_CONTROL_VAL_4) == INVALID)
	  	  {
	  		return INVALID;
	  	  }
  }

  if(bq_dev_write_reg(this->device_address, IO_CONTROL_REG, IO_CONTROL_VAL) == INVALID)
  {
  	return INVALID;
  }
  
  if(bq_dev_write_reg(this->device_address, CB_CTRL_REG, CB_CTRL_VAL) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_write_reg(this->device_address, CB_TIME_REG, CB_TIME_VAL) == INVALID)
  {
  	return INVALID;
  }

  if(bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL) == INVALID)
  {
  	return INVALID;
  }

  if(bq_dev_write_reg(this->device_address, FUNCTION_CONFIG_REG, FUNC_CONFIG_VAL_6) == INVALID)
  {
  	return INVALID;
  }
  
  if(bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL) == INVALID)
  {
  	return INVALID;
  }
  
  if(bq_dev_write_reg(this->device_address, IO_CONFIG_REG, IO_CONFIG_VAL) == INVALID)
  {
  	return INVALID;
  }

  temp = get_u32_value(COV_THRESHOLD);
  if (temp > 2000)
    temp = (temp - 2000)/50;
  else
    temp = 0;
  
  if(bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_write_reg(this->device_address, CONFIG_COV_REG, CONFIG_COV_VAL | temp) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_write_reg(this->device_address, CONFIG_COVT_REG,CONFIG_COVT_VAL | get_u16_value(COV_TIME)) == INVALID)
  {
  	return INVALID;
  }

  temp = get_u32_value(CUV_THRESHOLD);
  if (temp > 700)
    temp = (temp - 700)/100;
  else
    temp = 13;  /*Def CUV Threshold value = 2000mV*/

  if(bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL) == INVALID)
  {
  	return INVALID;
  }
  /*Def CUV Threshold = 2V*/
  if(bq_dev_write_reg(this->device_address, CONFIG_CUV_REG, CONFIG_CUV_VAL | temp) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_write_reg(this->device_address, CONFIG_CUVT_REG, CONFIG_CUVT_VAL| get_u16_value(CUV_TIME)) == INVALID)
  {
  	return INVALID;
  }


/*  temp = get_u32_value(PACK_OVER_TEMP1);
  if (temp <= 40)
    temp = 1;
  else if (temp >= 90)
    temp = 11;
  else
	  temp = (temp - 35)/5;

  temp2 = get_u32_value(PACK_OVER_TEMP2);
  if (temp2 <= 40)
    temp2 = 1;
  else if (temp2 >= 90)
    temp2 = 11;
  else
	  temp2 = (temp2 - 35)/5


  temp = (temp2<<4);
*/

  temp = dPACK_OVER_TEMP1_Config + (dPACK_OVER_TEMP2_Config << 4);

  if(bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_write_reg(this->device_address, CONFIG_OT_REG, temp) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_write_reg(this->device_address, CONFIG_OTT_REG, get_u16_value(PACK_OT_TIME1)) == INVALID)
  {
  	return INVALID;
  }

  this -> crc_error_count = 0;
  return VALID;
}

/**
* @brief Function Name: init_bq_dev_data_structure .                                                
* @brief Description  : initializes the structure that stores the information 
* of each BQ device .
* @param parameters   : number of BQ device in the stack                                                      
* @return Value       : none                                                    
*/     
unsigned char init_bq_dev_data_structure(bq_dev_t* this)
{
  if(bq_dev_read_reg(this->device_address, DEVICE_STATUS_REG, 1, DISCARD_CRC,
                 (unsigned char*) &this->device_status) == INVALID)
                 {
                 	return INVALID;
                 }
    
  if(bq_dev_read_reg(this->device_address, TEMPERATURE1_L_REG, 2, DISCARD_CRC,
                 (unsigned char*) &this->temperature1) == INVALID)
                 {
                 	return INVALID;
                 }
  if(bq_dev_read_reg(this->device_address, TEMPERATURE2_L_REG, 2, DISCARD_CRC,
                 (unsigned char*) &this->temperature2) == INVALID)
                 {
                 	return INVALID;
                 }

  /*Errors handling*/
  if(bq_dev_read_reg(this->device_address, ALERT_STATUS_REG, 1, DISCARD_CRC,
                 (unsigned char*) &this->alert_status) == INVALID)
                 {
                 	return INVALID;
                 }
  if(bq_dev_read_reg(this->device_address, FAULT_STATUS_REG, 1, DISCARD_CRC,
                 (unsigned char*) &this->fault_status) == INVALID)
                 {
                 	return INVALID;
                 }
  if(bq_dev_read_reg(this->device_address, COV_FAULT_REG, 1, DISCARD_CRC,
                 (unsigned char*) &this->cov_fault) == INVALID)
                 {
                 	return INVALID;
                 }
  if(bq_dev_read_reg(this->device_address, CUV_FAULT_REG, 1, DISCARD_CRC,
                 (unsigned char*) &this->cuv_fault) == INVALID)
                 {
                 	return INVALID;
                 }

  /*Clear alerts and faults (May not be so trivial...)*/
  bq_dev_clear_alerts(this);
  bq_dev_clear_faults(this);  
  
  return  VALID;
}


/**
* @brief Function Name: bq_dev_read_cell_voltage.                                                  
* @brief Description  : reads the cell voltage on a specific BQ device.
* @param parameters   : BQ device number in the stack                                                   
* @return Value       : returns the cell voltage read from the BQ device                                                    
*/     
short bq_dev_read_cell_voltage(bq_dev_t* this)
{
  unsigned char data[12];
  int temp;
  short i, ret_val;
  unsigned long voltage_comput;
  unsigned short cell_count = this->cell_count;
  
  ret_val = bq_dev_read_reg(this->device_address, VCELL1_L_REG, (cell_count<<1) , DISCARD_CRC,(unsigned char *) &data);
  if(ret_val == INVALID)
  {
	  return INVALID;
  }
  
  for (i=0; i<cell_count; i++)
  {

	  __byte(&temp,1) = data[i*2];
	  __byte(&temp,0) = data[(i*2)+1];

	  //compute cell voltage -> cell_voltage=VCELL x 6250/16383
	  voltage_comput = temp * (unsigned long)(adc_step_mul);
	  this->cell_voltage[i] = voltage_comput/((unsigned long)(adc_step_div));
  }
  
  return VALID;
}

unsigned char bq_dev_wake(bq_dev_t* this)
{
	bq_dev_write_reg(this->device_address, IO_CONTROL_REG, IO_CONTROL_VAL);
	return VALID;
}

unsigned char bq_dev_sleep(bq_dev_t* this)
{
	bq_dev_write_reg(this->device_address, IO_CONTROL_REG, 0x04);	//put to sleep
	bq_dev_clear_alerts(this);
	return VALID;
}

/**
* @brief Function Name: bq_dev_clear_alerts .                                                 
* @brief Description  : Clear the alert flags on the BQ device.
* @param parameters   : specific device ID                                                    
* @return Value       : none                                                    
*/     
unsigned char bq_dev_clear_alerts(bq_dev_t* this)
{
	unsigned char Value;

	//clear alert bit in device status register
	if(bq_dev_read_reg(this->device_address, DEVICE_STATUS_REG, 1, DISCARD_CRC,
			(unsigned char*) &Value) == INVALID)
	{
		return INVALID;
	}
	Value |= BIT5;
	//set alert bit as 1
	bq_dev_write_reg(this->device_address, DEVICE_STATUS_REG, Value);

	Value &= ~BIT5;
	//clear alert bit
	bq_dev_write_reg(this->device_address, DEVICE_STATUS_REG, Value);

	//read correct 1's from aleart status
	bq_dev_read_reg(this->device_address, ALERT_STATUS_REG, 1, DISCARD_CRC,
			(unsigned char*) &Value);

	//Write correct 1's to ALERT_STATUS_REG register
	bq_dev_write_reg(this->device_address, ALERT_STATUS_REG, Value);

	Value = 0x00;
	// Write 0's ALERT_STATUS_REG register
	bq_dev_write_reg(this->device_address, ALERT_STATUS_REG, Value);

	return VALID;
 
}


unsigned char bq_dev_clear_faults(bq_dev_t* this)
{
	unsigned char Value;

	//clear fault bit in device status register
	if(bq_dev_read_reg(this->device_address, DEVICE_STATUS_REG, 1, DISCARD_CRC,
			(unsigned char*) &Value) == INVALID)
	{
		return INVALID;
	}

	Value |= BIT6; //set fault bit as 1
	bq_dev_write_reg(this->device_address, DEVICE_STATUS_REG, Value);


	Value &= ~BIT6; //clear fault bit
	bq_dev_write_reg(this->device_address, DEVICE_STATUS_REG, Value);

	//read correct 1's from fault status
	bq_dev_read_reg(this->device_address, FAULT_STATUS_REG, 1, DISCARD_CRC,
			(unsigned char*) &Value);

	//Write correct 1's to FAULT_STATUS register
	bq_dev_write_reg(this->device_address, FAULT_STATUS_REG, Value);

	Value = 0x00;
	// Write 0's FAULT_STATUS register
	bq_dev_write_reg(this->device_address, FAULT_STATUS_REG, Value);

	return VALID;
}

unsigned char bq_dev_set_gpio(bq_dev_t* this)
{
  unsigned char Value;

    //clear fault bit in device status register
  if(bq_dev_read_reg(this->device_address, IO_CONTROL_REG, 1, DISCARD_CRC,
                 (unsigned char*) &Value) == 0)
                 {
               	  return 1;
                 }
  Value |= BIT6;
  //set GPIO_OUT bit as 1
  if(bq_dev_write_reg(this->device_address, IO_CONTROL_REG, Value) == 0)
  {
	  return 1;
  }
  return 0;
}

unsigned char bq_dev_clear_gpio(bq_dev_t* this)
{
  unsigned char Value;

    //clear fault bit in device status register
  if(bq_dev_read_reg(this->device_address, IO_CONTROL_REG, 1, DISCARD_CRC,
                 (unsigned char*) &Value) == 0)
  {
	  return 1;
  }
  Value &= ~BIT6;
  //clear GPIO_OUT
  if(bq_dev_write_reg(this->device_address, IO_CONTROL_REG, Value) == 0)
  {
	  return 1;
  }

  return 0;

}

short bq_dev_read_gpai(bq_dev_t* this)
{
  unsigned char data[2];

  //Bytes need to be swaped as BQ device supports Big Endian
  if(bq_dev_read_reg(this->device_address, GPAI_L_REG, 2, DISCARD_CRC,
                 (unsigned char*) &data[0]) == 0)
  {
  	return 1;
  }


  return 0;
}


unsigned char bq_dev_toggle_gpio(bq_dev_t* this)
{
  unsigned char Value;

    //clear fault bit in device status register
  if(bq_dev_read_reg(this->device_address, IO_CONTROL_REG, 1, DISCARD_CRC,
                 (unsigned char*) &Value) == 0)
  {
	  return 1;
  }
  Value ^= BIT6;
  //toggle GPIO_OUT
  if(bq_dev_write_reg(this->device_address, IO_CONTROL_REG, Value) == 0)
  {
	  return 1;
  }

  return 0;
}

/**
* @brief Function Name: bq_dev_read_errors.                                                 
* @brief Description  : Reads the device status, alert status, fault status,
* COV status, CUV status registers.
* @param parameters   : Device ID                                                    
* @return Value       : zero when the BQ device was read successfully
*/     
short bq_dev_read_errors(bq_dev_t* this)
{
  if(bq_dev_read_reg(this->device_address, DEVICE_STATUS_REG, 1, DISCARD_CRC,
                 (unsigned char*) &this->device_status) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_read_reg(this->device_address, ALERT_STATUS_REG, 1, DISCARD_CRC,
                 (unsigned char*) &this->alert_status) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_read_reg(this->device_address, FAULT_STATUS_REG, 1, DISCARD_CRC,
                 (unsigned char*) &this->fault_status) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_read_reg(this->device_address, COV_FAULT_REG, 1, DISCARD_CRC,
                 (unsigned char*) &this->cov_fault) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_read_reg(this->device_address, CUV_FAULT_REG, 1, DISCARD_CRC,
                 (unsigned char*) &this->cuv_fault) == INVALID)
  {
  	return INVALID;
  }
  
  return VALID;
}


/**
* @brief Function Name: bq_dev_read_temps.                                                 
* @brief Description  : reads the temperature registers.
* @param parameters   : Device ID                                                    
* @return Value       : zero when the BQ device was read sucessfully                                                    
*/     
short bq_dev_read_temps(bq_dev_t* this)
{
  unsigned char data[2];


  //RTS = (REGMSB × 256 + REGLSB) / 33104
  // RTS = .2 THEN TEMP = 40000 mC
  // RTS = .4 then temp = 90000 mC
  // slope = 250000
  // intercept = -10000
  // temp = (rts * 250000) - 10000
  //Bytes need to be swapped as BQ device supports Big Endian

  if(bq_dev_read_reg(this->device_address, TEMPERATURE1_L_REG, 2, DISCARD_CRC,
                 (unsigned char*) &data[0]) == INVALID)
  {
  	return INVALID;
  }

  this->temperature1ratio  = ((float)((data[0] << 8) | data[1]))/33104;
  this->temperature1 = (this->temperature1ratio * 250000) - 10000;

  if(bq_dev_read_reg(this->device_address, TEMPERATURE2_L_REG, 2, DISCARD_CRC,
                 (unsigned char*) &data[0]) == INVALID)
  {
  	return INVALID;
  }
  this->temperature2ratio  = ((float)((data[0] << 8) | data[1]))/33104;
  this->temperature2 = (this->temperature2ratio * 250000) - 10000;
 
  return VALID;
}


unsigned char bq_dev_read_status(bq_dev_t* this)
{
  if(bq_dev_read_reg(this->device_address, DEVICE_STATUS_REG, 1,DISCARD_CRC, (unsigned char*) &this->device_status) == INVALID)
  {
  	return INVALID;
  }
  return VALID;
}













/**
* @brief Function Name: copy_cell_voltage_values.                                                  
* @brief Description  : creates a backup of the last known cells voltages.
* @param parameters   : none                                                    
* @return Value       : none                                                    
*/     
//void copy_cell_voltage_values(void)
//{
//  unsigned short i, j, cell_cnt;
//
//  j = 0;
//  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
//  {
//    /*Read each cell voltage and compare it vs initial cell voltage*/
//    for (cell_cnt=0; cell_cnt<bq_pack.bq_devs[i].cell_count; cell_cnt++)
//    {
//      cell_values[j++] = bq_pack.bq_devs[i].cell_voltage[cell_cnt];
//    }
//  }
//
//  return;
//}
    



/**
* @brief Function Name: find_imbalanced_cell .                                                 
* @brief Description  : finds the imbalanced cells.
* @param parameters   : device ID                                                    
* @return Value       : Returns 0 if cells are balanced and other if there are 
* any imbalanced cells                                                     .
*/     
unsigned short find_imbalanced_cell(unsigned short in_dev_id,unsigned short low_cell)
{
  unsigned short cell_id, imb_cells_mask, cnt, cell_cnt;
  
  cnt = 0;
  cell_cnt = 0;
  imb_cells_mask = 0xFFFF;
  
  /*Read each cell voltage and compare it vs lowest cell voltage*/
  for (cell_id=0; cell_id<data_temp.bq_pack.bq_devs[in_dev_id].cell_count; cell_id++)
  {
    imb_cells_mask &= ~(1<<cnt);
    //changed to use lowest
    if ((data_temp.bq_pack.bq_devs[in_dev_id].cell_voltage[cell_id]) - low_cell < get_u32_value(BALANCE_VOLTS_THRESHOLD) && (data_temp.bq_pack.bq_devs[in_dev_id].cell_voltage[cell_id] > BALANCE_CELL_MIN) )
    {
      imb_cells_mask |= (1<<cnt);
      cell_cnt++;
    }
    cnt++;
  }
  data_temp.bq_pack.bq_devs[in_dev_id].num_cell_bal = cell_cnt;
  return imb_cells_mask;
}

/**
* @brief Function Name: enable_bypass_resistor.                                                 
* @brief Description  : Enable bypass resistors by controlling the CB outputs. 
* @param parameters   : Device ID, CB outputs to be controlled                                                    
* @return Value       : none                                                     
*/     
unsigned char enable_bypass_resistor(unsigned short in_dev_id, unsigned short in_value)
{
  if(bq_dev_write_reg(data_temp.bq_pack.bq_devs[in_dev_id].device_address,CB_TIME_REG, get_u32_value(BALANCE_TIME)) == INVALID)
  {
  	return INVALID;
  }
  if(bq_dev_write_reg(data_temp.bq_pack.bq_devs[in_dev_id].device_address,CB_CTRL_REG, (0x003F & in_value)) == INVALID)
  {
  	return INVALID;
  }
  
  data_temp.bq_pack.bq_devs[in_dev_id].cell_bal = in_value;

  return VALID;
}


/**
* @brief Function Name: disable_bypass_resistor.                                                 
* @brief Description  : disable bypass resistors by controlling the CB outputs .
* @param parameters   : Device ID, CB outputs to be controlled                                                    
* @return Value       : none                                                     
*/
unsigned char disable_bypass_resistor(unsigned short in_dev_id, unsigned short in_value)
{
  unsigned char reg_val;
  //Read CB_CTRL_REG register
  if(bq_dev_read_reg(data_temp.bq_pack.bq_devs[in_dev_id].device_address, CB_CTRL_REG, 1,
                  DISCARD_CRC, (unsigned char*) &reg_val) == 0)
  {
  	return 1;
  }
  
  //Write 0's to the bits in the CB_CTRL_REG register
  reg_val &= ~(in_value);
  if(bq_dev_write_reg(data_temp.bq_pack.bq_devs[in_dev_id].device_address,
                   CB_CTRL_REG, reg_val) == 0)
  {
  	return 1;
  }
  
  return 0;
}


/**
* @brief Function Name: disable_all_bypass_resistors.                                                 
* @brief Description  : disable all battery pack bypass resistors by controlling 
* the CB outputs .
* @param parameters   : none                                                    
* @return Value       : none                                                     
*/
unsigned char disable_all_bypass_resistors(void)
{
  unsigned short bq_dev_id;
  
  for (bq_dev_id=0; bq_dev_id<NUMBER_OF_BQ_DEVICES; bq_dev_id++)
  {
    if(bq_dev_write_reg(data_temp.bq_pack.bq_devs[bq_dev_id].device_address, CB_CTRL_REG, 0) == INVALID)
    {
    	return INVALID;
    }
    data_temp.bq_pack.bq_devs[bq_dev_id].cell_bal = 0;
    data_temp.bq_pack.bq_devs[bq_dev_id].num_cell_bal = 0;
  }
  data_temp.bq_pack.bal_num = 0;
  return VALID;
}


/*EOF*/

