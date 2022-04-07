#ifndef DCMotor_h
#define DCMotor_h
#include "Arduino.h"
//Following tutorial: https://www.circuitbasics.com/programming-with-classes-and-objects-on-the-arduino/

class DCMotor {
    public: 
        DCMotor(int enPin, int in1, int in2);
        void moveMotor(int motorVel);

        int _enPin;
        int _in1;
        int _in2;
};

#endif