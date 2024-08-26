#include <SoftwareSerial.h>
#include "IM920.h"

SoftwareSerial IM920Serial(22,23);  //受信 RX をピン 8、送信 TX をピン 9 に割り当て
IM920 im920(IM920Serial);
uint8_t tx_data[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
/*
const int switch1 = 2;//PushなSWのピン番号
const int switch2 = 3;
const int switch3 = 4;
const int switch4 = 5;
const int switch5 = 6;
const int switch6 = 7;
const int switch7 = 6;
const int switch8 = 7;
*/
//const int sw[8] = {2,3,4,5,6,7,0,0};
const int sw[11] = {35,26,33,25,17,18,5,19,34,32,16}; 

/*
byte switchState1 = 0;//押されているか否かを定義
byte switchState2 = 0;
byte switchState3 = 0;
byte switchState4 = 0;
byte switchState5 = 0;
byte switchState6 = 0;
*/
uint8_t switchState = 0;
uint8_t switchState2 = 0;

void setup() {
  for(int i=0; i<11; i++){
    pinMode(sw[i],INPUT_PULLUP);//ピン定義
  }
  Serial.begin(19200);
  im920.init();
}

void loop() {
  switchState = 0;
  switchState2 = 0;
  for(int i=0; i<8; i++) switchState |= ((digitalRead(sw[i]))? 0 : 1) << i;
  for(int i=0; i<3; i++) switchState2 |= ((digitalRead(sw[i+8]))? 0 : 1) << i;
  int kae = 0;
  kae = digitalRead(sw[10]);
  tx_data[0] = switchState;
  tx_data[1] = switchState2;
  //Serial.println(sw[5]);
  if(kae == 0){
    im920.write(tx_data,Bytes8,0002);}
  else{
    im920.write(tx_data,Bytes8,0003);}

  for(int i=0; i<11; i++){
    Serial.print(tx_data[i],HEX);
    //Serial.print(kae);
    Serial.print(" ");
  }
  //Serial.println(kae);
  Serial.println();
/*
byte send_data = 0b00000000;

switchState1 = (digitalRead(switch1) == 1 ) ? 0 : 1;//INPUT_PULLUPだと押されている時に0になるのがキモいからそれを反転させてる
switchState2 = (digitalRead(switch2) == 1 ) ? 0 : 1;
switchState3 = (digitalRead(switch3) == 1 ) ? 0 : 1;
switchState4 = (digitalRead(switch4) == 1 ) ? 0 : 1;
switchState5 = (digitalRead(switch5) == 1 ) ? 0 : 1;
switchState6 = (digitalRead(switch6) == 1 ) ? 0 : 1;


  if(switchState1 == 1){//全進B
    send_data = send_data | 0b00000011;
    Serial.print("up");
  }
  if(switchState2 == 1){//右B
    send_data = send_data | 0b00000001;
  }
  if(switchState3 == 1){//左B
    send_data = send_data | 0b00000010;
  }


 

  if(switchState4 == 1){

    if( switchState3 == 1){//後退+左B
      send_data = send_data | 0b00001010;
      Serial.print("Left");

    }else{

      if(switchState2 == 1){//後退+右B
        send_data = send_data | 0b00000101;
        Serial.print("Right");

      }else{//後退B
        send_data = send_data | 0b00001111;
        Serial.print("down");

      }
    }    
  }
  if(switchState5 == 1){//Servo+
    send_data = send_data | 0b00100000;
    Serial.print("L");
  }else{
    if(switchState6 == 1){//Servo-
      send_data = send_data | 0b00010000;
      Serial.print("R");
    }
  }


Serial.println(send_data,HEX);

//この辺は要調整
uint8_t tx_data[8] = {send_data,send_data,send_data,send_data,send_data,send_data,send_data,send_data};
//uint8_t tx_data[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

im920.write(tx_data,Bytes8);
//im920.write(tx_data,send_data);

//delay(92);
*/
}
