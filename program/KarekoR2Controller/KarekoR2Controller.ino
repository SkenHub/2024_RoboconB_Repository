#include <SoftwareSerial.h> 
#include "IM920.h"

SoftwareSerial IM920Serial(8, 9);  //受信 RX をピン 8、送信 TX をピン 9 に割り当て
IM920 im920(IM920Serial);
uint8_t tx_data[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

float Left_X,Left_Y,Right_X,Right_Y,Left_1,Left_2,Right_1,Right_2;

const int switch1 = 2;
const int switch2 = 3;
const int switch3 = 4;
const int switch4 = 5;
const int switch5 = 6;
const int switch6 = 7;

byte switchState1 = 0;
byte switchState2 = 0;
byte switchState3 = 0;
byte switchState4 = 0;
byte switchState5 = 0;
byte switchState6 = 0;

void setup() {

  pinMode(switch1,INPUT_PULLUP);
  pinMode(switch2,INPUT_PULLUP);
  pinMode(switch3,INPUT_PULLUP);
  pinMode(switch4,INPUT_PULLUP);
  pinMode(switch5,INPUT_PULLUP);
  pinMode(switch6,INPUT_PULLUP);
  
  Serial.begin(19200);
  im920.init();
  
}

void loop() {
byte send_data = 0b00000000;

switchState1 = (digitalRead(switch1) == 1 ) ? 0 : 1;
switchState2 = (digitalRead(switch2) == 1 ) ? 0 : 1;
switchState3 = (digitalRead(switch3) == 1 ) ? 0 : 1;
switchState4 = (digitalRead(switch4) == 1 ) ? 0 : 1;
switchState5 = (digitalRead(switch5) == 1 ) ? 0 : 1;
switchState6 = (digitalRead(switch6) == 1 ) ? 0 : 1;


  if(switchState1 == 1){
    send_data = send_data | 0b00000011;
    Serial.print("up");
  }
  if(switchState2 == 1){
    send_data = send_data | 0b00000001;
  }
  if(switchState3 == 1){
    send_data = send_data | 0b00000010;
  }


 

  if(switchState4 == 1){

    if( switchState3 == 1){
      send_data = send_data | 0b00001010;
      Serial.print("Left");

    }else{

      if(switchState2 == 1){
        send_data = send_data | 0b00000101;
        Serial.print("Right");

      }else{
        send_data = send_data | 0b00001111;
        Serial.print("down");

      }
    }    
  }
  if(switchState5 == 1){
    send_data = send_data | 0b00100000;
    Serial.print("L2");
  }else{
    if(switchState6 == 1){
      send_data = send_data | 0b00010000;
      Serial.print("R2");
    }
  }


Serial.println(send_data);

//この辺は要調整
uint8_t tx_data[8] = {send_data,send_data,send_data,send_data,send_data,send_data,send_data,send_data};
im920.write(tx_data,Bytes8);
}
