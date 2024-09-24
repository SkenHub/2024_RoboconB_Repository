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
#include <Math.h>

Uart serial;
uint8_t rx_data[14],RxData[14];
float velo[3];
ConvertIntFloat convert;

Uart cataprlt_serial;

Motor omni[3];
float target_rps[3];
float robot_param[2] = {90,450};
Encoder encoder[3];
Encoder_data encoder_data[3];
Pid pid[3];

void serial_get(){
	for(int i=0; i<13; i++){
		if(rx_data[i] == 0xA5){
			for(int j=0; j<14; j++) RxData[j] = rx_data[(i+j)%14];
			break;
		}
	}
	for(int i=0; i<3; i++){
		for(int j=0; j<4; j++)	convert.uint8_val[j] = RxData[(i*4)+j+1];
		velo[i] = convert.float_val * 20;
	}
}

float pwm[3],pid_pwm[3];
void omni_control(){
	float omega = velo[2] * PI /180.0;
	target_rps[0] = (-velo[0] + robot_param[1] * omega) / (2 * PI * robot_param[0]);
	target_rps[1] = (velo[0] / 2 - velo[1] * sqrt(3) / 2 + robot_param[1] * omega) / (2 * PI * robot_param[0]);
	target_rps[2] = (velo[0] / 2 + velo[1] * sqrt(3) / 2 + robot_param[1] * omega) / (2 * PI * robot_param[0]);

	//pwm[0] = (-velo[0]   +  velo[2]) / (2 * PI);
	//pwm[1] = (velo[0]/2 - velo[1]*sqrt(3)/2 +  velo[2]) / (2 * PI);
	//pwm[2] = (velo[0]/2 + velo[1]*sqrt(3)/2 +  velo[2]) / (2 * PI);

	for(int i=0; i<3; i++){
		pid_pwm[i] = pid[i].control(target_rps[i],encoder_data[i].rps,1);
		if(target_rps[i] == 0){
			pwm[i] = 0;
		}else{
			pwm[i] = pid_pwm[i];
		}

		omni[i].write(pwm[i]);
	}
}

void interrupt(){
	for(int i=0; i<3; i++) encoder[i].interrupt(&encoder_data[i]);
	serial_get();
	omni_control();
}

int main(void){
	sken_system.init();

	serial.init(C10,C11,SERIAL4,115200);
	serial.startDmaRead(rx_data,14);

	cataprlt_serial.init(A9,A10,SERIAL1,115200);

	omni[0].init(Apin,B15,TIMER12 ,CH2);
	omni[0].init(Bpin,B14,TIMER12 ,CH1);
	omni[1].init(Apin,A11,TIMER1 ,CH4);
	omni[1].init(Bpin,A8 ,TIMER1 ,CH1);
	omni[2].init(Apin,A6 ,TIMER13,CH1);
	omni[2].init(Bpin,A7 ,TIMER14,CH1);
	encoder[0].init(A0,A1,TIMER5);
	encoder[1].init(A5,B3,TIMER2);
	encoder[2].init(B6,B7,TIMER4);

	pid[0].setGain(1.5,0,0.1,50);
	pid[1].setGain(1.5,0,0.1,50);
	pid[2].setGain(1.5,0,0.1,50);

	sken_system.addTimerInterruptFunc(interrupt,0,1);

	while(true){
		cataprlt_serial.write(&RxData[13],1);
	}
}
