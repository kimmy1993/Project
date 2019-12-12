import cv2
import numpy as np
import math
import histogram as his

LONGAXIS_RATE=13
SHORTAXIS_RATE=18
SIZE_FOREHEAD=20
EYE_LEN= 50
SET_EYE_X=75
SET_EYE_Y=125
PI=3.141592
PRE_HEIGHT=120
PRE_WIDTH=100

def preprocessing(image, l_x,l_y,l_w,l_h, r_x,r_y,r_w,r_h):
    try:
        preImage=np.zeros((240,320,1),np.uint8)
        X = (r_x) - (l_x)
        Y = (r_y) - (l_y)
        angle = math.atan(Y/X)
        pix=0
        scale = EYE_LEN/X;
        
        #왼쪽 눈의 위치를 모두 같은 곳으로 고정시키기 위해서 지정된 위치만큼 어마나 평행이동 해야되는지 계산
        movX = (l_x - SET_EYE_X)*-1
        movY = (l_y - SET_EYE_Y)*-1
        #회전 및 스케일링
        
        Mv=cv2.getRotationMatrix2D((l_x,l_y),angle*180/PI,scale)
        image_r=cv2.warpAffine(image,Mv,(240,320))
        
        #평행이동
        M=np.float32([[1,0,movX],[0,1,movY]])
        image_m=cv2.warpAffine(image_r,M,(240,320))
        
        #타원 마스크와 전처리 이미지의 AND연산
        ovalImage = cv2.imread("mask.png", cv2.IMREAD_GRAYSCALE);
        image_and = cv2.bitwise_and(image_m,ovalImage,ovalImage);

        """
        #타원영역에 해당하는 부분(얼굴영역)을 평활화
        if(his.ovalspaceHistgram(image_and,ovalImage)==False):
            print("error histo")
            return False
        """
        #전처리된 이미지의 크기를 지정
        bufImage=image_and[SET_EYE_Y -17:PRE_HEIGHT+SET_EYE_Y - 17,SET_EYE_X-8:PRE_WIDTH+SET_EYE_X-8]			    						    
    
        return bufImage
    except:
        print("preprocessing error")
        return False
