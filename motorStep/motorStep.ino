#include <ArduinoJson.h>
// 33 landmarks

//Encoder variables
int encoderPinB = 2;
int encoderPinA = 3;

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

// Motor variables
const int stepPin = 4;
const int dirPin = 5;
long timeprev = 0;
int state = 0;

// velocity to delay time function
// fastest step speed is 550ms from tests, so scale accordingly
float delaytime(float speed) {
  return 550 * 200 / speed;
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

void setup() {
  // Sets the two pins as Outputs
  Serial.begin (14400);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  pinMode(encoderPinB, INPUT_PULLUP); // internal pullup input pin 2
  pinMode(encoderPinA, INPUT_PULLUP); // internal pullup input pin 3

  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nmbr 2 on most Arduino.
  attachInterrupt(0, ai0, RISING);

  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nmbr 3 on most Arduino.
  attachInterrupt(1, ai1, RISING);
}

void loop() {
  if (state == 0) { //finding target position
    if (Serial.available() > 0) {
      DynamicJsonDocument doc(2048);
      String posInput = Serial.readStringUntil('\n');

      DeserializationError err = deserializeJson(doc, posInput.c_str());
      if (err) {
        Serial.println(err.f_str());
        return;
      };
      target = doc[15]; //left hand y position is target
      Serial.println(target);
    }
    state = 1;
  }

  else if (state == 1) { //finding error
    long currT = micros();
    float deltaT = (currT - prevT) / 1.0e6;

    //find error
    int error = target - pos;
    //find derivative term
    float dedt = (error - errorprev) / deltaT;
    errorprev = error; //store previous error

    //find integral term
    eintegral = eintegral + error * deltaT;

    //PID calculation
    u = kp * error + kd * dedt + ki * eintegral;

    //motor velocity with level from 0-200 (arbritrary scale)
    vel = fabs(u);
    if (vel > 200) {
      vel = 200;
    }
    prevT = currT;
    state = 2;
  }

  else if (state == 2) { //motor high step
    long timenow = micros();
    long telapsed = timenow - timeprev;
    if (u < 0) {
      digitalWrite(dirPin, HIGH);
    }
    else {
      digitalWrite(dirPin, LOW);
    }
    digitalWrite(stepPin, HIGH);

    if (telapsed == delaytime(vel)) {
    timeprev = timenow;
    state = 3;
    }
  }

  else if (state == 3) { //motor low step
    long timenow = micros();
    long telapsed = timenow - timeprev;
    digitalWrite(stepPin, LOW);
    if (telapsed == 2 * delaytime(vel)) {
    timeprev = timenow;
    state = 0;
    }
  }
}
