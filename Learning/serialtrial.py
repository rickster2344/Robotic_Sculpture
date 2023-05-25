import serial
from time import sleep
import json

port = serial.Serial('/dev/cu.usbmodem1463301', baudrate=9600, timeout=1) #Defining port with timeout (if doesnt recieve info after 1 sec)
sleep(2) #ensure arduino is fully init

