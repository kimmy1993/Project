import cv2
import numpy as np
import math
from array import *

LONGAXIS_RATE=13
SHORTAXIS_RATE=18
SIZE_FOREHEAD=20
EYE_LEN= 50
SET_EYE_X=75
SET_EYE_Y=125
PI=3.141592
PRE_HEIGHT=120
PRE_WIDTH=100

def histogram(img):
    try:
        clahe = cv2.createCLAHE(clipLimit=5.0,tileGridSize=(4,4))   #Contrast Limited Adaptive Histogram Equalization 이미지를 작은 title형태로 나누어 그 title안에서 Equalization을 적용하는 방식,contrast limit라는 값을 적용하여 이 값을 넘어가는 경우는 그 영역은 다른 영역에 균일하게 배분하여 적용
        histoimage=[]
        #b=np.zero()
        #왼쪽 확대 화면  그 이후 Contrast Limited Adaptive Histogram Equalization 적용
        #오른쪽 확대 화면 그 이후 Contrast Limited Adaptive Histogram Equalization 적용
        left_buf=img[0:320,0:120]
        right_buf=img[0:320,120:240]
        
        left_image=clahe.apply(left_buf)
        middle_image=clahe.apply(img)
        right_image=clahe.apply(right_buf)
        
        for y in range(0,320):
            histo_x=[]
            for x in range(0,240):
                value=0
                if(x<240/4):
                    left=left_image[y, x]
                    value=left
                elif(x<240/2):
                    left=left_image[y, x]
                    middle=middle_image[y, x]
                    f =(x-240*1/4)/(240.0/4)
                    value=round((1.0-f)*left+(f)*middle)
                elif(x<240*3/4):
                    right=right_image[y,x-120]
                    middle=middle_image[y,x]
                    f =(x-240*1/2)/(240.0/4)
                    value=round((1.0-f)*middle+(f)*right)
                else:
                    right=right_image[y,x-120]
                    value=right
       
                if(value<0.0):
                    value=0
                elif(value>255.0):
                    value=255
                histo_x.append(value)
            histoimage.append(histo_x)
        a=np.array(histoimage,dtype=np.float64)
        cv2.imwrite("./sdf.png",a)
     
        return histoimage
    except:
        print("히스토그램 에러")
        return img

def face_histogram(img):
    try:
        clahe = cv2.createCLAHE(clipLimit=5.0,tileGridSize=(4,4))   #Contrast Limited Adaptive Histogram Equalization 이미지를 작은 title형태로 나누어 그 title안에서 Equalization을 적용하는 방식,contrast limit라는 값을 적용하여 이 값을 넘어가는 경우는 그 영역은 다른 영역에 균일하게 배분하여 적용
        img=clahe.apply(img)

        return img
    
    except:
        print("히스토그램 에러")
        return img
    
def ovalspaceHistgram(image,ovalImage):
    try:
        sums=0.0
        high=0.0
        hist=np.zeros((1,256),dtype=np.int64)
        sum_hist=np.zeros((1,256),dtype=np.int64)
        ovalspace=0
        result=np.zeros((120,100),dtype=np.int64)

        for i in range(120):
            for j in range(100):
                if ovalImage[i,j]==255:
                    print("2")
                    hist[image[i,j]]=hist[image[i,j]]+1
                    print("1")
                    ovalspace=ovalspace+1
                    print("3")  
        for i in range(256):
            if(hist[i]!=0):
                high=i
            sums+=hist[i]
            sum_hist[i]=sums

        for i in range(256):
            for j in range(256):
                if ovalImage[i,j]==255:
                    result[i,j]=sum_hist[image[i,j]*high/ovalspace]
                    image=np.array(result)
                    
    except:
        print("히스토그램 에러")
        return false
    
    return true
