#include "Arduino.h"
#include "DCMotor.h"

DCMotor::DCMotor(int enPin, int in1, int in2) {//Constructor. Code will be executed when object is created in the sketch
    pinMode(enPin, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);

    _enPin = enPin;
    _in1 = in1;
    _in2 = in2;
}

void DCMotor::moveMotor(int motorVel){
    if(motorVel > 255){ //normalize to 255, redundant protection.
      motorVel = 255;
      }
    else if (motorVel < -255){
      motorVel = -255;
    }
    
    if(motorVel > 0){ //positive, rotate clockwise
        digitalWrite(_in1, HIGH);
        digitalWrite(_in2, LOW);
    }

    else if (motorVel < 0 ){ //negative, rotate anti-clockwise
        digitalWrite(_in1, LOW);
        digitalWrite(_in2, HIGH);
    }

    else{ // 0, turn off motor
        digitalWrite(_in1, LOW);
        digitalWrite(_in2, LOW);
    }

    analogWrite(_enPin, abs(motorVel)); //send PWM signal to enable pin
}
