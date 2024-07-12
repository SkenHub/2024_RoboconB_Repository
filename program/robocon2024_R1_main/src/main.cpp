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

ConvertIntFloat convert;

Uart serial;
uint8_t ROSdata[20],rosdata[20];
float ROSfdata[4];
uint8_t robot_cmd[3];

uint8_t robot_state[3], STMdata[42];

OTOS mous;
uint8_t mous_data[36];
float MousData[9] = {0,0,0,0,0,0,0,0,0};
unsigned int check_sum;
Gpio sw;

Encoder encoder[2];
Encoder_data enc_data[2];

Gpio limit[6];
uint8_t limit_data;

uint8_t fst_byte;

SkenMdd mdd;
float mode[4] = {0,0,0,0};

float M1_gain[4] = {20,1,0.1,20};
float M2_gain[4] = {20,1,0.1,20};
float M3_gain[4] = {20,1,0.1,20};
float M4_gain[4] = {20,1,0.1,20};
float diameter[4] = {100,1200,0,0};
float encoder_parm[4] = {8192,8192,8192,8192};
float mecanum[4] = {0,0,0,0};

enum InitCase{
	M1=1,
	M2,
	M3,
	M4,
	other,
};

void receive_set(){
	for(int i=0; i<20; i++){
		fst_byte = rosdata[i]&0xA0;
		if(fst_byte == 0xA0){
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

void start_set(){
	//初期設定
	switch(ROSdata[0]&0x0F){
	case M1:
		for(int i=0; i<3; i++) M1_gain[i] = ROSfdata[i];
		mdd.tcp(M1_PID_GAIN_CONFIG,M1_gain,10,2000);
		break;
	case M2:
		for(int i=0; i<3; i++) M2_gain[i] = ROSfdata[i];
		mdd.tcp(M2_PID_GAIN_CONFIG,M2_gain,10,2000);
		break;
	case M3:
		for(int i=0; i<3; i++) M3_gain[i] = ROSfdata[i];
		mdd.tcp(M3_PID_GAIN_CONFIG,M3_gain,10,2000);
		break;
	case M4:
		for(int i=0; i<3; i++) M4_gain[i] = ROSfdata[i];
		mdd.tcp(M4_PID_GAIN_CONFIG,M4_gain,10,2000);
		break;
	case other:
		diameter[0] = ROSfdata[0];
		diameter[1] = ROSfdata[1];
		for(int i=0; i<4; i++)	encoder_parm[i] = ROSfdata[2];
		break;
	}
}

void MDD_control(){
	//足回り動作
	if(ROSdata[0] == 0xA0){
		for(int i=0; i<3; i++) mecanum[i] = ROSfdata[i];
	}else{
		for(int i=0; i<3; i++) mecanum[i] = 0;
	}
	mdd.udp(MECANUM_MODE,mecanum);
	if(ROSdata[13]&0x01) mdd.udp(PID_RESET_COMMAND,mode);
}

void sensor_set(){
	encoder[0].interrupt(&enc_data[0]);
	encoder[1].interrupt(&enc_data[1]);
	robot_state[0] = 0;
	for(int i=0; i<6; i++) robot_state[0] |= (limit[i].read()?1:0)<<i;
}

void make_STMdata(){
	STMdata[0] = 0xA6;
	for(int i=1; i<7; i++){
		STMdata[i] = mous_data[i-1];
	}
	STMdata[7] = ROSdata[0];
	for(int i=8; i<11; i++){
		STMdata[i] = robot_state[i-8];
	}
	check_sum = 0;
	for(int i=1;i<11;i++){
		check_sum += STMdata[i];
	}
	STMdata[11] = check_sum & 0xFF;
}

void interrupt(){
	mous.raw_data(mous_data,3);
	mous.get_odom(MousData,3);
	receive_set();
	MDD_control();
	sensor_set();
	make_STMdata();
	start_set();
}

void send_ros(){
	serial.write(STMdata,12);
	//MDD_control();
}

void OTOS_get(){
	mous.set_odom(MousData[0],MousData[1],ROSfdata[3]);
}

int main(void){
	sken_system.init();

	serial.init(C10,C11,SERIAL3,115200);
	serial.startDmaRead(rosdata,20);

	mous.init(B6,B7,I2C_1);
	sw.init(C13,INPUT_PULLUP);

	encoder[0].init(A0,A1,TIMER5);
	encoder[1].init(B3,A5,TIMER2);

	limit[0].init(B8,INPUT_PULLUP);
	limit[1].init(B9,INPUT_PULLUP);
	limit[2].init(A7,INPUT_PULLUP);
	limit[3].init(A6,INPUT_PULLUP);
	limit[4].init(A8,INPUT_PULLUP);
	limit[5].init(B15,INPUT_PULLUP);

	//mdd.init(A9,A10,SERIAL1);
	mdd.init(MDD_0,A12,A11,CAN_1);
	mdd.tcp(MOTOR_COMMAND_MODE_SELECT,mode,10,2000);
	mdd.tcp(M1_PID_GAIN_CONFIG,M1_gain,10,2000);
	mdd.tcp(M2_PID_GAIN_CONFIG,M2_gain,10,2000);
	mdd.tcp(M3_PID_GAIN_CONFIG,M3_gain,10,2000);
	mdd.tcp(M4_PID_GAIN_CONFIG,M4_gain,10,2000);
	mdd.tcp(ENCODER_RESOLUTION_CONFIG,encoder_parm,10,2000);
	mdd.tcp(ROBOT_DIAMETER_CONFIG,diameter,10,2000);
	mdd.tcp(MECANUM_MODE,mecanum,10,2000);

	mous.set_odom(0,0,0);

	sken_system.addTimerInterruptFunc(interrupt,0,1);
	sken_system.addTimerInterruptFunc(send_ros,1,10);
	sken_system.addTimerInterruptFunc(OTOS_get,2,100);
	while(true){
	}
}
