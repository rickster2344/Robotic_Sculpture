#include <ArduinoJson.h>
// defines pins numbers
const int stepPin = 5; 
const int dirPin = 4; 
int timeDelay = 0;

//Encoder variables
int encoderPinB = 2;
int encoderPinA = 3;

//state storage
int state = 0;

//PID coefficients
float kp = 1;
float kd = 0;
float ki = 0;

//PID storage vars
int pos = 0; //This variable will increase or decrease depending on the rotation of encoder
int target = 0;
long prevT = 0;
float u = 0;
float vel = 0;
float eintegral = 0;
float errorprev = 0;

// velocity to delay time function
// fastest step speed is 550ms from tests, so scale accordingly
float delaytime(float speed) {
  return 550 * 200 / speed;
}


void setup() {
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  
  //Setting up interrupt
  pinMode(encoderPinB, INPUT_PULLUP); // internal pullup input pin 2 
  pinMode(encoderPinA, INPUT_PULLUP); // internal pullup input pin 3
  
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nmbr 2 on most Arduino.
  attachInterrupt(0, ai0, RISING);

  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nmbr 3 on most Arduino.
  attachInterrupt(1, ai1, RISING);
}

void loop() {
  if (state == 0){
    long currT = micros();
    float deltaT = (currT - prevT) / 1.0e6;

    //find error term
    int error = target - pos;
    
    //find derivative term
    float dedt = (error - errorprev) / deltaT;
    errorprev = error; //store previous error

    //find integral term
    eintegral = eintegral + error * deltaT;

    //PID calculation
    u = kp * error + kd * dedt + ki * eintegral;

//    motor velocity with level from 0-200 (arbritrary scale)
    vel = fabs(u);
    if (vel > 200) {
      vel = 200;
    }
    prevT = currT;
    Serial.println(u);
  }
}

void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if (digitalRead(encoderPinA) == LOW) {
    pos++;
  } else {
    pos--;
  }
}

void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if (digitalRead(encoderPinB) == LOW) {
    pos--;
  }
  else {
    pos++;
  }
}
