/*
 * BIM.h
 *
 *  Created on: May 21, 2015
 *      Author: Nathan Lord
 */

#ifndef BIM_H_
#define BIM_H_

#define bim_num 			5       // bim number


#if bim_num == 1
#define bim_dev_num 		4		//number of devices
#define bim_cell_num		24		//number of cells
#define bim_cell_offset		84		//offset of cell numbering for full pack (used min and max cell number)
#define HEARTBEAT_ID 		832
#define Stats1_ID			0x305
#define Stats2_ID			0x306
#define Cell_1_4_ID			805
#define Cell_5_8_ID			806
#define Cell_9_12_ID		807
#define Cell_13_16_ID		808
#define Cell_17_20_ID		809
#define Cell_21_24_ID		810
#define Temp1_ID			826
#define Temp2_ID			827
#define Temp3_ID			828
#define Temp4_ID			829
#endif

#if bim_num == 2
#define bim_dev_num 		4		//number of devices
#define bim_cell_num		24		//number of cells
#define bim_cell_offset		60		//offset of cell numbering for full pack (used min and max cell number)
#define HEARTBEAT_ID 		833
#define Stats1_ID			775
#define Stats2_ID			776
#define Cell_1_4_ID			799
#define Cell_5_8_ID			800
#define Cell_9_12_ID		801
#define Cell_13_16_ID		802
#define Cell_17_20_ID		803
#define Cell_21_24_ID		804
#define Temp1_ID			822
#define Temp2_ID			823
#define Temp3_ID			824
#define Temp4_ID			825
#endif

#if bim_num == 3
#define bim_dev_num 		4		//number of devices
#define bim_cell_num		24		//number of cells
#define bim_cell_offset		36		//offset of cell numbering for full pack (used min and max cell number)
#define HEARTBEAT_ID 		834
#define Stats1_ID			777
#define Stats2_ID			778
#define Cell_1_4_ID			793
#define Cell_5_8_ID			794
#define Cell_9_12_ID		795
#define Cell_13_16_ID		796
#define Cell_17_20_ID		797
#define Cell_21_24_ID		798
#define Temp1_ID			818
#define Temp2_ID			819
#define Temp3_ID			820
#define Temp4_ID			821
#endif

#if bim_num == 4
#define bim_dev_num 		4		//number of devices
#define bim_cell_num		24		//number of cells
#define bim_cell_offset		12		//offset of cell numbering for full pack (used min and max cell number)
#define HEARTBEAT_ID 		835
#define Stats1_ID			779
#define Stats2_ID			780
#define Cell_1_4_ID			787
#define Cell_5_8_ID			788
#define Cell_9_12_ID		789
#define Cell_13_16_ID		790
#define Cell_17_20_ID		791
#define Cell_21_24_ID		792
#define Temp1_ID			814
#define Temp2_ID			815
#define Temp3_ID			816
#define Temp4_ID			817
#endif

#if bim_num == 5
#define bim_dev_num 		2		//number of devices
#define bim_cell_num		12		//number of cells
#define bim_cell_offset		0	//offset of cell numbering for full pack (used min and max cell number)
#define HEARTBEAT_ID 		836
#define Stats1_ID			781
#define Stats2_ID			782
#define Cell_1_4_ID			784
#define Cell_5_8_ID			785
#define Cell_9_12_ID		786
#define Cell_13_16_ID		0
#define Cell_17_20_ID		0
#define Cell_21_24_ID		0
#define Temp1_ID			812
#define Temp2_ID			813
#define Temp3_ID			0
#define Temp4_ID			0
#endif

#endif /* BIM_H_ */
