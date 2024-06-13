//#include <dummy.h>
#include <Ps3Controller.h>
#include <ESP32Servo.h>
//ピン番号にint型は贅沢かも?
constexpr byte S1 = 5;//Servo1のピン番号を記入(Lトリガーで制御)
float ServoDeg = 0.00;//Servo角度制御用変数の初期化
float ServoLimitDeg = 130.00;//Servo最大角
Servo servo;
uint8_t data[6];
float Left_X,Left_Y,Right_X,Right_Y,Left_1,Left_2,Right_1,Right_2;
////////////////////////////////////////////////////////////////////////////////////////
//doubleから変えてみる(もしかしたら治るかも?)
float Right_Motor_Output = 0;//左モーター
float Left_Motor_Output = 0;//右モーター

////////////////////////////////////////////////////////////////////////////////////////
 
constexpr byte Right_Motor_P1 = 12;
constexpr byte Right_Motor_P2 = 13;
constexpr int Right_Motor_PWM =A10;//4

constexpr byte Left_Motor_P1 = 32;
constexpr byte Left_Motor_P2 = 33;
constexpr int Left_Motor_PWM =A7;//2

//////

void setup() {
  Serial.begin(115200);
  Ps3.begin("00:1B:DC:0F:2B:91");
  Serial.println("Ready");

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////

ESP32PWM::allocateTimer(1);//魔法の呪文(これを入れるとなんかタイマーが干渉しなくなっていい感じ♪)

  servo.attach(S1,510,2400);
  pinMode(Right_Motor_P1,OUTPUT);
  pinMode(Right_Motor_P2,OUTPUT);
  ledcSetup(1,12800,8);

  pinMode(Left_Motor_P1,OUTPUT);
  pinMode(Left_Motor_P2,OUTPUT);
  ledcSetup(8,12800,8);

  ledcAttachPin(Right_Motor_PWM,1);
  ledcAttachPin(Left_Motor_PWM,8); 
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
    Serial.print(Right_Motor_Output);
    Serial.print(Left_Motor_Output);
    
  }

///////////////////////////////////////////////////////////////////////////////////////////

  /*if(Left_Y > 0){  
    if (Right_X == 0)
    {
      Left_Motor_Output = 255.0 * Left_Y / 128.0;
      Right_Motor_Output = 255.0 * Left_Y / 128.0; 
    }else if(Right_X < 0)
    {
      Left_Motor_Output = (255.0 * (Left_Y / 128.0)) - (255.0 * (abs(Right_X) / 128.0));
      Right_Motor_Output = 255.0 * Left_Y / 128.0;
    }else if(Right_X > 0)
    {
      Left_Motor_Output = 255.0 * Left_Y / 128.0;
      Right_Motor_Output = (255.0 * (Left_Y / 128.0)) - (255.0 * (Right_X / 127.0));//
    }
  }else if(Left_Y){
    if (Right_X == 0)
    {
      Left_Motor_Output = 255.0 * Left_Y / 127.0;
      Right_Motor_Output = 255.0 * Left_Y / 127.0; 
    }else if(Right_X < 0)
    {
      Left_Motor_Output = (255.0 * (Left_Y / 127.0)) + (255.0 * (abs(Right_X) / 128.0));
      Right_Motor_Output = 255.0 * Left_Y / 127.0;
    }else if(Right_X > 0)
    {
      Left_Motor_Output = 255.0 * Left_Y / 127.0;
      Right_Motor_Output = (255.0 * (Left_Y / 127.0)) + (255.0 * (abs(Right_X) / 127.0));//
    }
  }else{
    if (Right_X == 0)
    {
      Left_Motor_Output = 0;
      Right_Motor_Output = 0; 
    }else if(Right_X < 0)
    {
      Left_Motor_Output = -(255.0 * (abs(Right_X) / 128.0));
      Right_Motor_Output = (255.0 * (abs(Right_X) / 128.0)); 
    }else if(Right_X > 0)
    {
      Left_Motor_Output = (255.0 * (abs(Right_X) / 127.0));
      Right_Motor_Output = -(255.0 * (abs(Right_X) / 127.0)); 
    }    
  }*/
/*
  Serial.print(" ");
  Serial.print(Left_Motor_Output);
  Serial.print(" ");
  Serial.print(abs(Right_Motor_Output));
  Serial.println();*/

  if(Left_Y >= 0){  //いじりました
    Left_Motor_Output = 255.0 * (Left_Y / 128.0);
  }
  else if(Left_Y < 0){
    Left_Motor_Output = (255.0 * (Left_Y / 127.0));
  }
  if(Right_Y >= 0){
    Right_Motor_Output =255.0 * (Right_Y / 127.0);
  }
  else if(Right_Y < 0){
    Right_Motor_Output = (255.0 * (Right_Y / 127.0));
  }
  
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


  if(Right_Motor_Output > 0)
  {
    digitalWrite(Right_Motor_P1,LOW);
    digitalWrite(Right_Motor_P2,HIGH);
    //ledcWrite(0,abs(Right_Motor_Output));
    ledcWrite(0,(Right_Motor_Output));
    Serial.print("Right_M_P");
    Serial.println(Right_Motor_Output);
  }else if(Right_Motor_Output < 0)
  {
    digitalWrite(Right_Motor_P1,HIGH);
    digitalWrite(Right_Motor_P2,LOW);
    //ledcWrite(0,abs(Right_Motor_Output));
    ledcWrite(0,-(Right_Motor_Output));
    Serial.print("Right_M_M");
    Serial.println(Right_Motor_Output);
  }else if(Right_Motor_Output == 0)
  {
    digitalWrite(Right_Motor_P1,HIGH);
    digitalWrite(Right_Motor_P2,HIGH);
    ledcWrite(0,0);//stop
    //Serial.println(Right_Motor_Output);
  }
  if(Left_Motor_Output > 0)
  {
    digitalWrite(Left_Motor_P1,HIGH);
    digitalWrite(Left_Motor_P2,LOW);
    //ledcWrite(1,abs(Left_Motor_Output));
    ledcWrite(1,(Left_Motor_Output));
    Serial.print("Left_M_P");
    Serial.println(Left_Motor_Output);
  }else if(Left_Motor_Output < 0)
  {
    digitalWrite(Left_Motor_P1,LOW);
    digitalWrite(Left_Motor_P2,HIGH);
    //ledcWrite(1,abs(Left_Motor_Output));
    ledcWrite(1,-(Left_Motor_Output));
    Serial.print("Left_M_M");
  }else if(Left_Motor_Output == 0)
  {
    digitalWrite(Left_Motor_P1,HIGH);
    digitalWrite(Left_Motor_P2,HIGH);
    ledcWrite(1,0);
    //Serial.println(Left_Motor_Output);
  }


//Servo_move

if(Ps3.data.button.l2 == 1 ){//Lトリガー検知
   if(ServoDeg < ServoLimitDeg ){
       ServoDeg = ServoDeg + 0.5;}
      servo.write(ServoDeg); //ServoDeg度に回す
      Serial.println(ServoDeg);
      //Serial.println(Ps3.data.button.l2);
    }

if(Ps3.data.button.r2 == 1){//Rトリガー検知
       if(ServoDeg > 0.00){
       ServoDeg = ServoDeg - 0.5;}
       servo.write(ServoDeg);
       Serial.println(ServoDeg);
       //Serial.println(Ps3.data.button.r2);
  }



 /* Serial.print(Ps3.data.button.l2);//debug用にトリガーの出力をシリアルで垂れ流す
  Serial.print(":L2TR");
  Serial.print(Ps3.data.button.r2);
  Serial.print(":R2TR");
  Serial.println();*/
}