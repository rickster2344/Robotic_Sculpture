#a tracking turret using opencv https://github.com/HackerShackOfficial/Tracking-Turret/blob/master/turret.py
try:
# pyfirmata documentation:https://pyfirmata.readthedocs.io/_/downloads/en/0.9.5/pdf/
# github:https://github.com/tino/pyFirmata#readme
    import pyfirmata as py
except:
    import pip
    pip.main(['install','pyfirmata'])
    import pyfirmata as py
import time
import os
import pandas as pd
import keyboard #https://stackoverflow.com/questions/24072790/detect-key-press-in-python

board = py.Arduino('/dev/cu.usbmodem1463201')
positions= pd.read_csv("arm_positions.csv")
# print(len(positions))
# positions.head()

class Pulley:
    def __init__(self,dirPin,stepPin):
        self.dirPin = board.get_pin('d:'+ str(dirPin) +':o')
        self.stepPin = board.get_pin('d:'+ str(stepPin)+':o')
        self.currentStep = 0
        self.delay = 0.0005
        self.posLimit =0
        self.negLimit= 0 #constant
        self.reversed = -1

    def step(self,numSteps):
        if self.reversed == -1:
            if numSteps>0:
                self.dirPin.write(1)
                for i in range(numSteps):
                    self.stepPin.write(1)
                    time.sleep(self.delay)
                    self.stepPin.write(0)
                    time.sleep(self.delay)
            else:
                self.dirPin.write(0)
                for i in range(-numSteps):
                    self.stepPin.write(1)
                    time.sleep(self.delay)
                    self.stepPin.write(0)
                    time.sleep(self.delay)
        else:
            if numSteps>0:
                self.dirPin.write(0)
                for i in range(numSteps):
                    self.stepPin.write(1)
                    time.sleep(self.delay)
                    self.stepPin.write(0)
                    time.sleep(self.delay)
            else:
                self.dirPin.write(1)
                for i in range(-numSteps):
                    self.stepPin.write(1)
                    time.sleep(self.delay)
                    self.stepPin.write(0)
                    time.sleep(self.delay)

    def pos(self):
        print(self.currentStep)

    # calibrate finds the number of steps for the object to move from the ground to the ceiling
    def calibrate(self):
        print('Calibrating: press up to move up, press down to move down, enter to set limit. press r to reverse direction')
        print('Set bottom limit, move pulley to bottom position')
        while True:
            if keyboard.is_pressed('r'):
                self.reversed = -self.reversed
            elif keyboard.is_pressed('up'):
                self.step(-1)
            elif keyboard.is_pressed('down'):
                self.step(1)
            elif keyboard.is_pressed('\n'):
                print("\nBottom limit set\n")
                break
        time.sleep(0.5)
        print('Set top limit')
        while True:
            if keyboard.is_pressed('r'):
                self.reversed = -self.reversed
            elif keyboard.is_pressed('up'):
                self.step(-1)
                self.posLimit+=1
            elif keyboard.is_pressed('down'):
                self.step(1)
                self.posLimit-=1
            elif keyboard.is_pressed('\n'):
                print("\nTop limit set\n")
                self.currentStep= self.posLimit
                break
        time.sleep(0.5)
        print(f"Top Limit: {self.posLimit} Bottom Limit: {self.negLimit}")

    def test(self):
        print('test initiated')
        while True:
            if keyboard.is_pressed('r'):
                self.reversed = -self.reversed
            elif keyboard.is_pressed('up'):
                self.step(1)
            elif keyboard.is_pressed('down'):
                self.step(-1)
            elif keyboard.is_pressed('\n'):
                print("\ntest finished\n")
                break
    def move(self, position):
        self.step(position-self.currentStep)
        self.currentStep = position

    #motor movement
    def start(self):
        print('pulley started, press s to stop')

        while True:
            #self.move(position in csv)
            if keyboard.is_pressed('s'):
                print('pulley stopped.')
                break

t = Pulley(5,2)
# t.test()
# t.calibrate()
# t.pos()
