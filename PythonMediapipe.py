# from email import message
from re import ASCII
import cv2
import mediapipe as mp
import numpy as np
# import json
import serial 
import time
# from google.protobuf.json_format import MessageToJson

mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose
numLandmarks = 33
numVariables = 4
numDigitsPerVar = 5
lastline = ''
active = False
minY = 0
maxY = 0

port = serial.Serial('COM4', baudrate=115200, timeout=1) #Defining port with timeout (if doesnt recieve info after 1 sec), can increase baudrate if needed
time.sleep(1) #ensure arduino is fully init

cap = cv2.VideoCapture(0) #v cap device id=0
port.write('s'.encode('ascii'))
with mp_pose.Pose(min_detection_confidence=0.5, min_tracking_confidence=0.5) as pose: #rename mp_pose class to pose

  while cap.isOpened():
    ret, frame = cap.read() #extract ret and frame from webcam

    #Recolour feed, detegtion works in RGB
    image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB) 
   
    #Make detection
    results = pose.process(image) #pass image to model
    #pose landmarks are stored in results, check index in mediaframe github
  
    #Recolour opencv image back to BGR to display
    image= cv2.cvtColor(image,cv2.COLOR_RGB2BGR)

    #Draw pose landmarks
    mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_pose.POSE_CONNECTIONS, mp_drawing.DrawingSpec(color=(50,50,150), thickness=2, circle_radius=2), mp_drawing.DrawingSpec(color=(0,0,255), thickness=2, circle_radius=2))
    #recolouring ^

    # Export coords
    try:
      poses = results.pose_landmarks.landmark #try putting into your own protobuf thing, then use protobuf in arduino
      # poses_row = [[round(landmark.x,5), round(landmark.y,5), round(landmark.z,5), round(landmark.visibility,5)] for landmark in poses] #if all points are used
      poses_y = [round(landmark.y * 500) for landmark in poses] #only y used

    except:
      pass
    else:
      active = True

    if (active == True):
      # if (poses_y[15] < minY):#not necessary if min normalised to 0
      #   minY = poses_y[15]
      if (poses_y[15]> maxY):
        maxY = poses_y[15]

    # port.write(str(poses_y[15]).encode('ascii'))
    
      if(port.inWaiting() > 0):
          line = port.readline()
          print(line)
          if (lastline != line):
            if (line == b'r\r\n') :
                #linux uses linefeed, windows carriagereturnlinefeed, macox uses carrqiagereturn
                #line: b'request\r\n' therefore is crlf
                msg = poses_y[15]
                if (msg < 0):
                  msg = 0

                stringMes = str(msg)  
                print(f'Message sent: {msg}')
                numberarr = list(stringMes)
                for number in reversed(numberarr):
                  port.write(number.encode('ascii'))
                port.write('f'.encode('ascii')) #f for finished, 
            elif (line == b'c\r\n'):
              port.write('g'.encode('ascii'))
          lastline = line
  
    #display and intterupt
    cv2.imshow('Name', image)
     #after 1 sec, if q pressed, break
    if cv2.waitKey(1) & 0xFF == ord('q'):
      print(f'maxY: {maxY}, minY: {minY}')
      break

cap.release()
cv2.destroyAllWindows()