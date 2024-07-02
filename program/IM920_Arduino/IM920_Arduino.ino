#include <SoftwareSerial.h> 
#include "IM920.h"

SoftwareSerial IM920Serial(8, 9);  //受信 RX をピン 8、送信 TX をピン 9 に割り当て
ConvertIntFloat convert;
IM920 im920(IM920Serial);
uint8_t tx_data[8] = {0xFF,0,0,0x10,0,0xA0,0,0};
uint8_t rx_data[8] = {0,0,0,0,0,0,0,0};

void setup() {
  Serial.begin(19200);
  im920.init();
}

void loop() {
  long int rx[2];
  uint16_t NN;
  //im920.read(&NN,rx_data,Bytes8);
  for(int i=0; i<2; i++){
    for(int j=0; j<4; j++){
      convert.uint8_val[j] = rx_data[i*4+j];
    }
    rx[i] = convert.float_val;
  }
  for(int i=0; i<8; i++){
    Serial.print(tx_data[i]);
    Serial.print("  ");
  }
  Serial.println();
  im920.write(tx_data,Bytes8);
  //delay(100);
}
