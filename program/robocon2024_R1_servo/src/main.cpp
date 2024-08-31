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

Uart serial;
uint8_t serial_data;

float min_deg[3] = {40,50,0};
float max_deg[3] = {95,100,50};
float rate = 0.5;

float now_deg[3] = {min_deg[0],min_deg[1],min_deg[2]};

RcPwm Enkaku_servo,cra_servo[2];

void interrupt(){
	if((serial_data&0x04) || now_deg[2]>=max_deg[2]){
		now_deg[0] += (now_deg[0]<max_deg[0])? rate : 0;
	}else{
		now_deg[0] -= (now_deg[0]>min_deg[0])? rate : 0;
	}

	if((serial_data&0x08) && !(serial_data&0x04)){
		now_deg[1] += (now_deg[1]<max_deg[1])? rate : 0;
	}else{
		now_deg[1] -= (now_deg[1]>min_deg[1])? rate : 0;
	}

	if((serial_data&0x08) && !(serial_data&0x04)){
		now_deg[2] += (now_deg[2]<max_deg[2])? rate : 0;
	}else{
		now_deg[2] -= (now_deg[2]>min_deg[2])? rate : 0;
	}

	Enkaku_servo.turn(now_deg[0]);
	cra_servo[0].turn(now_deg[1]);
	cra_servo[1].turn(now_deg[2]);
}

int main(void){
	sken_system.init();

	serial.init(C10,C11,SERIAL4,115200);
	serial.startDmaRead(&serial_data,1);

	Enkaku_servo.init(B6,TIMER4,CH1);
	cra_servo[0].init(B7,TIMER4,CH2);
	cra_servo[1].init(B8,TIMER4,CH3);

	sken_system.addTimerInterruptFunc(interrupt,0,1);

	while(true){

	}
}
