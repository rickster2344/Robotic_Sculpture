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

board = py.Arduino('/dev/cu.usbmodem146201')
positions= pd.read_csv("arm_positions.csv")
# print(len(positions))
# positions.head()

# start iterator thread so serial buffer does not overflow for analog ports
iterator = py.util.Iterator(board)
iterator.start()

xDirPin = board.get_pin('d:5:o') #digital 5, output.
xStepPin = board.get_pin('d:2:o')
xDelay = 0.53

xDirPin.write(1)
for i in range(200):
    xStepPin.write(1)
    time.sleep(xDelay)
    xStepPin.write(0)
    time.sleep(xDelay)
time.sleep(3)
xDirPin.write(0)
for i in range(200):
    xStepPin.write(1)
    time.sleep(xDelay)
    xStepPin.write(0)
    time.sleep(xDelay)
