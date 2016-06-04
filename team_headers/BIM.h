/*
 * BIM.h
 *
 *  Created on: May 21, 2015
 *      Author: Nathan Lord
 */

#ifndef BIM_H_
#define BIM_H_

#define bim_num 			6      // bim number


#if bim_num == 1
#define bim_dev_num 		4		//number of devices
#define bim_cell_num		24		//number of cells
#define bim_cell_offset		0		//offset of cell numbering for full pack (used min and max cell number)
#define HEARTBEAT_ID 		0x356
#define Stats1_ID			0x2ee
#define Stats2_ID			0x2ef
#define Cell_1_4_ID			0x30f
#define Cell_5_8_ID			0x310
#define Cell_9_12_ID		0x311
#define Cell_13_16_ID		0x312
#define Cell_17_20_ID		0x313
#define Cell_21_24_ID		0x314
#define Temp1_ID			0x339
#define Temp2_ID			0x33a
#define Temp3_ID			0x33b
#define Temp4_ID			0x33c
#endif

#if bim_num == 2
#define bim_dev_num 		4		//number of devices
#define bim_cell_num		22		//number of cells
#define bim_cell_offset		24		//offset of cell numbering for full pack (used min and max cell number)
#define HEARTBEAT_ID 		0x357
#define Stats1_ID			0x2f0
#define Stats2_ID			0x2f1
#define Cell_1_4_ID			0x315
#define Cell_5_8_ID			0x316
#define Cell_9_12_ID		0x317
#define Cell_13_16_ID		0x318
#define Cell_17_20_ID		0x319
#define Cell_21_24_ID		0x31a
#define Temp1_ID			0x33d
#define Temp2_ID			0x33e
#define Temp3_ID			0x33f
#define Temp4_ID			0x340
#endif

#if bim_num == 3
#define bim_dev_num 		4		//number of devices
#define bim_cell_num		24		//number of cells
#define bim_cell_offset		46		//offset of cell numbering for full pack (used min and max cell number)
#define HEARTBEAT_ID 		0x358
#define Stats1_ID			0x2f2
#define Stats2_ID			0x2f3
#define Cell_1_4_ID			0x31b
#define Cell_5_8_ID			0x31c
#define Cell_9_12_ID		0x31d
#define Cell_13_16_ID		0x31e
#define Cell_17_20_ID		0x31f
#define Cell_21_24_ID		0x320
#define Temp1_ID			0x341
#define Temp2_ID			0x342
#define Temp3_ID			0x343
#define Temp4_ID			0x344
#endif

#if bim_num == 4
#define bim_dev_num 		4		//number of devices
#define bim_cell_num		24		//number of cells
#define bim_cell_offset		70		//offset of cell numbering for full pack (used min and max cell number)
#define HEARTBEAT_ID 		835
#define Stats1_ID			0x2f4
#define Stats2_ID			0x2f5
#define Cell_1_4_ID			0x321
#define Cell_5_8_ID			0x322
#define Cell_9_12_ID		0x323
#define Cell_13_16_ID		0x324
#define Cell_17_20_ID		0x325
#define Cell_21_24_ID		0x326
#define Temp1_ID			0x345
#define Temp2_ID			0x346
#define Temp3_ID			0x347
#define Temp4_ID			0x348
#endif

#if bim_num == 5
#define bim_dev_num 		4		//number of devices
#define bim_cell_num		24		//number of cells
#define bim_cell_offset		96	//offset of cell numbering for full pack (used min and max cell number)
#define HEARTBEAT_ID 		836
#define Stats1_ID			0x2f6
#define Stats2_ID			0x2f7
#define Cell_1_4_ID			0x327
#define Cell_5_8_ID			0x328
#define Cell_9_12_ID		0x329
#define Cell_13_16_ID		0x32A
#define Cell_17_20_ID		0x32B
#define Cell_21_24_ID		0x32C
#define Temp1_ID			0x349
#define Temp2_ID			0x34a
#define Temp3_ID			0x34b
#define Temp4_ID			0x34c
#endif

#if bim_num == 6
#define bim_dev_num 		4		//number of devices
#define bim_cell_num		16		//number of cells
#define bim_cell_offset		118	//offset of cell numbering for full pack (used min and max cell number)
#define HEARTBEAT_ID 		836
#define Stats1_ID			0x2f8
#define Stats2_ID			0x2f9
#define Cell_1_4_ID			0x32D
#define Cell_5_8_ID			0x32E
#define Cell_9_12_ID		0x32F
#define Cell_13_16_ID		0x330
#define Cell_17_20_ID		0
#define Cell_21_24_ID		0
#define Temp1_ID			0x34d
#define Temp2_ID			0x34e
#define Temp3_ID			0x34f
#define Temp4_ID			0x350
#endif

#endif /* BIM_H_ */
