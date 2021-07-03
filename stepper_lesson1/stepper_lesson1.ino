#include <Stepper.h> //stepper library
//https://youtu.be/0qwrnUeSpYQ 
//steps per revolution
const float StepsPRev = 32.;
//Gear Reduction of Stepper
const float GearRed = 64; 
//# of steps per geared output rotation
const float StepsPOutRev= StepsPRev * GearRed;

int StepsRequired;

//specify pins used for motor coils
//8,9,10,11 used to in1, in2,in3,in4.
//pins in sequence 1-3-2-4
//create instance of stepper class
Stepper steppermotor(StepsPRev, 8, 10, 9, 11);

void setup() {
  // put your setup code here, to run once:
//stepper library automatically sets pins as o utput 
}

void loop() {
  // Slow rotation- 4- step CW sequence, look at lights on driver board
 steppermotor.setSpeed(1);
 StepsRequired = 4;
 steppermotor.step(StepsRequired);
 delay(500);

// rotate CW 1/2 turn slowly
  StepsRequired = StepsPOutRev/2;
  steppermotor.setSpeed(100);
  steppermotor.step(StepsRequired);
  delay(1000);

//Rotate counter-CW 1/2 quickly
  StepsRequired = -StepsPOutRev/2;
  steppermotor.setSpeed(700);
  steppermotor.step(StepsRequired);
  delay(2000);
 
}
