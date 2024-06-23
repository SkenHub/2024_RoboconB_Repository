#include <SoftwareSerial.h>
#include <Servo.h>
#include "IM920.h"

SoftwareSerial IM920Serial(8,9);
ConvertIntFloat convert;
IM920 im920(IM920Serial);

uint8_t tx_data[8] = {0xFF,0,0,0,0,0xA0,0,0};
uint8_t rx_data[8] = {0,0,0,0,0,0,0,0};

constexpr byte S1 = 9;//Servo1のピン番号を記入
float ServoDeg = 0.00;//Servo角度制御用変数の初期化
float ServoLimitDeg = 130.00;//Servo最大角
Servo servo;
uint8_t data[6];

float Right_Motor_Output = 0;//左モーター
float Left_Motor_Output = 0;//右モーター

constexpr byte Right_Motor_P1 = 12;
constexpr byte Right_Motor_P2 = 13;
constexpr int Right_Motor_PWM =6;
constexpr byte Left_Motor_P1 = 14;
constexpr byte Left_Motor_P2 = 2;
constexpr int Left_Motor_PWM =12;


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
  long int rx[2];
  uint16_t NN;
  im920.read(&NN,rx_data,Bytes8);

  byte firstByte = rx_data[0];
  int bitdata[8];
  for(int y=0; y<7; y++){
    bitdata[y] = (firstByte >> y) & 0b00000001;
  }

  Serial.println(rx_data[0],BIN); 



  if(bitdata[0] = 1){
    Left_Motor_Output = 200.0;
    if(bitdata[2] = 1){
      Left_Motor_Output = -200.0;
    }
  }

  else if(bitdata[0] = 0){
    Left_Motor_Output = 0.0;
  }

  if(bitdata[1] = 1){
    Right_Motor_Output = 200.0;
    if(bitdata[3] = 1){
      Right_Motor_Output = -200.0;
    }
  }

  else if(bitdata[1] = 0){
    Right_Motor_Output = 0.0;
  }

  if(bitdata[4] = 1){
    Left_Motor_Output = Left_Motor_Output * 1.3;
    Right_Motor_Output = Right_Motor_Output * 1.3;
  }

  if(bitdata[5] = 1){
    Left_Motor_Output = Left_Motor_Output * 0.7;
    Right_Motor_Output = Right_Motor_Output * 0.7;
  }  


  if(Right_Motor_Output > 0){
    analogWrite(Right_Motor_P1,0);
    analogWrite(Right_Motor_P2,Right_Motor_Output);
  }

  else if(Right_Motor_Output < 0){
    analogWrite(Right_Motor_P1,Right_Motor_Output);
    analogWrite(Right_Motor_P2,0);
  }

  else if(Right_Motor_Output == 0) {
    analogWrite(Right_Motor_P1,0);
    analogWrite(Right_Motor_P2,0);
  }

  if(Left_Motor_Output > 0){
    analogWrite(Left_Motor_P1,Left_Motor_Output);
    analogWrite(Left_Motor_P2,0);
  }

  else if(Left_Motor_Output < 0){
    analogWrite(Left_Motor_P1,0);
    analogWrite(Left_Motor_P2,Left_Motor_Output);
  }

  else if(Left_Motor_Output == 0){
    analogWrite(Left_Motor_P1,0);
    analogWrite(Left_Motor_P2,0);
  }





//Servo_move
  if(bitdata[6] == 1){//Lトリガー検知
    if(ServoDeg < ServoLimitDeg){
      ServoDeg = ServoDeg + 0.5;}

    servo.write(ServoDeg); //ServoDeg度に回す
  }

  if(bitdata[7] == 1){//Rトリガー検知
    if(ServoDeg > 0.00){
      ServoDeg = ServoDeg - 0.5;}

  servo.write(ServoDeg);
  }



  Serial.print("Left_motor");
  Serial.println(Left_Motor_Output);
  Serial.print("Right_motor");
  Serial.println(Right_Motor_Output);

 

}