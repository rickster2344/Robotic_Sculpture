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
DCMotor motor1(5,6,7);

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

float charArrToInt(char input[4]){
  int i;
  float pow10=10;
  float number =0; 
  int value = 0;
  for(i=0; i<4; i++){
    value = input[3-i]-'0';
    number += value*pow(pow10,i);
  }
  return number;
}

//ammend: null terminated charater arrays instead of strings. 
//change communication protocol to [r,.,.,.,\n] change to staggered process of reading message
//pseudo parallel reading of data.

void request(){//there seems to be a bottleneck somewhere in the ask send protocol. 
  //making it really slow to go between mediapipe and arduino
  char message[4]; //character array to store number
  
  Serial.println('r');
  bool condition = false;
  do{//waits for there to be something in the serial port
    if (Serial.available()>0){
      condition = true;
    }
  } while (condition == false);

//read the incoming byte:
  int j = 0;
  char msg;
  do{
    msg = Serial.read();
    if(msg != 'f' && Serial.available()>0){
       message[j] = msg;
       Serial.print(msg);
    }
    j+=1;
  }while(msg != 'f');;

  target = charArrToInt(message);
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
    Serial.print(vel);
    Serial.print(" u: ");
    Serial.println(u);
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
