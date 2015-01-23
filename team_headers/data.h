/*
 * data.h
 *
 *  Created on: Oct 30, 2013
 *      Author: Nathan
 */

#ifndef DATA_H_
#define DATA_H_

typedef union CANFLOAT
{
	float F32;
	unsigned long U32;
} canfloat;

typedef struct USER_DATA
{
	canfloat coolant_flow;
	canfloat motor_coolant_temp;
	canfloat motor_control_coolant_temp;
	canfloat radiator_coolant_temp;
	canfloat coolant_pressure_1;
	canfloat coolant_pressure_2;
	canfloat motor_plate_temp_1;
	canfloat motor_plate_temp_2;
	canfloat ambient_temp;
	canfloat motor_temp;
	char gp_button;
} user_data_struct;

#endif /* DATA_H_ */
