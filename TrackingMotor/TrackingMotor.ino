#include "DCMotor.h"

//State machine variable
int state = 0;
int active = false;

//Encoder Variables
volatile long temp, encoderPos = 500; //This variable will increase or decrease depending on the rotation of encoder
int encoderPinB = 2;
int encoderPinA = 3;

//PID coefficients
float kp = 1;
float kd = 0;
float ki = 0;

//PID storage vars
//int pos = 0; //This variable will increase or decrease depending on the rotation of encoder
int target = 0;
unsigned long prevT = 0;
float u = 0;
float vel = 0;
float eintegral = 0;
float errorprev = 0;

//Motor Defenition:
//DCMotor motor2(9,8,7);
DCMotor motor1(6,7,8);

void setup() {
  Serial.begin(115200);
  //Encoder Setup:
  pinMode(encoderPinB, INPUT_PULLUP); // internal pullup input pin 2 
  pinMode(encoderPinA, INPUT_PULLUP); // internal pullup input pin 3
  attachInterrupt(0, ai0, RISING);
  attachInterrupt(1, ai1, RISING);
}

void timer(int micro){
  unsigned long now;
  unsigned long b4;
  now = millis();
  b4 = now;
  while (now - b4 < micro){
    now = millis();
  }
}


void check(){//watchdog function checking for response from python. if no response for 1 second, off.
  unsigned long currT;
  unsigned long prevT;
  currT = millis();
  prevT = currT;
  char comm;
  bool condition = false;
  Serial.println('c');
  while (currT - prevT < 1000 && condition == false){
    currT = millis();

    if (Serial.available()>0){
      comm = Serial.read();
      condition = true;
      active = true;
    }
  }
  if (currT-prevT >= 1000){
    active = false;
  }
}


//ammend: null terminated charater arrays instead of strings. 
//change communication protocol to [r,.,.,.,\n] change to staggered process of reading message
//pseudo parallel reading of data.

void request(){
  char msg;
  int intChar = 0;
  float pow10 = 10;
  float value = 0;
  int degree = 0;

  bool condition = false;
  
  Serial.println('r');
  do{
    condition = false;
    do{//waits for there to be something in the serial port
      if (Serial.available()>0){
        condition = true;
      }
    } while (condition == false);
    msg = Serial.read();
    if(msg != 'f'){
      intChar = msg - '0';
      value += intChar*pow(pow10,degree);
      degree +=1;
    } 
  }while(msg != 'f');
   target = int(value);
   Serial.print("Target: ");
   Serial.println(target);
}
  
  
void loop() {
  if (state ==0){//idle state
    motor1.moveMotor(0);//turn off motors
    bool condition = false;
    char msg;
    do{ //wait for start
        if (Serial.available()>0){
          msg = Serial.read();
          condition = true;
        }
      }while (condition == false);
    if (msg== 's'){
      active = true;
      state = 1;
    }
  }
  
  else if (state == 1){ //Read target position
    check();
    
    if (active == true){
      request();
      state = 2;
    }
    else{
      state = 0;
      Serial.println('0');
    }
   }
   
  else if (state == 2) { //PID calculation
    unsigned long currT = micros();
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
    state = 3;
  }

  else if (state == 3){ //motor movement
    motor1.moveMotor(vel);
    state = 1;
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
