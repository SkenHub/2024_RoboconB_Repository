#include <SoftwareSerial.h> 
#include "IM920_name.h"

SoftwareSerial IM920Serial(8, 9);  //受信 RX をピン 8、送信 TX をピン 9 に割り当て
ConvertIntFloat convert;
IM920COM im920;

void IM920write(uint8_t* data,IM920Bytes bytes){
  IM920Serial.print(im920.TXDA);
  Serial.print(im920.TXDA);
  String tx_data;
  for(int i; i<bytes*8; i++){
    tx_data = IM920Format(data[i]);
    IM920Serial.print(tx_data);
    Serial.print(data[i],HEX);
  }
  IM920Serial.print(im920.finish);
  Serial.print(im920.finish);
}

void setup() {
  IM920Serial.begin(19200);  //ソフトウェアシリアル開始IM920とは19200を指定
  Serial.begin(19200);
}

void loop() {
  long int num[2] = {4687,4567};
  uint8_t tx_data[8];
  for(int i=0; i<2; i++){
    convert.int_val = num[i];
    for(int j=0; j<4; j++){
      tx_data[i*4+j] = convert.uint8_val[j];
    }
  }
  IM920write(tx_data,Bytes8);
  delay(100);
}
