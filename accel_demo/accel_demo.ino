/*
 * stepper motor 2: manipulating 2 28YBJ-48 Unipolar steppers with ULN 2003 driver
 * acellstepper library
 */
#include <AccelStepper.h>

#define FULLSTEP 4
#define HALFSTEP 8

//Defining motor pins
#define motorPin1 8 //board pin 1
#define motorPin2 9 //board pin 2
#define motorPin3 10 //board pin 3
#define motorPin4 11 //board pin 4

#define motorPin5 4 //board pin 1
#define motorPin6 5 //board pin 2
#define motorPin7 6 //board pin 3
#define motorPin8 7 //board pin 4

//Defining 2 motor objects
//seq= 1-3-2-4 to properly sequence unipolar motor
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
AccelStepper stepper2(FULLSTEP, motorPin5, motorPin7, motorPin6, motorPin8);

void setup() {
  //  Motor 1- 1 rev CW
  stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(50.0);
  stepper1.setSpeed(200);
  stepper1.moveTo(2048);

 // Motor 2- 1 rev CCW
  stepper2.setMaxSpeed(1000.0);
  stepper2.setAcceleration(50.0);
  stepper2.setSpeed(200);
  stepper2.moveTo(-2048);
}

void loop() {
  // change direction at limits
  if (stepper1.distanceToGo() ==0){
    stepper1.moveTo(-stepper1.currentPosition());
  }
  if (stepper2.distanceToGo() ==0){
     stepper2.moveTo(-stepper2.currentPosition());
  }
  stepper1.run();
  stepper2.run();
}
