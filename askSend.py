import serial
import time

port = serial.Serial('COM4', baudrate=1200, timeout=1)

seconds = time.time()
def write_read(x):
    port.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = port.readline()
    return data

line = ''
value = '20'

while (True):
    if(port.inWaiting() > 0):
        line = port.readline()
        print(line)
        if (line == b'request\r\n'):
            #linux uses linefeed, windows carriagereturnlinefeed, macox uses carriagereturn
            #line: b'request\r\n' therefore is crlf
            port.write(value.encode('ascii'))
