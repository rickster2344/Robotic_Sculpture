try:
    import pyfirmata as py
except:
    import pip
    pip.main(['install','pyfirmata'])
    import pyfirmata as py
import time
import numpy as np
import pandas as pd
import keyboard #https://stackoverflow.com/questions/24072790/detect-key-press-in-python

board = py.Arduino('/dev/cu.usbmodem1463301')
universalDelay = 0.0005
fps = 30

class Pulley:
    def __init__(self, dirPin, stepPin, df):
        self.dirPin = board.get_pin('d:'+ str(dirPin) +':o')
        self.stepPin = board.get_pin('d:'+ str(stepPin)+':o')
        self.currentStep = 0
        self.difference = 0
        self.delay = 0.0005
        self.posLimit =0
        self.negLimit= 0 #constant
        self.reversed = -1
        self.data = df
#movement functions
    def onStep(self, bool):
        if bool== True:
            if self.reversed == -1:
                self.dirPin.write(1)
                self.stepPin.write(1)
            else:
                self.dirPin.write(0)
                self.stepPin.write(1)
        else:
            if self.reversed == -1:
                self.dirPin.write(0)
                self.stepPin.write(1)
            else:
                self.dirPin.write(1)
                self.stepPin.write(1)
    def offStep(self,bool):
        if bool== True:
            if self.reversed == -1:
                self.stepPin.write(0)
            else:
                self.stepPin.write(1)
        else:
            if self.reversed == -1:
                self.stepPin.write(1)
            else:
                self.stepPin.write(0)

    def moveOn(self, position):
        if position > self.currentStep:
            self.onStep(True)
            self.currentStep += 1
        elif position < self.currentStep:
            self.onStep(False)
            self.currentStep -= 1
        else:
            pass

    def moveOff(self, position):
        if position > self.currentStep:
            self.offStep(True)
        elif position < self.currentStep:
            self.offStep(False)
        else:
            pass

    def oneStep(self):
        if (True):
            self.onStep(True)
            time.sleep(self.delay)
            self.offStep(True)
            time.sleep(self.delay)
        else:
            self.onStep(False)
            time.sleep(self.delay)
            self.offStep(False)
            time.sleep(self.delay)
#CALIBRATION SEQUENCE FROM PREVIOUS WORK
        # calibrate finds the number of steps for the object to move from the ground to the ceiling, this can then be multiplied by a percentage to move object to position. (x% of the way up to the ceiling)

    def calibrate(self):
        print('Calibrating: press up to move up, press down to move down, s to set limit.')

        print('Set direction. Press r to reverse direction')
        while True:
            if keyboard.is_pressed('r'):
                self.reversed = -self.reversed
            elif keyboard.is_pressed('up'):
                self.oneStep(True)
            elif keyboard.is_pressed('down'):
                self.oneStep(False)
            elif keyboard.is_pressed('s'):
                print('Direction set...')
                break
        time.sleep(0.2)
        print('Set bottom limit, move pulley to bottom position') #bottom limit is always 0.
        while True:
            if keyboard.is_pressed('up'):
                self.oneStep(True)
            elif keyboard.is_pressed('down'):
                self.oneStep(False)
            elif keyboard.is_pressed('s'):
                print("\nBottom limit set...\n")
                break
        time.sleep(0.2)

        print('Set top limit') #sets the top position of the motor and counts the steps to get to the top
        while True:
            if keyboard.is_pressed('up'):
                self.oneStep(True)
                self.posLimit+=1
            elif keyboard.is_pressed('down'):
                self.oneStep(False)
                self.posLimit-=1
            elif keyboard.is_pressed('s'):
                print("\nTop limit set...\n")
                self.currentStep= self.posLimit
                break
        time.sleep(0.2)

        print(f"Top Limit: {self.posLimit} Bottom Limit: {self.negLimit}")
        time.sleep(0.2)

def scale(df):
    dfcopy= df
    df-= dfcopy.min()
    df = df/dfcopy.max()
    del dfcopy
    if (df.min()==0 and df.max()==1):
        print(f"{df.name} scaled from 0-1")
    else:
        print('error')
    return df

positions= pd.read_csv("arm_positions.csv")
thumb_y= positions['thumb_y']
handtip_y= positions['handtip_y']
hand_y= positions['hand_y']

thumb_y=scale(thumb_y)
handtip_y= scale(handtip_y)
hand_y= scale(hand_y)

t = Pulley(5,2,thumb_y)
u = Pulley(6,3,handtip_y)
v = Pulley(7,4,hand_y)

lst = [t,u,v]
for module in lst:
    module.calibrate()

loopReps= np.floor(1/fps/(universalDelay*2))#do it enough times that the time delay corresponds with the frame rate.
print(loopReps)
for i in range(len(positions)):
    for j in range(int(loopReps)):
        for module in lst:
            module.moveOn(int(np.floor(module.data[i]*module.posLimit)))
        time.sleep(universalDelay)
        for module in lst:
            module.moveOff(int(np.floor(module.data[i]*module.posLimit)))
        time.sleep(universalDelay)
    print(f't pos: {t.currentStep} u pos: {u.currentStep} v pos: {v.currentStep}')
