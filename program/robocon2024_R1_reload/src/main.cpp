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

Uart serial1,serial2;
uint8_t serial_data;

Motor mtr;

void interrupt(){
	if(serial_data&0x01){
		mtr.write(-30);
	}else if(serial_data&0x02){
		mtr.write(30);
	}else{
		mtr.write(0);
	}
}

int main(void){
	sken_system.init();

	serial1.init(C10,C11,SERIAL4,115200);
	serial1.startDmaRead(&serial_data,1);

	serial2.init(A9,A10,SERIAL1,115200);

	mtr.init(Apin,B15,TIMER12 ,CH2);
	mtr.init(Bpin,B14,TIMER12 ,CH1);

	sken_system.addTimerInterruptFunc(interrupt,0,1);

	while(true){
		serial2.write(&serial_data,1);
	}
}
