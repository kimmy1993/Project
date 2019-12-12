import numpy as np
import cv2
import histogram as his


import preprocessing as pre

def detection(image,result_img,face_cascade,face2_cascade,lefteye_cascade,righteye_cascade,smile_cascade):

    global facesize_x, facesize_y,l_x, l_y, l_w, l_h,r_x, r_y, r_w, r_h
    
    x=0
    y=0
    w=0
    h=0
    facesize_x=240
    facesize_y=320
    sets=0
    
    try:
        facerect2 = face2_cascade.detectMultiScale(image,1.2,5,cv2.CASCADE_FIND_BIGGEST_OBJECT|cv2.CASCADE_DO_ROUGH_SEARCH,minSize=(60,60))
        for (f2_x,f2_y,f2_w,f2_h) in facerect2:
                x = f2_x
                y = f2_y
                w = f2_w
                h = f2_h
                
        if (x==0):
            facerect = face_cascade.detectMultiScale(image,1.2,5,cv2.CASCADE_FIND_BIGGEST_OBJECT|cv2.CASCADE_DO_ROUGH_SEARCH,minSize=(60,60))
            for (f_x,f_y,f_w,f_h) in facerect:
                x = f_x
                y = f_y
                w = f_w
                h = f_h
        if(x==0):
            return 0, image
        
        setimage=image[y:y+h,x:x+w]
          
        faces=cv2.resize(setimage,(facesize_x,facesize_y),cv2.INTER_LINEAR);                #입력영상, 결과영상, 필터

        grayface=cv2.cvtColor(faces,cv2.COLOR_BGR2GRAY)
        
        #grayface=his.face_histogram(grayface)
        
        #np.array(his.histogram(grayface))
        #grayface=cv2.imread("./sdf.png",0);
        #grayface=cv2.bilateralFilter(grayface,0,20.0,1.0)
        
        try:
            #lefteyes = lefteye_cascade.detectMultiScale(grayface,1.1,20,minSize=(30,30),maxSize=(70,70),flags=cv2.CASCADE_SCALE_IMAGE)
            #righteyes = righteye_cascade.detectMultiScale(grayface,1.1,10,minSize=(30,30),maxSize=(70,70),flags=cv2.CASCADE_SCALE_IMAGE)
            lefteyes = lefteye_cascade.detectMultiScale(grayface,1.1,5,minSize=(30,30),maxSize=(70,70),flags=cv2.CASCADE_FIND_BIGGEST_OBJECT)
            righteyes = righteye_cascade.detectMultiScale(grayface,1.1,5,minSize=(30,30),maxSize=(70,70),flags=cv2.CASCADE_FIND_BIGGEST_OBJECT)

            l_x=facesize_x
            for (ex,ey,ew,eh) in lefteyes:
                if((l_x>w*2/5)or(l_y>h*2/5)or(l_x+l_w>w*3/5 or l_y+l_h>h*3/5)):
                    l_x=ex
                    l_y=ey
                    l_w=ew
                    l_h=eh
                    
            if l_x==facesize_x:
                return 0, image

            r_x=facesize_x
            for (ex,ey,ew,eh) in righteyes:
                if (((l_x<w/2)and(ex>w/2)) and ((l_y-(l_h/3)<=ey) and ((l_y+l_h/3+10)>=ey))):     	#왼쪽눈이 왼편에 있을시
                   #if((r_x<w*1/2)or(r_y>h*2/5)or(r_x+r_w<w*3/5 or r_y+r_h>h*3/5)):
                    r_x = ex
                    r_y = ey
                    r_h = eh
                    r_w = ew
              
            if l_x>r_x:
                l_x, r_x= r_x, l_x
                l_y, r_y= r_y, l_y
                l_w, r_w= r_w, l_w
                l_h, r_h= r_h, l_h

            for (ex,ey,ew,eh) in lefteyes:                                                              #더 조그마한 눈 검출시
                if((l_x<=ex and l_x+l_w>=ex+ew) and ((l_y<=ey and l_y+l_h*2/5>=ey) and l_y+l_h>=ey+eh)):
                    l_x=ex
                    l_y=ey
                    l_w=ew
                    l_h=eh
                    
            for (ex,ey,ew,eh) in righteyes:
                 if((r_x<=ex and r_x+r_w>=ex+ew) and ((r_y<=ey and r_y+r_h*2/5>=ey) and r_y+r_h>=ey+eh)):
                    r_x = ex
                    r_y = ey
                    r_h = eh
                    r_w = ew

            if r_x==facesize_x:
                return 0, image
            #elif r_x!=facesize_x:
                cv2.rectangle(faces, (l_x,l_y), (l_x+l_w, l_y+l_h),(255,0,0),2)
                #cv2.rectangle(grayface, (l_x,l_y), (l_x+l_w, l_y+l_h),(255,0,0),2)
                #cv2.rectangle(grayface, (r_x,r_y), (r_x+r_w, r_y+r_h),(0,0,255),2)
                cv2.rectangle(faces, (r_x,r_y), (r_x+r_w, r_y+r_h),(0,0,255),2)

        except:
            print("eye error")
            return 0, image
        """
        try:
            smileface=smile_cascade.detectMultiScale(grayface,1.1,3,minSize=(30,30),flags=cv2.CASCADE_SCALE_IMAGE)
                      
            for (s_x,s_y,s_w,s_h) in smileface:
                if ((s_x<facesize_x/2 and (s_x+s_w>facesize_x*1/2 and s_x+s_w<facesize_x*4/5))and(s_y>facesize_y*1/2 and s_y+s_h<facesize_y-20)):
                    cv2.rectangle(faces,(s_x,s_y),(s_x+s_w,s_y+s_h),(255,255,0),2)
                    print("smile")
                    
                    sets=2  
        except:
            print("smile error")
            return 0, image
        """
        #result_img=pre.preprocessing(grayface, l_x,l_y,l_w,l_h, r_x,r_y,r_w,r_h);
        ##cv2.imwrite("./savelast.png",result_img)
        newimage=cv2.resize(faces,(w,h),cv2.INTER_LINEAR);                #입력영상, 결과영상, 필터
        image[y:y+h,x:x+w]=newimage
        cv2.rectangle(image,(x-10,y-10),(x+w+10,y+h+10),(0,255,0),2)
        
        if sets!=2:
            sets=1
        #return sets, result_img
        return sets, grayface
    except:
        print("face error")
        return 0, image

def detection_insert(image,result_img,face_cascade,face2_cascade,lefteye_cascade,righteye_cascade,smile_cascade):

    global facesize_x, facesize_y,l_x, l_y, l_w, l_h,r_x, r_y, r_w, r_h
    
    x=0
    y=0
    w=0
    h=0
    facesize_x=240
    facesize_y=320
    sets=0
    
    try:
        facerect2 = face2_cascade.detectMultiScale(image,1.2,5,cv2.CASCADE_FIND_BIGGEST_OBJECT|cv2.CASCADE_DO_ROUGH_SEARCH,minSize=(60,60))
        for (f2_x,f2_y,f2_w,f2_h) in facerect2:
                x = f2_x
                y = f2_y
                w = f2_w
                h = f2_h
                
        if (x==0):
            facerect = face_cascade.detectMultiScale(image,1.2,5,cv2.CASCADE_FIND_BIGGEST_OBJECT|cv2.CASCADE_DO_ROUGH_SEARCH,minSize=(60,60))
            for (f_x,f_y,f_w,f_h) in facerect:
                x = f_x
                y = f_y
                w = f_w
                h = f_h
        if(x==0):
            return 0, image
        
        setimage=image[y-10:y+h+10,x-10:x+w+10]
          
        faces=cv2.resize(setimage,(facesize_x,facesize_y),cv2.INTER_LINEAR);                #입력영상, 결과영상, 필터

        grayface=cv2.cvtColor(faces,cv2.COLOR_BGR2GRAY)
        
        #grayface=his.face_histogram(grayface)
        
        #np.array(his.histogram(grayface))

        #grayface=cv2.imread("./sdf.png",0);
        #grayface=cv2.bilateralFilter(grayface,0,20.0,1.0)
    
        try:
            #lefteyes = lefteye_cascade.detectMultiScale(grayface,1.1,20,minSize=(30,30),maxSize=(70,70),flags=cv2.CASCADE_SCALE_IMAGE)
            #righteyes = righteye_cascade.detectMultiScale(grayface,1.1,10,minSize=(30,30),maxSize=(70,70),flags=cv2.CASCADE_SCALE_IMAGE)
            lefteyes = lefteye_cascade.detectMultiScale(grayface,1.1,5,minSize=(30,30),maxSize=(70,70),flags=cv2.CASCADE_FIND_BIGGEST_OBJECT)
            righteyes = righteye_cascade.detectMultiScale(grayface,1.1,5,minSize=(30,30),maxSize=(70,70),flags=cv2.CASCADE_FIND_BIGGEST_OBJECT)

            l_x=facesize_x
            for (ex,ey,ew,eh) in lefteyes:
                if((l_x>w*2/5)or(l_y>h*2/5)or(l_x+l_w>w*3/5 or l_y+l_h>h*3/5)):
                    l_x=ex
                    l_y=ey
                    l_w=ew
                    l_h=eh
                    
            if l_x==facesize_x:
                return 0, image

            r_x=facesize_x
            for (ex,ey,ew,eh) in righteyes:
                if (((l_x<w/2)and(ex>w/2)) and ((l_y-(l_h/3)<=ey) and ((l_y+l_h/3+10)>=ey))):     	#왼쪽눈이 왼편에 있을시
                   #if((r_x<w*1/2)or(r_y>h*2/5)or(r_x+r_w<w*3/5 or r_y+r_h>h*3/5)):
                    r_x = ex
                    r_y = ey
                    r_h = eh
                    r_w = ew
              
            if l_x>r_x:
                l_x, r_x= r_x, l_x
                l_y, r_y= r_y, l_y
                l_w, r_w= r_w, l_w
                l_h, r_h= r_h, l_h

            for (ex,ey,ew,eh) in lefteyes:                                                              #더 조그마한 눈 검출시
                if((l_x<=ex and l_x+l_w>=ex+ew) and ((l_y<=ey and l_y+l_h*2/5>=ey) and l_y+l_h>=ey+eh)):
                    l_x=ex
                    l_y=ey
                    l_w=ew
                    l_h=eh
                    
            for (ex,ey,ew,eh) in righteyes:
                 if((r_x<=ex and r_x+r_w>=ex+ew) and ((r_y<=ey and r_y+r_h*2/5>=ey) and r_y+r_h>=ey+eh)):
                    r_x = ex
                    r_y = ey
                    r_h = eh
                    r_w = ew

            if r_x==facesize_x:
                return 0, image
            #elif r_x!=facesize_x:
                #cv2.rectangle(faces, (l_x,l_y), (l_x+l_w, l_y+l_h),(255,0,0),2)
                #cv2.rectangle(grayface, (l_x,l_y), (l_x+l_w, l_y+l_h),(255,0,0),2)
                #cv2.rectangle(grayface, (r_x,r_y), (r_x+r_w, r_y+r_h),(0,0,255),2)
                #cv2.rectangle(faces, (r_x,r_y), (r_x+r_w, r_y+r_h),(0,0,255),2)
        except:
            print("eye error")
            return 0, image
        
        #result_img=pre.preprocessing(grayface, l_x,l_y,l_w,l_h, r_x,r_y,r_w,r_h);
        newimage=cv2.resize(faces,(w+20,h+20),cv2.INTER_LINEAR);                #입력영상, 결과영상, 필터
        image[y-10:y+h+10,x-10:x+w+10]=newimage
        #cv2.rectangle(image,(x,y),(x+w,y+h),(0,255,0),2)
        paths="./insert/0.png"
        grayimage=cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
        cv2.imwrite(paths,grayimage)
        
        if sets!=2:
            sets=1
        return sets, image
    
    except:
        print("face error")
        return 0, image
