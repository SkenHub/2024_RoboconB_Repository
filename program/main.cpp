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

CanData cdata;
uint8_t sdata[8];
Gpio sw;
Encoder enc;
Encoder_data enc_data;

short power = 0;
int powerp = 0;
int speed;
int keep[2];
int count;
short out;
double deg;
double deg_fast;
double deg_stop;

double rps,rps_max;
double rps_min;

double rps_shoot;
double deg_shoot;
bool shoot;

int rpm_rbms;



double rps_rbms,rps_rbms_max;
double rps_rbms_min;
double deg_rbms;

int deg_rbms_count_now;
int deg_rbms_count_1;
double deg_count;
double deg_count_1;
double rps_count;

double trc;


Pid pid_control;
double target = 0;

double rps_line;
double deg_line;
bool line;




bool brake;


int attack = 0;//‘Å‚¿”ò‚Î‚·‚â‚Â
bool under = 0;//‰º“Š‚°

void func(void)
{
	enc.interrupt(&enc_data);

	deg = enc_data.deg;
	rps = enc_data.rps;
	if(rps>rps_max){
		rps_max = rps;
		deg_fast = deg;
	}

	if(rps<rps_min){
		rps_min = rps;
	}



	if((cdata.rx_data[2]*256 + cdata.rx_data[3]) <= 30000){
		rpm_rbms = cdata.rx_data[2]*256 + cdata.rx_data[3];
	}else{
		rpm_rbms = cdata.rx_data[2]*256 + cdata.rx_data[3]-65536;
	}
	rps_rbms = double(rpm_rbms)/(60*36);




	deg_rbms += rps_rbms*360/(1000);

	if(rps_rbms>rps_rbms_max){
		rps_rbms_max = rps_rbms;
	}

	if(rps_rbms<rps_rbms_min){
		rps_rbms_min = rps_rbms;
	}

	trc = cdata.rx_data[4]*256 + cdata.rx_data[5];



	powerp = pid_control.control(target,rps,1);
	if(powerp>10000){
		powerp = 10000;
	}else if(powerp < -10000){
			powerp = -10000;
	}


	deg_rbms_count_1 = deg_rbms_count_now;
	deg_rbms_count_now = cdata.rx_data[0]*256 + cdata.rx_data[1];

	if((deg_rbms_count_now >= 0 && deg_rbms_count_now <= 2700) && (deg_rbms_count_1 <= 8191 && deg_rbms_count_1 >= 5192)){
		count++;
	}else if((deg_rbms_count_now >= 5191 && deg_rbms_count_now <= 8191) && (deg_rbms_count_1 <= 2700 && deg_rbms_count_1 >= 0)){
		count--;
	}


	deg_count_1 = deg_count;
	deg_count = count*10 + (double(deg_rbms_count_now)*360/(8192*36));
	rps_count = (deg_count - deg_count_1)*1000/(360);



}



int main(void)
{
	sken_system.init();
	sw.init(C13,INPUT_PULLUP);
	sken_system.startCanCommunicate(B13,B12,CAN_2);
	sken_system.addCanRceiveInterruptFunc(CAN_2,&cdata);
	sken_system.addTimerInterruptFunc(func,0,1);
	pid_control.setGain(10000,0,0);


	enc.init(A0,A1,TIMER2);


	while(true){



		if(!sw.read()){
			/////////////////////////////////////////////

			if(brake == 0){
				if(deg<(720)){
					target = 3;
					power = powerp;

				}else{
					brake = 1;
					target = -6;
					pid_control.reset();
				}


			}else if(rps>=0){
					power = powerp;

			}else{
				power = powerp;
				target = 0;
			}





		}else if(brake == 0){
			power = 0;
		}else{
			power = 0;

		}
		sdata[0] = (power >> 8);
		sdata[1] = power;
		sdata[2] = 0;
		sdata[3] = 0;
		sdata[4] = 0;
		sdata[5] = 0;
		sdata[6] = 0;
		sdata[7] = 0;
		sken_system.canTransmit(CAN_2,0x200,sdata,8);//

		pid_control.reset();

		if(deg > 1440 && shoot == 0){
			deg_shoot = deg;
			rps_shoot = rps;
			shoot = 1;
		}

		if (rps>=7 && line == 0){
			deg_line = deg;
			rps_line = rps;
			line = 1;
		}




	}
}

