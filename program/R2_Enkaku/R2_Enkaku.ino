#include <SoftwareSerial.h>
#include <Servo.h>
#include "IM920.h"

SoftwareSerial IM920Serial(9,10);
ConvertIntFloat convert;
IM920 im920(IM920Serial);

//uint8_t tx_data[8] = {0xFF,0,0,0,0,0xA0,0,0};
uint8_t rx_data[8] = {0,0,0,0,0,0,0,0};

constexpr byte S1 = 6;//Servo1のピン番号を記入
constexpr byte S2 = 5;
float ServoDeg;// = 180.00;//Servo角度制御用変数の初期化
float servo_ht = 60.00;
float ServoLimitDeg = 180.00;//Servo最大角
Servo servo;
Servo servo2;
float R_Mtr = 0;//左モーター
float L_Mtr = 0;//右モーター
int sarvo_kiri = 0;
constexpr byte Right_Motor_P1 = A0;
constexpr byte Right_Motor_P2 = A1;
constexpr byte Left_Motor_P1 = A4;
constexpr byte Left_Motor_P2 = A5;

void setup() {
  Serial.begin(115200);
  //Serial.println("Hello");
  servo.attach(S1);//,510,2400);
  servo2.attach(S2);//,510,2400);
  im920.init();
  pinMode(Right_Motor_P1,OUTPUT);
  pinMode(Right_Motor_P2,OUTPUT);
  pinMode(Left_Motor_P1,OUTPUT);
  pinMode(Left_Motor_P2,OUTPUT);
}

void loop() {
  
//IM920との通信
  uint16_t NN;
  im920.read(&NN,rx_data,Bytes8);
  byte firstByte = rx_data[0];
  Serial.println(firstByte,HEX);
  
  R_Mtr=0;
  L_Mtr=0;
  if(firstByte&0x01){
    R_Mtr -= 200;
    L_Mtr -= 200;
  }
  if(firstByte&0x02){
    R_Mtr += 200;
    L_Mtr += 200;
  }
  if(firstByte&0x04){
    R_Mtr += 200;
    L_Mtr -= 200;
  }
  if(firstByte&0x08){
    R_Mtr -= 200;
    L_Mtr += 200;
  }
  if(firstByte&0x40){
    R_Mtr *= -1;
   L_Mtr *= -1; 
  } 
  if(firstByte&0x80){
    servo_ht = 180.00;
    servo2.write(servo_ht);
  }
  else{
    servo_ht = 150.00;
    servo2.write(servo_ht);
  }
  //Serial.println(servo_ht);
  //Serial.print(R_Mtr);
  //Serial.print(" ");
  //Serial.println(L_Mtr);
  if(firstByte&0x0F){
    analogWrite(Right_Motor_P1,(R_Mtr>0)?R_Mtr:0);
    analogWrite(Right_Motor_P2,(R_Mtr<0)?-1*R_Mtr:0);
    analogWrite(Left_Motor_P1,(L_Mtr>0)?L_Mtr:0);
    analogWrite(Left_Motor_P2,(L_Mtr<0)?-1*L_Mtr:0);
  }else{
    digitalWrite(Right_Motor_P1,HIGH);
    digitalWrite(Right_Motor_P2,HIGH);
    digitalWrite(Left_Motor_P1,HIGH);
    digitalWrite(Left_Motor_P2,HIGH);
  }
  
  if(firstByte&0x10){//Lトリガー検知
    /*if(ServoDeg < ServoLimitDeg){
      ServoDeg += 0.1;}*/
    ServoDeg = 150.00;
  }
  else{
    ServoDeg = 0.00;
  }
  /*if(firstByte&0x20){//Rトリガー検知
    if(ServoDeg > 0.00){
      ServoDeg -= 0.1;}
  } */
  //Serial.println(ServoDeg);
  //Serial.println(servo_ht);
  servo.write(ServoDeg);
}