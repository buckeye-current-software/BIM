/*
 * all.h
 *
 *  Created on: Oct 29, 2013
 *      Author: Nathan
 */

#ifndef ALL_H_
#define ALL_H_

#include "template_all.h"
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "stopwatch_template.h"
#include "op.h"
#include "sys_op.h"
#include "data.h"
#include "sys_data.h"
#include "Init.h"
#include "Init_template.h"
#include "can_template.h"
#include "SensorCov.h"
#include "SensorCov_template.h"
#include "boot_template.h"
#include "PowerDown.h"
#include "PowerDown_template.h"
#include "common_template.h"
#include "main.h"
#include "clock.h"
#include "clock_template.h"
#include "can.h"
#include <stdlib.h>
#include <math.h>
#include "adc_template.h"
#include "gpio_template.h"
#include "DSP2803x_GlobalPrototypes.h"
#include "Flash2803x_API_Library.h"
#include "BQ.h"
#include "bq_data_flash.h"
#include "bq_pack.h"
#include "spi_bq.h"
#include "spi.h"
#include "canfloat.h"

extern sys_ops_struct sys_ops;
extern user_ops_struct user_ops;
extern sys_data_struct sys_data;
extern user_data_struct user_data;

#define CPU_FREQ_MHZ	60

#endif /* ALL_H_ */
