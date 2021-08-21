import serial
from time import sleep
import json

port = serial.Serial('/dev/cu.usbmodem1463301', baudrate=9600, timeout=1) #Defining port with timeout (if doesnt recieve info after 1 sec)
sleep(2) #ensure arduino is fully init

numPoints = 4
numRows = 32

dataFile= open('dataFile.txt', 'w') #name file and 'w' for write as parameters, in case you need to save some data

def printToFile(data,index):
    
    dataFile.write(data)
    if index != (numPoints - 1):
        dataFile.write(',')
    else:
        dataFile.write('\n')


# data = [[1,2,3], [3,3,3]]
# sentence = str(f'X:{data[0][0]}Y:{data[0][1]}Z:{data[0][2]}')
# port.write(sentence.encode())

# sleep(5)
# print(port.readline())


#x,y,z,v in string, print to serial and then interpret in arduino using substring.
# have json file with name of part 1-32, put ^string into json for each name