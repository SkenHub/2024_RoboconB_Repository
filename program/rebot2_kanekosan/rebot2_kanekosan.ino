#include <dummy.h>
#include <Ps3Controller.h>
#include <ESP32Servo.h>

const int S1 = 5;//Servo1のピン番号を記入(Lトリガーで制御)
int ServoDeg = 0;//Servo角度制御用変数の初期化
Servo servo;
 uint8_t data[6];
 double Left_X,Left_Y,Right_X,Right_Y,Left_1,Left_2,Right_1,Right_2;
 ////////////////////////////////////////////////////////////////////////////////////////

 double Right_Motor_Output = 0;//左モーター
 double Left_Motor_Output = 0;//右モーター

 ////////////////////////////////////////////////////////////////////////////////////////
 
 int Right_Motor_P1 = 12;
 int Right_Motor_P2 = 13;
 int Right_Motor_PWM =A10;//4

 int Left_Motor_P1 = 25;
 int Left_Motor_P2 = 26;
 int Left_Motor_PWM =A12;//2
//void servomt(){
//  if (Left_1 == 1) {
//    servo1.write(10);
// } else if (Left_2 == 1) {
//    servo1.write(90);
//  }
//  if (Left_3 == 1) {
//    servo2.write(0);
//  } else if (Left_4 == 1) {
//    servo2.write(80);
//  }  これは何?
//}



void setup() {
  Serial.begin(115200);
  Ps3.begin("e8:68:e7:31:03:5a");
  Serial.println("Ready");

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////

  pinMode(Right_Motor_P1,OUTPUT);
  pinMode(Right_Motor_P2,OUTPUT);
  ledcSetup(0,12800,8);

  pinMode(Left_Motor_P1,OUTPUT);
  pinMode(Left_Motor_P2,OUTPUT);
  ledcSetup(1,12800,8);

  ledcAttachPin(Right_Motor_PWM,0);
  ledcAttachPin(Left_Motor_PWM,1); 
      servo.attach(S1,510,2400);
}

void loop() {
  if (Ps3.isConnected()){
    /*Serial.print("LX=,");
    Serial.print(Ps3.data.analog.stick.lx);
    Serial.print(",LY=,");
    Serial.print(Ps3.data.analog.stick.ly);
    Serial.print(",RX=,");
    Serial.print(Ps3.data.analog.stick.rx);
    Serial.print(",RY=,");
    Serial.print(Ps3.data.analog.stick.ry);
    Serial.print(Ps3.data.button.ps);
    Serial.println();
    data[0]=0xa5;
    data[1]=0xa5;
    data[2]=Ps3.data.analog.stick.lx;
    data[3]=Ps3.data.analog.stick.ly;
    data[4]=Ps3.data.analog.stick.rx;
    data[5]=Ps3.data.analog.stick.ry;
    Serial.write(data,6);*/
    
    Left_X = Ps3.data.analog.stick.lx;
    Left_Y = -Ps3.data.analog.stick.ly;
    Right_X = Ps3.data.analog.stick.rx;
    Right_Y = -Ps3.data.analog.stick.ry;
    Left_1 = Ps3.data.button.l1;
    Left_2 = Ps3.data.button.l2;
    Right_1 = Ps3.data.button.r1;
    Right_2 = Ps3.data.button.r2;
    Serial.print(Left_X);
    Serial.print(" ");
    Serial.print(Left_Y);
    Serial.print(" ");
    Serial.print(Right_X);
    Serial.print(" ");
    Serial.print(Right_Y);
  }

///////////////////////////////////////////////////////////////////////////////////////////

  if(Left_Y > 0){  
    if (Right_X == 0)
    {
      Left_Motor_Output = 100.0 * Left_Y / 128.0;
      Right_Motor_Output = 100.0 * Left_Y / 128.0; 
    }else if(Right_X < 0)
    {
      Left_Motor_Output = (100.0 * (Left_Y / 128.0)) - (100.0 * (abs(Right_X) / 128.0));
      Right_Motor_Output = 100.0 * Left_Y / 128.0;
    }else if(Right_X > 0)
    {
      Left_Motor_Output = 100.0 * Left_Y / 128.0;
      Right_Motor_Output = (100.0 * (Left_Y / 128.0)) - (100.0 * (Right_X / 127.0));//
    }
  }else if(Left_Y){
    if (Right_X == 0)
    {
      Left_Motor_Output = 100.0 * Left_Y / 127.0;
      Right_Motor_Output = 100.0 * Left_Y / 127.0; 
    }else if(Right_X < 0)
    {
      Left_Motor_Output = (100.0 * (Left_Y / 127.0)) + (100.0 * (abs(Right_X) / 128.0));
      Right_Motor_Output = 100.0 * Left_Y / 127.0;
    }else if(Right_X > 0)
    {
      Left_Motor_Output = 100.0 * Left_Y / 127.0;
      Right_Motor_Output = (100.0 * (Left_Y / 127.0)) + (100.0 * (abs(Right_X) / 127.0));//
    }
  }else{
    if (Right_X == 0)
    {
      Left_Motor_Output = 0;
      Right_Motor_Output = 0; 
    }else if(Right_X < 0)
    {
      Left_Motor_Output = -(100.0 * (abs(Right_X) / 128.0));
      Right_Motor_Output = (100.0 * (abs(Right_X) / 128.0)); 
    }else if(Right_X > 0)
    {
      Left_Motor_Output = (100.0 * (abs(Right_X) / 127.0));
      Right_Motor_Output = -(100.0 * (abs(Right_X) / 127.0)); 
    }    
  }

  Serial.print(" ");
  Serial.print(Left_Motor_Output);
  Serial.print(" ");
  Serial.print(abs(Right_Motor_Output));
  Serial.println();


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
  if(Right_Motor_Output > 0)
  {
    digitalWrite(Right_Motor_P1,HIGH);
    digitalWrite(Right_Motor_P2,LOW);
    ledcWrite(0,abs(Right_Motor_Output));
  }else if(Right_Motor_Output < 0)
  {
    digitalWrite(Right_Motor_P1,LOW);
    digitalWrite(Right_Motor_P2,HIGH);
    ledcWrite(0,abs(Right_Motor_Output));
  }else if(Right_Motor_Output == 0)
  {
    digitalWrite(Right_Motor_P1,HIGH);
    digitalWrite(Right_Motor_P2,HIGH);
    ledcWrite(0,0);//stop
  }

  if(Left_Motor_Output > 0)
  {
    digitalWrite(Left_Motor_P1,HIGH);
    digitalWrite(Left_Motor_P2,LOW);
    ledcWrite(1,abs(Left_Motor_Output));
  }else if(Left_Motor_Output < 0)
  {
    digitalWrite(Left_Motor_P1,LOW);
    digitalWrite(Left_Motor_P2,HIGH);
    ledcWrite(1,abs(Left_Motor_Output));
  }else if(Left_Motor_Output == 0)
  {
    digitalWrite(Left_Motor_P1,HIGH);
    digitalWrite(Left_Motor_P2,HIGH);
    ledcWrite(1,0);
  }
//}



//Servo_move
  if(Ps3.data.button.l2 = 1 ){//Lトリガー検知
if(ServoDeg != 180 ){
  ServoDeg = ServoDeg + 1;}
       servo.write(ServoDeg); //ServoDeg度に回す
  }
  if(Ps3.data.button.r2 = 1){//Rトリガー検知
       if(ServoDeg != 0){
       ServoDeg = ServoDeg - 1;}
       servo.write(ServoDeg);
  }

  Serial.print(Ps3.data.button.l2);//debug用にトリガーの出力をシリアルで垂れ流す
  Serial.print(":L2TR");
  Serial.print(Ps3.data.button.l2);
  Serial.print(":R2TR");
  Serial.println();
}
