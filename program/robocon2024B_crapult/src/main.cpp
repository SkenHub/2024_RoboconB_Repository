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

PS4 ps4;
PS4_data ps4_data;

IM920 im920;
uint16_t NN;
uint8_t im920_data[8];

RcPwm servo;
float servo1_param[4] = {20,20,50,1.0};
enum servo_param{
	now,min,max,rate,
};

Uart mdd;
float omni[3] = {0,0,0};
uint8_t send_data[14];
ConvertIntFloat convert;

Gpio sw;

void servo_control(){
	//if(ps4_data.R1){
	if(im920_data[0]&0x02){
		servo1_param[now] += (servo1_param[max]>servo1_param[now])? servo1_param[rate]:0;
	}else{
		servo1_param[now] -= (servo1_param[min]<servo1_param[now])? servo1_param[rate]:0;
	}
	servo.turn(servo1_param[now]);
}

void omni_set(){
	/*
	omni[0] = ps4_data.LxPad/127.0 * 1000;
	omni[1] = ps4_data.LyPad/127.0 * 1000;
	omni[2] = ps4_data.RxPad/127.0 * -150;
	*/
	omni[0]  = (im920_data[1]&0x01)? 1000 : 0;
	omni[0] -= (im920_data[1]&0x02)? 1000 : 0;
	omni[1]  = (im920_data[1]&0x04)? 1000 : 0;
	omni[1] -= (im920_data[1]&0x08)? 1000 : 0;
	omni[2]  = (im920_data[1]&0x10)? -150 : 0;
	omni[2] -= (im920_data[1]&0x20)? -150 : 0;
}

void send_write(){
	send_data[0] = 0xA5;
	for(int i=0; i<3; i++){
		convert.float_val = omni[i];
		for(int j=0; j<4; j++) send_data[i*4+j+1] = convert.uint8_val[j];
	}
	send_data[13] = ((im920_data[0]>>2)&0x02) | (im920_data[0]&0x01);
}

void interrupt(){
	ps4.Getdata(&ps4_data);
	im920.read(&NN,im920_data,Bytes8);
	servo_control();
	omni_set();
	send_write();
}

int main(void){
	sken_system.init();

	//ps4.StartRecive(C10,C11,SERIAL3);
	im920.init(C10,C11,SERIAL3);

	servo.init(B6,TIMER4,CH1);

	sw.init(C13,INPUT_PULLUP);

	mdd.init(A9,A10,SERIAL1,115200);

	sken_system.addTimerInterruptFunc(interrupt,0,1);

	while(true){
		mdd.write(send_data,14,10);
	}
}
