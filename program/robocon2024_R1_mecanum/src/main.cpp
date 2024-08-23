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
#include "math.h"

Motor mecanum[4];
Encoder encoder[4];
Encoder_data encoder_data[4];
Pid pid[4];
const float wheel_diameter = 100.0;
const float x_side=475,y_side=350;
float target_velo[3];
float robot_velo[3],robot_rad;
float target_mecanum[4];
float out[4];

Uart serial;
uint8_t rx_data[17],get_data[16];
ConvertIntFloat convert;

void get_target(){
	for(int i=0; i<17; i++){
		if(rx_data[i] == 0xA0){
			for(int j=0; j<16; j++) get_data[j] = rx_data[(i+1+j)%17];
			break;
		}
	}

	for(int i=0; i<3; i++){
		for(int j=0; j<4; j++) convert.uint8_val[j] = get_data[i*4+j];
		target_velo[i] = convert.float_val;
	}

	for(int i=0; i<4; i++) convert.uint8_val[i] = get_data[12+i];
	robot_rad = convert.float_val*(2*PI/360.0);

}

void caluc_mecanum_velo(){
	robot_velo[0] = target_velo[0]*cos(robot_rad) - target_velo[1]*sin(robot_rad);
	robot_velo[1] = target_velo[0]*sin(robot_rad) + target_velo[1]*cos(robot_rad);
	robot_velo[2] = target_velo[2]*(2*PI/360.0);

	target_mecanum[0] = (-robot_velo[0] + robot_velo[1] + (x_side+y_side)*robot_velo[2])/(wheel_diameter*PI);
	target_mecanum[1] = ( robot_velo[0] + robot_velo[1] - (x_side+y_side)*robot_velo[2])/(wheel_diameter*PI);
	target_mecanum[2] = (-robot_velo[0] + robot_velo[1] - (x_side+y_side)*robot_velo[2])/(wheel_diameter*PI);
	target_mecanum[3] = ( robot_velo[0] + robot_velo[1] + (x_side+y_side)*robot_velo[2])/(wheel_diameter*PI);
}

void pid_control(){
	for(int i=0; i<4; i++) out[i] = pid[i].control(target_mecanum[i],encoder_data[i].rps,1);
	if(target_velo[0]==0 && target_velo[1]==0 && target_velo[2]==0){
		for(int i=0; i<4; i++) pid[i].reset();
	}
	for(int i=0; i<4; i++) mecanum[i].write(out[i]);
}

void interrupt(){
	get_target();
	for(int i=0; i<4; i++) encoder[i].interrupt(&encoder_data[i]);
	caluc_mecanum_velo();
	pid_control();
}

int main(void){
	sken_system.init();

	serial.init(C10,C11,SERIAL4,115200);
	mecanum[0].init(Apin,B15,TIMER12 ,CH2);
	mecanum[0].init(Bpin,B14,TIMER12 ,CH1);
	mecanum[1].init(Apin,A11,TIMER1 ,CH4);
	mecanum[1].init(Bpin,A8 ,TIMER1 ,CH1);
	mecanum[2].init(Apin,A7 ,TIMER14,CH1);
	mecanum[2].init(Bpin,A6 ,TIMER13,CH1);
	mecanum[3].init(Apin,B9 ,TIMER11,CH1);
	mecanum[3].init(Bpin,B8 ,TIMER10,CH1);
	encoder[0].init(A0,A1,TIMER5,wheel_diameter);
	encoder[1].init(A5,B3,TIMER2,wheel_diameter);
	encoder[2].init(B6,B7,TIMER4,wheel_diameter);
	encoder[3].init(C6,C7,TIMER3,wheel_diameter);
	pid[0].setGain(10,1,0.1,20);
	pid[1].setGain(10,1,0.1,20);
	pid[2].setGain(10,1,0.1,20);
	pid[3].setGain(10,1,0.1,20);

	sken_system.addTimerInterruptFunc(interrupt,0,1);
	serial.startDmaRead(rx_data,17);

	while(true){

	}
}
