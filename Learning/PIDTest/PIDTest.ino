#include "DCMotor.h"
DCMotor motor1(5,6,7);
volatile long encoderPos,temp = 0;
int encoderPinB = 2;
int encoderPinA = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(encoderPinB, INPUT_PULLUP); // internal pullup input pin 2 
  pinMode(encoderPinA, INPUT_PULLUP); // internal pullup input pin 3
  attachInterrupt(0, ai0, RISING);
  attachInterrupt(1, ai1, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  motor1.moveMotor(120);
  
  if( temp!=encoderPos){
    Serial.println(encoderPos);
  }

 }


//Encoder Functionality:
void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(encoderPinA)==LOW) {
    encoderPos++;
  }
  else{
    encoderPos--;
  }
}
   
void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(encoderPinB)==LOW) {
    encoderPos--;
  }
  else{
    encoderPos++;
  }
}
