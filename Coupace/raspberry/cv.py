import cv2
import os
import numpy as np
import detection
import threading
import cancel
import time
import socket
import vision

#HOST='172.30.1.18'
#PORT=10203

def cv(buf):
    start=time.time()
    pic=0
    pic_smlie=0
    smile=0
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
        result_img=np.zeros((320,240))
        vision_image=frame[0:640,0:480]
        #grayimage=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
        #image=cv2.resize(frame,None,1,1,cv2.INTER_LINEAR)
        
        set_image,result_img=detection.detection(frame,result_img,face_cascade,face2_cascade,lefteye_cascade,righteye_cascade,smile_cascade)
        if set_image==1:
            pic=pic+1
            cv2.imwrite("./buf/set.png",vision_image)
            result.append(result_img)
            #result.append(frame)
        elif set_image==2:
            pic=pic+1
            pic_smlie=pic_smlie+1
            cv2.imwrite("./buf/set.png",vision_image)
            result.append(result_img)
            #result.append()
            
        cv2.imshow('image',frame)
        if cv2.waitKey(1)&0xFF==ord('q'):
            break
        
        end=time.time()
        #print(end-start)
        if (((end-start)>20.0) or (pic==5)):
            for i in range(pic):
                paths="./buf/%d.png" %i
                cv2.imwrite(paths,result[i])
            """
            if pic>0:
                s= socket.socket(socket.AF_INET,socket.SOCK_STREAM)
                s.connect((HOST,PORT))
                s.send(("pipic@@"+str(pic)+"@").encode())
                msg=s.recv(4).decode()
                if (msg==str(1)):
                    for i in range(pic):
                        paths="./buf/%d.png" %i
                        #cv2.imwrite(paths,result[i])
                        s_i=open(paths,'rb')

                        size=os.path.getsize(paths)
                        send_size=int(size/2048)
                        for j in range(send_size+1):
                            byte=s_i.read(2048)
                            print(byte)
                            s.send(byte)

                        #s.send(("@@").encode())
                        #encode_param=[int(cv2.IMWRITE_JPEG_QUALITY),90]
                        #res,imgencode=cv2.imencode('.jpg',send_image,encode_param)
                        #date=np.array(imgencode)
                        #stringdata=date.tostring()
                        msg=s.recv(4).decode()
                        
                elif(msg==str(-1)):
                    print("socket error")
                s.close();
                print(str(end-start))
                #cancel.cancel(pic,pic_smlie)
        """
            #vision_image=cv2.imread("./buf/set.png");
            
            vision_smile=vision.main("1")
            if( ( vision_smile=='POSSIBLE' ) or ( vision_smile=='LIKELY' ) or ( vision_smile=='VERY_LIKELY') ):
                print(vision_smile)
                smile=1
            else:
                smile=0
                
            break
    cancel.cancel(pic,smile)
    cap.release()
    cv2.waitKey(100)
    cv2.destroyAllWindows()
    cv2.waitKey(1)
    #print(1)
    return pic, pic_smlie, smile
