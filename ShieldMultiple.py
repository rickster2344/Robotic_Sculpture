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

board = py.Arduino('/dev/cu.usbmodem1463201')
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

    #add calibration seq here

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

# t.calibrate()
# u.calibrate()
# v.calibrate()

# set for easy code testing instead of calibration sequence
lst = [t,u,v]
for module in lst:
    module.posLimit=1000
    module.currentStep=1000

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
