#include <SoftwareSerial.h>
#include <Servo.h>
#include "IM920.h"

SoftwareSerial IM920Serial(9,10);
ConvertIntFloat convert;
IM920 im920(IM920Serial);

uint8_t tx_data[8] = {0xFF,0,0,0,0,0xA0,0,0};
uint8_t rx_data[8] = {0,0,0,0,0,0,0,0};

constexpr byte S1 = 6;//Servo1のピン番号を記入
float ServoDeg = 0.00;//Servo角度制御用変数の初期化
float ServoLimitDeg = 180.00;//Servo最大角
Servo servo;

float R_Mtr = 0;//左モーター
float L_Mtr = 0;//右モーター

constexpr byte Right_Motor_P1 = 12;
constexpr byte Right_Motor_P2 = 13;
constexpr byte Left_Motor_P1 = 14;
constexpr byte Left_Motor_P2 = 15;

void setup() {
  Serial.begin(115200);

  Serial.println("Hello");

  servo.attach(S1,510,2400);
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
  //Serial.println(firstByte,HEX);

  R_Mtr=0;
  L_Mtr=0;
  if(firstByte&0x01){
    R_Mtr += 125;
    L_Mtr += 125;
  }
  if(firstByte&0x02){
    R_Mtr -= 125;
    L_Mtr -= 125;
  }
  if(firstByte&0x04){
    R_Mtr -= 125;
    L_Mtr += 125;
  }
  if(firstByte&0x08){
    R_Mtr += 125;
    L_Mtr -= 125;
  }

  Serial.print(R_Mtr);
  Serial.print(" ");
  Serial.println(L_Mtr);
  

  analogWrite(Right_Motor_P1,(R_Mtr>0)?R_Mtr:0);
  analogWrite(Right_Motor_P2,(R_Mtr<0)?-1*R_Mtr:0);
  analogWrite(Left_Motor_P1,(L_Mtr>0)?L_Mtr:0);
  analogWrite(Left_Motor_P2,(L_Mtr<0)?-1*L_Mtr:0);
  
  if(firstByte&0x10){//Lトリガー検知
    if(ServoDeg < ServoLimitDeg){
      ServoDeg = ServoDeg + 0.07;}
    //Serial.println(ServoDeg); //ServoDeg度に回す
  }
  if(firstByte&0x20){//Rトリガー検知
    if(ServoDeg > 0.00){
      ServoDeg = ServoDeg - 0.07;}
  } 
  //Serial.println(ServoDeg);
  servo.write(ServoDeg);
  
  /*
  if(bitdata[0] = 1){
    Left_Motor_muki = 1;
    Right_Motor_muki = 1;
    Left_Motor_Output = 200.0;
    Right_Motor_Output = 200.0;
  }

  if(bitdata[1] = 1){
    Left_Motor_muki = -1;
    Right_Motor_muki = -1;
    Left_Motor_Output = 200.0;
    Right_Motor_Output = 200.0;
  } 

  if(bitdata[2] = 1){
    Left_Motor_muki = -1;
    Right_Motor_muki = 1;
    Left_Motor_Output = 200.0;
    Right_Motor_Output = 200.0;
  }

  if(bitdata[3] = 1){  
    Left_Motor_muki = 1;
    Right_Motor_muki = -1;
    Left_Motor_Output = 200.0;
    Right_Motor_Output = 200.0;
  }

  if(bitdata[0] = 0 , bitdata[1] = 0 , bitdata[2] = 0 , bitdata[3] = 0){
    Left_Motor_muki = 0;
    Right_Motor_muki = 0;
  }

  if(bitdata[6] = 1){
    Left_Motor_Output = Left_Motor_Output * 1.27;
    Right_Motor_Output = Right_Motor_Output * 1.27;
  }

  if(bitdata[7] = 1){
    Left_Motor_Output = Left_Motor_Output * 0.7;
    Right_Motor_Output = Right_Motor_Output * 0.7;
  } 



  if(Right_Motor_muki > 0){
    analogWrite(Right_Motor_P1,Right_Motor_Output);
    analogWrite(Right_Motor_P2,0);
  }
  else if(Right_Motor_muki < 0){
    analogWrite(Right_Motor_P1,0);
    analogWrite(Right_Motor_P2,Right_Motor_Output);
  }
  else{
    analogWrite(Right_Motor_P1,0);
    analogWrite(Right_Motor_P2,0);
  }
  if(Left_Motor_muki > 0){
    analogWrite(Left_Motor_P1,Left_Motor_Output);
    analogWrite(Left_Motor_P2,0);
  }
  else if(Left_Motor_muki < 0){
    analogWrite(Left_Motor_P1,0);
    analogWrite(Left_Motor_P2,Left_Motor_Output);
  }
  else{
    analogWrite(Left_Motor_P1,0);
    analogWrite(Left_Motor_P2,0);
  }

//Servo_move
  if(bitdata[4] == 1){//Lトリガー検知
    if(ServoDeg < ServoLimitDeg){
      ServoDeg = ServoDeg + 0.07;}

    servo.write(ServoDeg);
    //Serial.println(ServoDeg); //ServoDeg度に回す
  }

  if(bitdata[5] == 1){//Rトリガー検知
    if(ServoDeg > 0.00){
      ServoDeg = ServoDeg - 0.07;}

    servo.write(ServoDeg);
    //Serial.println(ServoDeg);
  }
  */

  //Serial.print("Left_motor");
  //Serial.println(Left_Motor_Output);
  //Serial.print("Right_motor");
  //Serial.println(Right_Motor_Output);
  //Serial.print(Left_Motor_muki);
  //Serial.println(Right_Motor_muki); 
  /*
  Serial.print(bitdata[0]);
  Serial.print(bitdata[1]);
  Serial.print(bitdata[2]);
  Serial.print(bitdata[3]);
  Serial.print(bitdata[4]);
  Serial.println(bitdata[5]);
  */
}