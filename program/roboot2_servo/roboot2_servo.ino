#include <dummy.h>
#include <Ps3Controller.h>
#include<ESP32Servo.h>



 uint8_t data[6];
 double Left_X,Left_Y,Right_X,Right_Y,Left_1,Left_2,Right_1,Right_2;

 ////////////////////////////////////////////////////////////////////////////////////////

 double Right_Motor_Output = 0;//左モーター
 double Left_Motor_Output = 0;//右モーター

 ////////////////////////////////////////////////////////////////////////////////////////
 
 int Right_Motor_P1 = 12;
 int Right_Motor_P2 = 14;
 int Right_Motor_PWM =27;//4

 int Left_Motor_P1 = 35;
 int Left_Motor_P2 = 32;
 int Left_Motor_PWM =33;//2

 double deg1 = 0;
 double deg2 = 0;

 int sv1 = 2; //サーボ1のピン
 int sv2 = 0; //サーボ2のピン

 Servo servo1;
 Servo servo2;

void setup() {
  Serial.begin(115200);
  Ps3.begin("E8:68:E7:31:17:1E");
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

  servo1.attach(sv1,510,2400);
  servo2.attach(sv2,510,2400);
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
    Serial.print(" ");
    Serial.print(Left_1);
    Serial.print(" ");
    Serial.print(Left_2);
    Serial.print(" ");
    Serial.print(Right_1);
    Serial.print(" ");
    Serial.print(Right_2);
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

////////////////////////////////////////////////////////////////////////////////

  if(Left_1 == 1){

    if(deg1<=180) deg1+=0.1;
    servo1.write(deg1);
  }
  if(Right_1 == 1){
    if(deg1>=0) deg1-=0.1;
    servo1.write(deg1);
  }
  if(Left_2 == 1){
    if(deg2<=180) deg2+=0.1;
    servo2.write(deg2);
  }
  if(Right_2 == 1){
    if(deg2>=0) deg2-=0.1;
    servo2.write(deg2);
  }
}

