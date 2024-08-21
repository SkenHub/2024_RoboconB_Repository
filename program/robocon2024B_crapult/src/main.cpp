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

CanData can_data;
uint8_t can_tx[8] = {0,0,0,0,0,0,0,0};
struct RoboMas{
	float deg,rps,torque;
}robomas;

RcPwm servo[2];
float servo1_param[4] = {0,0,15,1.0};
float servo2_param[4] = {0,0,20,5.0};
enum servo_param{
	now,min,max,rate,
};

Uart mdd;
float omni[3] = {0,0,0};
uint8_t omni_data[13],deg_data[5],DegData[5];
float deg;
ConvertIntFloat convert;

Gpio sw;

int16_t robomas_data[3];
void get_robomas_data(){
	for(int i=0; i<3; i++){
		robomas_data[i] = can_data.rx_data[i*2]<<8 | can_data.rx_data[i*2+1];
	}
	robomas.rps = robomas_data[1]/1000.0;
}

int16_t robomas_send_data = 0;
void send_robomas(){
	if(/*ps4_data.Square*/(im920_data[0]&0x01) && robomas_send_data > -10000) robomas_send_data += -10;
	if(/*ps4_data.Circle*/(im920_data[0]&0x08) && deg>50) robomas_send_data = 8000;
	if(!(/*ps4_data.Square*/(im920_data[0]&0x01) && deg < 200) && robomas_send_data < 0) robomas_send_data = 0;
	if(!(/*ps4_data.Square*/(im920_data[0]&0x08) && deg >  50) && robomas_send_data > 0) robomas_send_data = 0;
	can_tx[0] = (robomas_send_data>>8)&0xFF;
	can_tx[1] =  robomas_send_data    &0xFF;
}

void servo_control(){
	//if(ps4_data.R1){
	if(im920_data[0]&0x02){
		servo1_param[now] += (servo1_param[max]>servo1_param[now])? servo1_param[rate]:0;
	}else{
		servo1_param[now] -= (servo1_param[min]<servo1_param[now])? servo1_param[rate]:0;
	}

	//if(ps4_data.Cross){
	if(im920_data[0]&0x04){
		servo2_param[now] += (servo2_param[max]>servo2_param[now])? servo2_param[rate]:0;
	}else{
		servo2_param[now] -= (servo2_param[min]<servo2_param[now])? servo2_param[rate]:0;
	}
	servo[0].turn(servo1_param[now]);
	servo[1].turn(servo2_param[now]);
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

void omni_write(){
	omni_data[0] = 0xA5;
	for(int i=0; i<3; i++){
		convert.float_val = omni[i];
		for(int j=0; j<4; j++) omni_data[i*4+j+1] = convert.uint8_val[j];
	}
}

void enc_deg(){
	for(int i=0; i<5; i++){
		if(deg_data[i] == 0xA6){
			for(int j=0; j<5; j++) DegData[j] = deg_data[(i+j)%5];
		}
	}
	for(int i=0; i<4; i++){
		convert.uint8_val[i] = DegData[i+1];
	}
	deg = convert.float_val;
}

void interrupt(){
	ps4.Getdata(&ps4_data);
	im920.read(&NN,im920_data,Bytes8);
	get_robomas_data();
	send_robomas();
	servo_control();
	omni_set();
	omni_write();
	enc_deg();
}

int main(void){
	sken_system.init();

	//ps4.StartRecive(C10,C11,SERIAL3);
	im920.init(C10,C11,SERIAL3);

	sken_system.startCanCommunicate(B13,B12,CAN_2);
	sken_system.addCanRceiveInterruptFunc(CAN_2,&can_data);

	servo[0].init(B6,TIMER4,CH1);
	servo[1].init(B7,TIMER4,CH2);

	sw.init(C13,INPUT_PULLUP);

	mdd.init(A9,A10,SERIAL1,115200);
	mdd.startDmaRead(deg_data,5);

	sken_system.addTimerInterruptFunc(interrupt,0,1);

	while(true){
		sken_system.canTransmit(CAN_2,0x200,can_tx,8);
		mdd.write(omni_data,13,10);
	}
}
