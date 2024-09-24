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
uint8_t rx_data;

Motor mtr;
Encoder encoder[2];
Encoder_data encoder_data[2];
Pid pid;
float target_rps = 5,out;


void catapult_control(){
	if((rx_data&0x01) && (encoder_data[1].deg < 240)){
		out += (out<30)? 0.01:0;
	}else{
		pid.reset();
		out = 0;
	}

	if((rx_data&0x02) && (encoder_data[1].deg > 50)){
		pid.reset();
		out = -10;
	}

	mtr.write(out);
}

void interrupt(){
	for(int i=0; i<2; i++) encoder[i].interrupt(&encoder_data[i]);
	catapult_control();
}

int main(void){
	sken_system.init();

	serial.init(C10,C11,SERIAL4,115200);
	serial.startDmaRead(&rx_data,1);

	mtr.init(Apin,B15,TIMER12,CH2);
	mtr.init(Bpin,B14,TIMER12,CH1);

	encoder[0].init(A0,A1,TIMER5);
	encoder[1].init(A5,B3,TIMER2);

	pid.setGain(0.1,5.0,0,20);

	sken_system.addTimerInterruptFunc(interrupt,0,1);

	while(true){
	}
}
