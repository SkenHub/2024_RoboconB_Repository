#include <dummy.h>
#include <Ps3Controller.h>
#include <ESP32Servo.h>

const int S1 = 5;//Servo1のピン番号を記入(Lトリガーで制御)
//const int S2 = 34;//Servo2のピン番号を記入(Rトリガーで制御)
int ServoDeg = 0;//Servo角度制御用
Servo servo;
//Servo servo2;
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
//  Ps3.attach(Joshiryo);//named by Taichi Kurozumi この関数はサーボ動作用


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
//      servo2.attach(S2,510,2400);
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


//
//void Joshiryo(){

//Servo_move
//if( abs(Ps3.event.analog_changed.button.l2) ){
  if(Ps3.data.analog.button.l2-75>0){//負の数をサーボに突っ込んだらめんどくさそうだから取り除く
if(ServoDeg != 180){
  ServoDeg = ServoDeg + 1;}
       servo.write(ServoDeg); //Servo1をLトリガーの入力分回す(0~180°)。トリガーは0~255なため、75を引いて0~180にしている
  }//else if(Ps3.data.analog.button.l2-75<0){
   // servo1.write(0);
  //}
//}

//   if( abs(Ps3.event.analog_changed.button.cross) ){//Cross!!
  //     Serial.print("Pressing the cross button: ");
    //   Serial.println(Ps3.data.analog.button.cross, DEC);
      // servo1.write(Ps3.data.analog.button.cross);
   //}

//if( abs(Ps3.event.analog_changed.button.r2) ){
  if(Ps3.data.analog.button.r2-75>0){//上に同じく、めんどくさそうなので取り除く
       if(ServoDeg != 0){
       ServoDeg = ServoDeg - 1;}
       servo.write(ServoDeg);
       //servo2.write(Ps3.data.analog.button.r2-75); //Servo2をRトリガーの入力分回す(0~180°)
  }
//}

  Serial.print(Ps3.event.analog_changed.button.l2);//debug用にトリガーの出力をシリアルで垂れ流す
  Serial.print("Left_TR_Servo_Output");
  Serial.print(Ps3.event.analog_changed.button.r2);
  Serial.print("Right_TR_Servo_Output");
  Serial.println();
}
