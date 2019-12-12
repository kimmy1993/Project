import cv2
import os
import numpy as np
import detection
import threading
import cancel
import time
import socket

#HOST='172.30.1.18'
#PORT=10203

def insert(buf):
    start=time.time()
    pic=0
    pic_smlie=0
    result=[]
    try:
        face_cascade=cv2.CascadeClassifier("/home/pi/Desktop/project/haarcascade_frontalface_default.xml")
        face2_cascade=cv2.CascadeClassifier("/home/pi/Desktop/project/lbpcascade_frontalface.xml")
        lefteye_cascade=cv2.CascadeClassifier("/home/pi/Desktop/project/haarcascade_mcs_lefteye.xml")
        righteye_cascade=cv2.CascadeClassifier("/home/pi/Desktop/project/haarcascade_mcs_righteye.xml")
        smile_cascade=cv2.CascadeClassifier("/home/pi/Desktop/project/haarcascade_smile.xml")
    except:
        print("검출기 예러 ")
        exit(0)
    
    os.system('sudo modprobe bcm2835-v4l2')
    cap = cv2.VideoCapture(0)
    cap.set(3,480)
    cap.set(4,640)
    
    while True:
        set_image=0
        ret, frame = cap.read()
        if not ret:
            print(' video error')
            break
        
        #frame=cv2.imread("./picture/2.jpg");
        result_img=np.zeros((120,100))
        #grayimage=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
        #image=cv2.resize(frame,None,1,1,cv2.INTER_LINEAR)
        
        set_image,result_img=detection.detection_insert(frame,result_img,face_cascade,face2_cascade,lefteye_cascade,righteye_cascade,smile_cascade)
        if set_image==1:
            pic=pic+1
            result.append(result_img)
        elif set_image==2:
            pic=pic+1
            pic_smlie=pic_smlie+1
            result.append(result_img)
        
        #cv2.imshow('image',frame)
        if cv2.waitKey(1)&0xFF==ord('q'):
            break
        
        end=time.time()
        if (((end-start)>20.0) or (pic==1)):
        
            cap.release()
            #cv2.destroyAllWindows()
            break
        
    return pic

