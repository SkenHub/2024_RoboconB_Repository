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
float MousData[18];
float past_mous[9];
float x_pos,y_pos,Deg;
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

SkenMdd mdd;
float mode[4] = {0,0,0,0};

float M1_gain[4] = {20,1,0.1,20};
float M2_gain[4] = {20,1,0.1,20};
float M3_gain[4] = {20,1,0.1,20};
float M4_gain[4] = {20,1,0.1,20};
float diameter[4] = {100,1200,0,0};
float encoder_parm[4] = {8192,8192,8192,8192};
float mecanum[4] = {0,0,0,0};

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
	//足回り動作
	mdd_serial.write(ROSdata,17);
	/*
	if(ROSdata[0] == 0xA0){
		for(int i=0; i<3; i++) mecanum[i] = ROSfdata[i];
	}else{
		for(int i=0; i<3; i++) mecanum[i] = 0;
	}
	mdd.udp(MECANUM_MODE,mecanum);
	if(ROSdata[13]&0x01) mdd.tcp(PID_RESET_COMMAND,mode,10,1000);
	*/
}

void sensor_set(){
	encoder[0].interrupt(&enc_data[0]);
	encoder[1].interrupt(&enc_data[1]);
	robot_state[0] = 0;
	for(int i=0; i<6; i++) robot_state[0] |= (limit[i].read()?1:0)<<i;
	enc_deg = (enc_data[0].distance-enc_data[1].distance)/(measu_param[1]) * (360.0/(2.0*PI));
	enc_deg_bit = enc_deg/0.0055;
}

void mous_to_bit(){
	int16_t bit;
	for(int i=0; i<9; i++){
		bit = MousData[i]/coefficient[i];
		mous_data[i*2]   =  bit     & 0xFF;
		mous_data[i*2+1] = (bit>>8) & 0xFF;
	}
}

void make_STMdata(){
	STMdata[0] = 0xA6;
	STMdata[1] = ROSdata[0];
	for(int i=0; i<18; i++){
		STMdata[i+2] = mous_data[i];
	}
	/*
	STMdata[6] = enc_deg_bit     & 0xFF;
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
	//mous.raw_data(mous_data,3);
	//get_OTOS();
	mous.interrupt(MousData);
	mous_to_bit();
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

	/*
	mdd.init(A9,A10,SERIAL1);
	//mdd.init(MDD_0,A12,A11,CAN_1);
	mdd.tcp(MOTOR_COMMAND_MODE_SELECT,mode,10,2000);
	mdd.tcp(M1_PID_GAIN_CONFIG,M1_gain,10,2000);
	mdd.tcp(M2_PID_GAIN_CONFIG,M2_gain,10,2000);
	mdd.tcp(M3_PID_GAIN_CONFIG,M3_gain,10,2000);
	mdd.tcp(M4_PID_GAIN_CONFIG,M4_gain,10,2000);
	mdd.tcp(ENCODER_RESOLUTION_CONFIG,encoder_parm,10,2000);
	mdd.tcp(ROBOT_DIAMETER_CONFIG,diameter,10,2000);
	mdd.tcp(MECANUM_MODE,mecanum,10,2000);
	*/
	mdd_serial.init(A9,A10,SERIAL1,115200);

	mous.set_odom(0,0,0);

	sken_system.addTimerInterruptFunc(interrupt,0,1);
	sken_system.addTimerInterruptFunc(send_ros,1,10);
	while(true){
	}
}
