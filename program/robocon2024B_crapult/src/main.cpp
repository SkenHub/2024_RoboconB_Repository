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

IM920 im920;
uint16_t im_id;
uint8_t im_data[8];
uint8_t rx_data;

CanData can_data;
uint8_t can_tx[8] = {0,0,0,0,0,0,0,0};
struct RoboMas{
	float deg,rpm,torque;
}robomas;

RcPwm servo[2];
float servo1_param[4] = {50,40,60,0.01};
float servo2_param[4] = {50,40,60,0.01};
enum servo_param{
	now,min,max,rate,
};

SkenMdd mdd;
float mode[4] = {0,0,0,0};
float M1_gain[4] = {20,0,0,20};
float M2_gain[4] = {20,5,0,20};
float M3_gain[4] = {20,5,0,20};
float M4_gain[4] = {20,0,0,20};
float diameter[4] = {100,1200,0,0};
float encoder_parm[4] = {8192,8192,8192,8192};
float omni[4] = {0,0,0,0};

Gpio sw;

void im920_get(){
	im920.read(&im_id,im_data,Bytes8);
	rx_data = im_data[0];
}

int16_t robomas_data[3];
void get_robomas_data(){
	for(int i=0; i<3; i++){
		robomas_data[i] = can_data.rx_data[i*2]<<8 | can_data.rx_data[i*2+1];
	}
	robomas.rpm = robomas_data[1]/1000.0;
}

int16_t robomas_send_data;
void send_robomas(){
	if(rx_data&0x01){
		robomas_send_data = 1000;
	}else{
		robomas_send_data = 0;
	}
	can_tx[0] = (robomas_send_data>>8)&0xFF;
	can_tx[1] =  robomas_send_data    &0xFF;
}

void servo_control(){
	if(rx_data&0x02){
		servo1_param[now] += (servo1_param[max]>servo1_param[now])? servo1_param[rate]:0;
	}else{
		servo1_param[now] -= (servo1_param[min]<servo1_param[now])? servo1_param[rate]:0;
	}

	if(rx_data&0x04){
		servo2_param[now] += (servo2_param[max]>servo2_param[now])? servo2_param[rate]:0;
	}else{
		servo2_param[now] -= (servo2_param[min]<servo2_param[now])? servo2_param[rate]:0;
	}
	servo[0].turn(servo1_param[now]);
	servo[1].turn(servo2_param[now]);
}

void interrupt(){
	get_robomas_data();
	send_robomas();
	servo_control();
	im920_get();
}

int main(void){
	sken_system.init();

	im920.init(C10,C11,SERIAL3);

	sken_system.startCanCommunicate(B13,B12,CAN_2);
	sken_system.addCanRceiveInterruptFunc(CAN_2,&can_data);

	servo[0].init(A0,TIMER1,CH1);
	servo[1].init(A0,TIMER1,CH1);

	sw.init(C13,INPUT_PULLUP);

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
	*/

	sken_system.addTimerInterruptFunc(interrupt,0,1);

	while(true){
		sken_system.canTransmit(CAN_2,0x200,can_tx,8);
	}
}
