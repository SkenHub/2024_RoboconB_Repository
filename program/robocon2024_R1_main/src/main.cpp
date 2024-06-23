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
uint8_t ROSdata[15];
float ROSfdata[3];
uint8_t robot_cmd[2];

OTOS mous;
uint8_t mous_data[36];
uint8_t robot_state[2];
uint8_t STMdata[39];

uint8_t num;
int num2 = 200;

SkenMdd mdd;
float mode[4] = {0,0,0,0};
float M1_gain[4] = {30,0,0,20};
float M2_gain[4] = {30,0,0,20};
float M3_gain[4] = {30,0,0,20};
float M4_gain[4] = {30,0,0,20};
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
	//受信データを処理
	for(int i=0; i<3; i++){
		for(int j=0; j<4; j++)	convert.uint8_val[j] = ROSdata[i*4+j+1];
		ROSfdata[i] = convert.float_val;
	}
	robot_cmd[0] = ROSdata[13];
	robot_cmd[1] = ROSdata[14];
}

void start_set(){
	num = ROSdata[0]&0xF0;
	if(num == 0xA0){
		//初期設定
		switch(ROSdata[0]&0x0F){
		case M1:
			for(int i=0; i<3; i++) M1_gain[i] = ROSfdata[i];
			break;
		case M2:
			for(int i=0; i<3; i++) M2_gain[i] = ROSfdata[i];
			break;
		case M3:
			for(int i=0; i<3; i++) M3_gain[i] = ROSfdata[i];
			break;
		case M4:
			for(int i=0; i<3; i++) M4_gain[i] = ROSfdata[i];
			break;
		case other:
			diameter[0] = ROSfdata[0];
			diameter[1] = ROSfdata[1];
			for(int i=0; i<4; i++)	encoder_parm[i] = ROSfdata[2];
			break;
		}
	}
}

void MDD_control(){
	//足回り動作
	if(ROSdata[0] == 0xA0){
		for(int i=0; i<3; i++) mecanum[i] = ROSfdata[i];
		mdd.udp(MECANUM_MODE,mecanum);
	}
}

void make_STMdata(){
	STMdata[0] = ROSdata[0];
	for(int i=1; i<37; i++){
		STMdata[i] = mous_data[i-1];
	}
	for(int i=0; i<2; i++){
		STMdata[37+i] = robot_state[i];
	}
}

void interrupt(){
	mous.raw_data(mous_data);
	make_STMdata();

	receive_set();
	start_set();
	MDD_control();
}

int main(void){
	sken_system.init();

	serial.init(C10,C11,SERIAL4,115200);
	serial.startDmaRead(ROSdata,15);

	mous.init(B6,B7,I2C_1);

	sken_system.addTimerInterruptFunc(interrupt,0,1);

	mdd.init(A9,A10,SERIAL1);
	mdd.tcp(MOTOR_COMMAND_MODE_SELECT,mode,10,2000);
	mdd.tcp(M1_PID_GAIN_CONFIG,M1_gain,10,2000);
	mdd.tcp(M2_PID_GAIN_CONFIG,M2_gain,10,2000);
	mdd.tcp(M3_PID_GAIN_CONFIG,M3_gain,10,2000);
	mdd.tcp(M4_PID_GAIN_CONFIG,M4_gain,10,2000);
	mdd.tcp(ENCODER_RESOLUTION_CONFIG,encoder_parm,10,2000);
	mdd.tcp(ROBOT_DIAMETER_CONFIG,diameter,10,2000);

	while(true){
		serial.write(STMdata,39,20);
	}
}
