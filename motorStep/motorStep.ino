#include "DCMotor.h"

//State machine variable
int state = 0;

//Encoder Variables
volatile long temp, encoderPos = 0; //This variable will increase or decrease depending on the rotation of encoder
int encoderPinB = 2;
int encoderPinA = 3;

//PID coefficients
float kp = 1;
float kd = 0;
float ki = 0;

//PID storage vars
//int pos = 0; //This variable will increase or decrease depending on the rotation of encoder
int target = 0;
long prevT = 0;
float u = 0;
float vel = 0;
float eintegral = 0;
float errorprev = 0;

//Motor Defenition:
//DCMotor motor2(9,8,7);
DCMotor motor1(4,5,6);

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  //Encoder Setup:
  pinMode(encoderPinB, INPUT_PULLUP); // internal pullup input pin 2 
  pinMode(encoderPinA, INPUT_PULLUP); // internal pullup input pin 3
  attachInterrupt(0, ai0, RISING);
  attachInterrupt(1, ai1, RISING);
}

//Message request functionality:
long now;
long b4;

void timer(int milis){
  now = micros();
  b4 = now;
  while (now - b4 < milis){
    now = micros();
  }
}
//ammend: null terminated charater arrays instead of strings. 
//change communication protocol to [r,.,.,.,\n] change to staggered process of reading message
//pseudo parallel reading of data.

void request(){//there seems to be a bottleneck somewhere in the ask send protocol. 
  //making it really slow to go between mediapipe and arduino
  String message;
  Serial.println("request");
  bool condition = false;
  digitalWrite(13,LOW);
  do{
    if (Serial.available()>0){//this is slow
      condition = true;
    }
  } while (condition == false);
  digitalWrite(13,HIGH);
//    read the incoming byte:
//   message = Serial.readStringUntil('\n');// these two lines are holding up because string operations are very slow on a microcontroller (and on regular computer)
//   target= message.toInt();
   target = 0;
   Serial.print("Target: ");
   Serial.println(target);
}
  
void loop() {
  if (state == 0){ //Read target position
    request();
    state = 1;
   }
   
  else if (state == 1) { //PID calculation
    long currT = micros();
    float deltaT = (currT - prevT) / 1.0e6;

    //find error
    Serial.print("position: ");
    Serial.println(encoderPos/100);
    int error = target - encoderPos/100;
    //find derivative term
    float dedt = (error - errorprev) / deltaT;
    errorprev = error; //store previous error

    //find integral term
    eintegral = eintegral + error * deltaT;

    //PID calculation
    u = kp * error + kd * dedt + ki * eintegral;

    vel = u;
    //motor velocity with level from 0-255 PWM scale
    if (vel > 255) {
      vel = 255;
    }
    else if (vel < -255){
      vel = -255;
    }
    Serial.print("velocity: ");
    Serial.println(vel);
    prevT = currT;
    state = 2;
  }

  else if (state == 2){ //motor movement
    motor1.moveMotor(vel);
    state = 0;
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
