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
import numpy as np
import pandas as pd
import keyboard #https://stackoverflow.com/questions/24072790/detect-key-press-in-python

board = py.Arduino('/dev/cu.usbmodem1463201')

class Pulley:
    def __init__(self,dirPin,stepPin):
        self.dirPin = board.get_pin('d:'+ str(dirPin) +':o')
        self.stepPin = board.get_pin('d:'+ str(stepPin)+':o')
        self.currentStep = 0
        self.delay = 0.0005
        self.posLimit =0
        self.negLimit= 0 #constant
        self.reversed = -1

#steps the motor by numSteps
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

# check the position of the motor
    def pos(self):
        print(self.currentStep)

    # calibrate finds the number of steps for the object to move from the ground to the ceiling, this can then be multiplied by a percentage to move object to position. (x% of the way up to the ceiling)
    def calibrate(self):
        print('Calibrating: press up to move up, press down to move down, s to set limit.')

        print('Set direction. Press r to reverse direction')
        while True:
            if keyboard.is_pressed('r'):
                self.reversed = -self.reversed
            elif keyboard.is_pressed('up'):
                self.step(1)
            elif keyboard.is_pressed('down'):
                self.step(-1)
            elif keyboard.is_pressed('s'):
                print('Direction set...')
                break
        time.sleep(0.2)
        print('Set bottom limit, move pulley to bottom position') #bottom limit is always 0.
        while True:
            if keyboard.is_pressed('up'):
                self.step(1)
            elif keyboard.is_pressed('down'):
                self.step(-1)
            elif keyboard.is_pressed('s'):
                print("\nBottom limit set...\n")
                break
        time.sleep(0.2)

        print('Set top limit') #sets the top position of the motor and counts the steps to get to the top
        while True:
            if keyboard.is_pressed('up'):
                self.step(1)
                self.posLimit+=1
            elif keyboard.is_pressed('down'):
                self.step(-1)
                self.posLimit-=1
            elif keyboard.is_pressed('s'):
                print("\nTop limit set...\n")
                self.currentStep= self.posLimit
                break
        time.sleep(0.2)

        print(f"Top Limit: {self.posLimit} Bottom Limit: {self.negLimit}")
        time.sleep(0.2)

    def test(self):
        print('test initiated')
        while True:
            if keyboard.is_pressed('r'):
                self.reversed = -self.reversed
            elif keyboard.is_pressed('up'):
                self.step(1)
            elif keyboard.is_pressed('down'):
                self.step(-1)
            elif keyboard.is_pressed('s'):
                print("\ntest finished\n")
                break
    def move(self, position):
        if position >0:
            self.step(position-self.currentStep)
            self.currentStep = position
        else:
            print('Error, pos less than 0')

#function scales the dataframe from 0 to 1, this acts as a multiplier that formats the coordinates to the motor positions in real life
def scale(df):
    dfcopy= df
    df= dfcopy
    df-= dfcopy.min()
    df = df/dfcopy.max()
    del dfcopy
    if (df.min()==0 and df.max()==1):
        print(f"{df.name} scaled from 0-1")

positions= pd.read_csv("arm_positions.csv")
thumb_y= positions['thumb_y']
scale(thumb_y)

t = Pulley(5,2)
t.calibrate()
for i in range(len(thumb_y)):
    num = np.floor(thumb_y[i]*t.posLimit)
    t.move(int(num))
    print(num)

#arduino interacts through a serial so it could interact that way
