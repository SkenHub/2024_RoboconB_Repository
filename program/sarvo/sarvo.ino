#include<Servo.h>
Servo makitori;
Servo agesage;
int Left_1, Left_2,Right_1, Right_2;
float kakudo = 0;
float kakudo2 = 0;
void setup() {
  makitori.attach(1);
  agesage.attach(1);
}
void loop() {
  if(Left_1 = 1){
    kakudo += 0.01;
    makitori.write(kakudo);}
  if(Right_1 = 1){
    kakudo -= 0.01;
    makitori.write(kakudo);}
  if(Left_2 = 1){
    kakudo2 += 0.01;
    agesage.write(kakudo2);}
  if(Right_2 = 1){
    kakudo2 -= 0.01;
    agesage.write(kakudo2);}
}