/*
 * OTOS.cpp
 *
 *  Created on: 2024/06/21
 *      Author: hhhot
 */

#include "OTOS.h"

OTOS::OTOS() {}

void OTOS::init(Pin scl,Pin sda,I2CSelect i2c,int drift_time){
	i2c_.init(scl,sda,i2c,20000);
	reset();
}

void OTOS::get_odom(float* odom,int odom_num){
	for(int i=0; i<odom_num; i++){
		i2c_.read(0x17,0x20+(i*2)  ,1,(uint8_t*)&data_[0],1);
		i2c_.read(0x17,0x20+(i*2)+1,1,(uint8_t*)&data_[1],1);
		Data_ = data_[1]<<8 | data_[0];
		odom_[i] = Data_*coefficient_[i%9];
		odom[i] = odom_[i];
	}
}

void OTOS::set_odom(float x,float y,float deg){
	int16_t Data[3];
	Data[0] = x/coefficient_[0];
	Data[1] = y/coefficient_[1];
	Data[2] = deg/coefficient_[2];
	for(int i=0; i<3; i++){
		data_[0] =  Data[i]     & 0xFF;
		data_[1] = (Data[i]>>8) & 0xFF;
		i2c_.write(0x17,0x20+(i*2)  ,1,(uint8_t*)&data_[0],1);
		i2c_.write(0x17,0x20+(i*2)+1,1,(uint8_t*)&data_[1],1);
	}
}

void OTOS::set_xpos(float x){
	int16_t Data = x/coefficient_[0];
	data_[0] =  Data     & 0xFF;
	data_[1] = (Data>>8) & 0xFF;
	i2c_.write(0x17,0x20,1,(uint8_t*)&data_[0],1);
	i2c_.write(0x17,0x21,1,(uint8_t*)&data_[1],1);
}
void OTOS::set_ypos(float y){
	int16_t Data = y/coefficient_[1];
	data_[0] =  Data     & 0xFF;
	data_[1] = (Data>>8) & 0xFF;
	i2c_.write(0x17,0x22,1,(uint8_t*)&data_[0],1);
	i2c_.write(0x17,0x23,1,(uint8_t*)&data_[1],1);
}
void OTOS::set_deg(float deg){
	int16_t Data = deg/coefficient_[2];
	data_[0] =  Data     & 0xFF;
	data_[1] = (Data>>8) & 0xFF;
	i2c_.write(0x17,0x24,1,(uint8_t*)&data_[0],1);
	i2c_.write(0x17,0x25,1,(uint8_t*)&data_[1],1);
}

void OTOS::offset_odom(float x,float y,float deg){
	int16_t Data[3];
	Data[0] = x/coefficient_[0];
	Data[1] = y/coefficient_[1];
	Data[2] = deg/coefficient_[2];
	for(int i=0; i<3; i++){
		data_[0] =  Data[i]     & 0xFF;
		data_[1] = (Data[i]>>8) & 0xFF;
		i2c_.write(0x17,0x10+(i*2)  ,1,(uint8_t*)&data_[0],1);
		i2c_.write(0x17,0x10+(i*2)+1,1,(uint8_t*)&data_[1],1);
	}
}

void OTOS::raw_data(uint8_t* data,int odom_num){
	for(int i=0; i<odom_num*2; i++){
		i2c_.read(0x17,0x20+i,1,(uint8_t*)&data[i],1);
	}
}

void OTOS::reset(){
	uint8_t data=1;
	i2c_.write(0x17,0x07,1,(uint8_t*)&data,1);
}
