try:
    from pyfirmata import Arduino, util
except:
    import pip
    pip.main(['install','pyfirmata'])
    from pyfirmata import Arduino, util
import os
import pandas as pd
# import uln2003
#https://github.com/IDWizard/uln2003
#rasberry pi stepper libraries https://github.com/topics/uln2003


# file = "arm_positions.csv"
# if (file.is_file()== False):
#     print('Data not found')
board = Arduino('/dev/cu.usbmodem146201')
positions= pd.read_csv("arm_positions.csv")
print(len(positions))
# positions.head()

iterator = util.Iterator(board)
iterator.start()
