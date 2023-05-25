# some complicated code i dont get: https://forum.openframeworks.cc/t/solved-stepper-motor-with-firmata-arduino/4973
try:
# pyfirmata documentation:https: //pyfirmata.readthedocs.io/_/downloads/en/0.9.5/pdf/
# github:https: //github.com/tino/pyFirmata#readme
# pyfirmata pypi with description: https://pypi.org/project/pyFirmata/
    import pyfirmata as py
except:
    import pip
    pip.main(['install','pyfirmata'])
    import pyfirmata as py
import time
import os
import pandas as pd
# import uln2003
#https://github.com/IDWizard/uln2003
#rasberry pi stepper libraries https://github.com/topics/uln2003


board = py.Arduino('/dev/cu.usbmodem146201')
positions= pd.read_csv("arm_positions.csv")
# print(len(positions))
# positions.head()

# start iterator thread so serial buffer does not overflow for analog ports
iterator = py.util.Iterator(board)
iterator.start()

class Stepper:
    def __init__(self,outSteps,stepPin1, stepPin2, stepPin3, stepPin4):
        self.windingState = 0
        self.delay = 0.01
        self.stepPin1=stepPin1
        self.stepPin2=stepPin2
        self.stepPin3=stepPin3
        self.stepPin4=stepPin4
        self.outSteps= outSteps
        self.angleRatio = outSteps/360

    def show(self):
        print(f"Pin1: {self.stepPin1}, Pin2: {self.stepPin2}, Pin3: {self.stepPin3}, Pin4: {self.stepPin4}")
        print(f"Steps Per Output Revolution: {self.outSteps}")

    # changes the speed of motor by defining the delay
    # max speed calculator: https://www.allaboutcircuits.com/tools/stepper-motor-calculator/
    def setSpeed(self, speed):
        if speed >1000:
            print('max speed is 1000')
        elif speed<1:
            print('min speed is 1')
        else:
            self.delay= 2/speed

    # Changes digital signal of pins to change
    def fullStepState(self,num):
        if num==0:
            board.digital[self.stepPin1].write(1)
            board.digital[self.stepPin2].write(0)
            board.digital[self.stepPin3].write(0)
            board.digital[self.stepPin4].write(0)
        elif num==1:
            board.digital[self.stepPin1].write(0)
            board.digital[self.stepPin2].write(1)
            board.digital[self.stepPin3].write(0)
            board.digital[self.stepPin4].write(0)
        elif num==2:
            board.digital[self.stepPin1].write(0)
            board.digital[self.stepPin2].write(0)
            board.digital[self.stepPin3].write(1)
            board.digital[self.stepPin4].write(0)
        else:
            board.digital[self.stepPin1].write(0)
            board.digital[self.stepPin2].write(0)
            board.digital[self.stepPin3].write(0)
            board.digital[self.stepPin4].write(1)

    # steps the motor once in clockwise or counterclockwise
    def oneStep(self, bool):
        if (bool == True):
            self.fullStepState(self.windingState)
            self.windingState+=1
            self.windingState = self.windingState%4
        else:
            self.fullStepState(self.windingState)
            self.windingState-=1
            self.windingState = self.windingState%4

    #steps the motor a certain number of times
    def step(self, numSteps):
        if numSteps > 0:
            for i in range(numSteps):
                self.oneStep(True)
                time.sleep(self.delay)
        else:
            numSteps = -numSteps
            for i in range(numSteps):
                self.oneStep(False)
                time.sleep(self.delay)
    def angle(self, angle):
        self.step(round(angle*self.angleRatio))

# Servo Control:
# about servos"https://youtu.be/J8atdmEqZsc
# example code:https://scruss.com/blog/2012/10/28/servo-control-from-pyfirmata-arduino/

#creates servo object, digital, pin 3, servo.
servoPin1 = board.get_pin('d:3:s')
#flips to rotation limits
for i in range(10):
    servoPin1.write(180)
    time.sleep(0.25)
    servoPin1.write(0)
    time.sleep(0.25)
#full c, then cc rotation.
for i in range(0,180):
    servoPin1.write(i)
    time.sleep(0.01)
for i in range(180,0,-1):
    servoPin1.write(i)
    time.sleep(0.01)

#Unipolar movement

#describing the 2byj-48 stepper
gearRed = 64
stepsPRev = 32
stepsPOutRev = gearRed * stepsPRev

# create Stepper object
duino1 = Stepper(stepsPOutRev,13,12,11,10)
duino1.setSpeed(1000)
duino1.show()

# rotate 180 degrees c & cc
duino1.angle(180)
time.sleep(1)
duino1.angle(180)
time.sleep(1)
duino1.step(-stepsPOutRev)
