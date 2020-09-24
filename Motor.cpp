#include"Motor.h"
#include<Arduino.h>
Motor::Motor(int pin1,int pin2){
    pin1_=pin1;
    pin2_=pin2;
    pinMode(pin1_,OUTPUT);
    pinMode(pin2_,OUTPUT);
}
void Motor::run(int speed){
  if(speed>0){

        digitalWrite(pin1_,HIGH);
        digitalWrite(pin2_,LOW);
        

    }

    if(speed<0){

        digitalWrite(pin1_,LOW);
        digitalWrite(pin2_,HIGH);
        

    }
     if(speed==0){

        digitalWrite(pin1_,LOW);
        digitalWrite(pin2_,LOW);
        

    }
    
    
}
