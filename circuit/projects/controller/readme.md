# 回路系のピン
## コントローラ  
buttonPin1 = 35; 
buttonPin2 = 26;
buttonPin3 = 33;
buttonPin4 = 25;
buttonPin5 = 17;
buttonPin6 = 18;
buttonPin7 = 5;
buttonPin8 = 19;
buttonPin9 = 34;
buttonPin10 = 32; 
buttonPin11 = 21;
IM920,RX:ESP32TX,23
IM920,TX:ESP32RX,22
LED1:GPIO13
LED2:GPIO14
LED3:GPIO27
const int sw[11] = {35,26,33,25,17,18,5,19,34,32,21}; 
# 過去のコントローラ基板のデータはcontroller-backupsの中にzipファイル形式であります．