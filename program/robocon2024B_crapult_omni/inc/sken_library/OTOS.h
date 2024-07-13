/*
 * OTOS.h
 *
 *  Created on: 2024/06/21
 *      Author: hhhot
 */

#ifndef SKEN_LIBRARY_OTOS_H_
#define SKEN_LIBRARY_OTOS_H_

#include "include.h"

class OTOS {
	I2C i2c_;
	float coefficient_[9] =
	{0.0003,0.0003,0.0055,0.00015,0.00015,0.061,0.0048,0.0048,5.5};
	uint8_t data_[2];
	int16_t Data_;
	float odom_[18];
public:
	OTOS();
	void init(Pin scl,Pin sda,I2CSelect i2c,int drift_time = 5000);
	void get_odom(float* odom,int odom_num);
	void set_odom(float x,float y,float deg);
	void set_xpos(float x);
	void set_ypos(float y);
	void set_deg(float deg);
	void offset_odom(float x,float y,float deg);
	void raw_data(uint8_t* data,int odom_num);
	void reset();
};

#endif /* SKEN_LIBRARY_OTOS_H_ */
