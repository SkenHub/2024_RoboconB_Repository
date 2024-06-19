# IM920
通信モジュールIM920sLを使用するためのクラス.

# enum IM920Bytes
* Bytes8：8バイトの送受信のために使う
* Bytes16：16バイトの送受信のために使う
* Bytes24：24バイトの送受信のために使う
* Bytes32：32バイトの送受信のために使う

# union ConvertIntFloat
送受信データの整数値、実数値への変換に使う  
* long int int_val
* float float_val
* uint8_t uint8_val[4]

# class IM920
## IM920::IM920(SoftwareSerial& serial);
宣言時に設定済みのソフトウェアシリアルを与える

[パラメータ]
ソフトウェアシリアル

[サンプルコード]
オブジェクトを作成
```c++
#include <SoftwareSerial.h> 
#include "IM920.h"

SoftwareSerial IM920Serial(8, 9);  //受信 RX をピン 8、送信 TX をピン 9 に割り当て
IM920 im920(IM920Serial);

void setup(){

}

void loop(){

}
```

## IM920::init(Pin tx_pin,Pin rx_pin,UartNumber uart_num)
初期化関数  

[パラメータ]  
無し

[戻り値]  
無し  

[サンプルコード]  
初期化  
``` c++
#include <SoftwareSerial.h> 
#include "IM920.h"

SoftwareSerial IM920Serial(8, 9);  //受信 RX をピン 8、送信 TX をピン 9 に割り当て
IM920 im920(IM920Serial);

void setup() {
  im920.init();
}

void loop(){
    
}
```

## void IM920::read(uint16_t* rx_id,uint8_t* rx_data,IM920Bytes bytes)
受信関数  

[パラメータ]  
受信先のノード番号を格納するための変数のアドレス  
受信データを格納するための配列のアドレス  
受信データの大きさ  

[戻り値]  
なし  

[サンプルコード]  
受信データを8バイトで取得し、シリアルモニターに出力  
``` c++
#include <SoftwareSerial.h> 
#include "IM920.h"

SoftwareSerial IM920Serial(8, 9);  //受信 RX をピン 8、送信 TX をピン 9 に割り当て
IM920 im920(IM920Serial);
uint8_t rx_data[8] = {0,0,0,0,0,0,0,0};
uint16_t NN;

void setup() {
  Serial.begin(19200);
  im920.init();
}

void loop() {
  long int rx[2];
  im920.read(&NN,rx_data,Bytes8);
  for(int i=0; i<8; i++){
    Serial.print(rx_data[i]);
    Serial.print("  ");
  }
  Serial.println();
}
```

## void IM920::write(uint8_t* tx_data,IM920Bytes bytes,int node_num = 0x0000)
送信関数  

[パラメータ]
送信データ配列のアドレス  
送信データの大きさ  
送信先のノード番号（省略すると全てのノードに送信される）  

[戻り値]  
なし  

[サンプルコード]  
8バイトのデータを0.1秒周期で送信
``` c++
#include <SoftwareSerial.h> 
#include "IM920.h"

SoftwareSerial IM920Serial(8, 9);  //受信 RX をピン 8、送信 TX をピン 9 に割り当て
IM920 im920(IM920Serial);
uint8_t tx_data[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

void setup() {
  Serial.begin(19200);
  im920.init();
}

void loop() {
  im920.write(tx_data,Bytes8);
  delay(100);
}
```
