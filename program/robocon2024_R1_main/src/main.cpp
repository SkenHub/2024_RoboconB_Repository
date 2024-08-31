/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"
#include "sken_library/OTOS.h"
#include "math.h"

ConvertIntFloat convert;

Uart serial;
uint8_t ROSdata[20],rosdata[20];
float ROSfdata[4];
uint8_t robot_cmd[3];

uint8_t robot_state[3], STMdata[42];

OTOS mous;
uint8_t mous_data[36];
float coefficient[9] = {0.0003,0.0003,0.0055,0.00015,0.00015,0.061,0.0048,0.0048,5.5};
float MousData[9];
float past_mous[9];
float OdomData[9];
int16_t x_bit,y_bit,deg_bit;
unsigned int check_sum;
Gpio sw;

Encoder encoder[2];
Encoder_data enc_data[2];
float enc_deg;
int16_t enc_deg_bit;
float measu_param[2] = {55,817.5};

Gpio limit[6];
uint8_t limit_data;

Uart mdd_serial;
uint8_t tx_data[17];

void receive_set(){
	for(int i=0; i<20; i++){
		if(rosdata[i] == 0xA0){
			for(int j=0; j<20; j++) ROSdata[j] = rosdata[(i+j)%20];
			break;
		}
	}
	//受信データを処理
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++)	convert.uint8_val[j] = ROSdata[i*4+j+1];
		ROSfdata[i] = convert.float_val;
	}
	robot_cmd[0] = ROSdata[17];
	robot_cmd[1] = ROSdata[18];
	robot_cmd[2] = ROSdata[19];
}

void MDD_control(){
	ROSdata[17] &= ~((robot_state[0]>>4)&0b11);
	mdd_serial.write(ROSdata,18);
}

void sensor_set(){
	encoder[0].interrupt(&enc_data[0]);
	encoder[1].interrupt(&enc_data[1]);
	robot_state[0] = 0;
	for(int i=0; i<6; i++) robot_state[0] |= (limit[i].read()?1:0)<<i;
	enc_deg = (enc_data[0].distance-enc_data[1].distance)/(measu_param[1]) * (360.0/(2.0*PI));
	enc_deg_bit = enc_deg/0.0055;
}

void make_STMdata(){
	STMdata[0] = 0xA6;
	STMdata[1] = ROSdata[0];
	for(int i=0; i<18; i++){
		STMdata[i+2] = mous_data[i];
	}
	/*
	STMdata[6] =  enc_deg_bit     & 0xFF;
	STMdata[7] = (enc_deg_bit>>8) & 0xFF;
	*/
	for(int i=0; i<3; i++){
		STMdata[i+20] = robot_state[i];
	}
	check_sum = 0;
	for(int i=1;i<23;i++){
		check_sum += STMdata[i];
	}
	STMdata[23] = check_sum & 0xFF;
}

void interrupt(){
	mous.raw_data(mous_data,9);
	receive_set();
	MDD_control();
	sensor_set();
	make_STMdata();
}

void send_ros(){
	serial.write(STMdata,24);
	//MDD_control();
}

int main(void){
	sken_system.init();

	serial.init(C10,C11,SERIAL3,115200);
	serial.startDmaRead(rosdata,20);

	mous.init(B6,B7,I2C_1);
	sw.init(C13,INPUT_PULLUP);

	encoder[0].init(A0,A1,TIMER5,measu_param[0]);
	encoder[1].init(B3,A5,TIMER2,measu_param[0]);

	limit[0].init(B8,INPUT_PULLUP);
	limit[1].init(B9,INPUT_PULLUP);
	limit[2].init(A7,INPUT_PULLUP);
	limit[3].init(A6,INPUT_PULLUP);
	limit[4].init(A8,INPUT_PULLUP);
	limit[5].init(B15,INPUT_PULLUP);

	mdd_serial.init(A9,A10,SERIAL1,115200);

	mous.set_odom(0,0,0);

	sken_system.addTimerInterruptFunc(interrupt,0,1);
	sken_system.addTimerInterruptFunc(send_ros,1,1);
	while(true){
	}
}
