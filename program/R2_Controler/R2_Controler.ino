#include <SoftwareSerial.h>
#include "IM920.h"

SoftwareSerial IM920Serial(22,23);  //受信 RX をピン 8、送信 TX をピン 9 に割り当て
IM920 im920(IM920Serial);
uint8_t tx_data[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
const int sw[11] = {35,26,33,25,17,18,5,19,34,32,16};
constexpr byte LED = 12;

uint8_t switchState = 0;
uint8_t switchState2 = 0;

void setup() {
  for(int i=0; i<11; i++){
    pinMode(sw[i],INPUT_PULLUP);//ピン定義
  }
  pinMode(LED,OUTPUT);
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
  if(switchState&0x40){
    digitalWrite(LED,HIGH);}
  else{
    digitalWrite(LED,LOW);}
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
}
