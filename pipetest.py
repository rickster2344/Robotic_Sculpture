import cv2
import mediapipe as mp
import numpy as np
import json
import serial 
import time
from google.protobuf.json_format import MessageToJson

mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose
numLandmarks = 33
numVariables = 4
numDigitsPerVar = 5

port = serial.Serial('/dev/cu.usbmodem1463301', baudrate=115200, timeout=1) #Defining port with timeout (if doesnt recieve info after 1 sec)
time.sleep(2) #ensure arduino is fully init

cap = cv2.VideoCapture(0) #v cap device id=0

with mp_pose.Pose(min_detection_confidence=0.5, min_tracking_confidence=0.5) as pose: #rename mp_pose class to pose

  while cap.isOpened():
    ret, frame = cap.read() #extract ret and frame from webcam


    #Recolour feed
    image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB) #
    #Make detection
    results = pose.process(image) #pass image to model
    
    #pose landmarks are stored in results, check index in mediaframe github
    # print(results.pose_landmarks)
  
    #Recolour opencv image back to BGR to display
    image= cv2.cvtColor(image,cv2.COLOR_RGB2BGR)

    #Draw pose landmarks
    mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_pose.POSE_CONNECTIONS,mp_drawing.DrawingSpec(color=(50,50,150), thickness=2, circle_radius=2), mp_drawing.DrawingSpec(color=(0,0,255), thickness=2, circle_radius=2))
    #recolouring ^

    # Export coords
    try:
      poses = results.pose_landmarks.landmark #try putting into your own protobuf thing, then use protobuf in arduino
      poses_row = [[round(landmark.x,5), round(landmark.y,5), round(landmark.z,5), round(landmark.visibility,5)] for landmark in poses]

    except:
      pass
      
    # print(poses)
    pose_json = json.dumps(poses_row) + str('\n')
    # for i in range(0,numLandmarks):
    #   for j in range(0,numVariables):
    #     port.write(poses_row[i][j])
    port.write(pose_json.encode())
    # print(pose_json)


    print(port.readline())


    #display and intterupt
    cv2.imshow('Name', image)
     #after 1 sec, if q pressed, break
    if cv2.waitKey(1) & 0xFF == ord('q'): 
      break

cap.release()
cv2.destroyAllWindows()
