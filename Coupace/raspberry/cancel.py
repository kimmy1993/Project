import cv2
import numpy as np

def cancel(pic,pic_smlie):
    cv2.destroyAllWindows()
    #if not  pic==0:
        #per=pic_smlie/pic
    #else:
        #per=0
    result=np.zeros((200,100,3),np.uint8)
    #str_result="result = "+str(pic)+" / "+str(pic_smlie)+"="+str(per)
    if pic_smlie==1:
        str_result="Smile_Point"
        cv2.putText(result, str_result,(0,0),cv2.FONT_HERSHEY_SIMPLEX,1,(255,255,255),3)
    cv2.imshow('result',result)
    """
    if cv2.waitKey(0)&0xFF==ord('q'):
        cv2.destroyAllWindows()
        exit(0)
    cv2.waitKey(1)
    cv2.destroyAllWindows()
    cv2.waitKey(1)
    exit(0)
    """
