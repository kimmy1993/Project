import tkinter as tk
from tkinter import * #리스트박스의 selectmode = EXTENDED 활성화
from tkinter import messagebox
import os
import datetime
import socket
from tkinter import font

#import picamera
#-*- coding: utf-8 -*-




cCOU = "#4980B3"
strOwnID = "1"#"sejong";
#strOwnID = "sejong"
strOwnPWD = "1"#"1234";
unlockButton = False
MasterID = None #사업자로그인 성공시 사업자로그인 등록

"""
아이템리스트
저장형태: [상품번호][이름][가격]
접    근: [인덱스][0 or 1 or 2]

"""
itemList = list()
#itemList = [3,2,1]

#itemListBox = tk.Listbox()

#http://stackoverflow.com/questions/14817210/using-buttons-in-tkinter-to-navigate-to-different-pages-of-the-application



"""
#율곡관
HOST='223.195.19.70' #127 0 0 1localhost
PORT=10203 #서버와 같은 포트사용
"""
#HOST = '112.169.245.254'
#HOST = '223.195.3.155'
#HOST = '112.169.245.101'
#HOST = '192.168.0.12'
HOST = '172.30.1.18'
PORT = 10203

#s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #소켓생성

#소켓 테스트
"""
s.connect((HOST,PORT)) #실행시 소켓연결
message = "치즈김치구이메롱"
message2 = "LOGIN@@김치"
s.send(message2.encode() )
"""

"""
s.connect((HOST,PORT))
s.send(b'Hello, python') #문자를 보냄
data=s.recv(1024) #서버로 부터 정보를 받음
s.close()
print('Received',repr(data))
"""




class Page(tk.Frame): # page는 Frame을 상속받음
    def __init__(self, *args, **kwargs):

        #coupace = tk.PhotoImage(file = "C:\\Users\\macaw\\Documents\\4-1\\캡스톤\\test800480.png")
        #coupaceBG = tk.PhotoImage(file ="C:\\Users\\macaw\\Documents\\4-1\\capstone\\app\\test800480.png")
        #coupace = tk.PhotoImage("test800480.png")
        
        """
        라즈베리파이 bg에서 에러
        """

        #tk.Frame.__init__(self, *args, **kwargs, bg = "#4980B3")#, image = coupace) #"Green")
        tk.Frame.__init__(self, *args, **kwargs)
        ##bgLabel = tk.Label(self, image = coupaceBG)
        ##bgLabel.place(x = 0, y  = 0, relwidth = 1, relheight = 1)

    
        
        #tk.Frame.config(bg = "Green")
        #coupace.place(self, x = 0, y = 0 , relwidth = 1, relheight = 1)

        
    def show(self):
        self.lift()




#촬영페이지
class PageCam(Page):
    
    btnShutter = None
    totalPrice = None
    lblTotalPrice = None
    #salesPerDay = None #일매출액
    #mileage = None


    userID = None # ID 3개중 선택된 1개의 ID

    entryIDList = None # 전송받은 3개의 ID를 받아와 화면에 출력
    
    serialIDList = []

    #적립결제버튼, 마일리지사용결제버튼
    btnSaveMileage = None
    btnUseMileage = None

    #적립, 사용여부 라디오버튼
    radioSave = None
    radioUse = None

    var = None #라디오버튼 값저장. 0적립 1포인트사용
    

    def __init__(self, *args, **kwargs):
        Page.__init__(self, *args, **kwargs)
        label = tk.Label(self, bg = "#4980B3")
        label.pack(side="top", fill="both", expand=True)
        label = tk.Label(self, text="", bg = "#4980B3")
        label.place(width = 200, height = 50, x = 100, y = 300)

        btnShutter = tk.Button(self, text = "촬영", font = (15), command = self.photoShoot)
        btnShutter.place(x = 100, y = 320, width = 100, height = 50)

        self.lblTotalPrice = tk.Label(self, text = "Total : ", bg = "#4980B3", font = (15))
        self.lblTotalPrice.place(width = 200, height = 50, x = 300, y = 250 )

        self.entryIDList = tk.Listbox(self, font = (15) )#, selectmode = EXTENDED)
        self.entryIDList.place( width = 200, height = 100 , x = 300 ,y = 150 )

        #결제버튼
        self.btnSaveMileage = tk.Button(self, text = "결제", font = (15), command = self.sendIDPlusPrice)
        self.btnSaveMileage.place( width = 100, height = 50, x = 450, y = 320)

        #self.btnUseMileage = tk.Button(self, text = "결제 & 사용", font = (15))
        #self.btnUseMileage.place( width = 100, height = 50, x = 400, y = 350)

        #결제, 적립여부
        self.var = IntVar()
        self.radioSave = tk.Radiobutton(self, bg = cCOU, text = "포인트적립", variable = self.var, value = 0, font = (15))
        self.radioSave.place( width = 100, height = 30, x = 300, y = 300)
        self.radioUse = tk.Radiobutton(self, bg = cCOU, text = "포인트사용", variable = self.var, value = 1, font = (15))
        self.radioUse.place( width = 100, height = 30, x = 300 , y = 350)


        ##테스트코드
        """
        self.entryIDList.insert(tk.END, "abcd")
        self.entryIDList.insert(tk.END, "basdf")
        self.entryIDList.insert(tk.END, "sdfasdf")

        self.serialIDList = ["abcd","basdf", "sdfasdf"]
        """
        ##~테스트코드


    def photoShoot(self):
        print("Click")
        self.updateSalesBook() #촬영후 판매기록저장
        print("sendData has activated")
        #self.sendData() #마일리지전송

        #1. 사진찍기
        #self.sendImage() #2. 사진전송
        #self.sendTotalPrice() #계산정보 전송
        self.receiveID()#3. ID 3개받기
        #sendIDPlusPrice()#4. ID 1개 + 계산정보 전송
        print("sendData has finished")
        pass


    def setTotalPrice(self, totalPricePassed):
        self.totalPrice = totalPricePassed
        self.lblTotalPrice.config( text = "Total : " + (str)(self.totalPrice) )#레이블갱신
        pass

    def updateSalesBook(self):
        f = open("SalesBook.txt", 'a')
        """
        data = self.sentence3.get() + "\n"
        f.write(data)
        data = self.sentence1.get() + "\n"
        f.write(data)
        data = self.sentence2.get() + "\n"
        f.write(data)
        """
        now = datetime.datetime.now() # 촬영시간
        nowDate = now.strftime('%Y-%m-%d %H:%M:%S') # 2015-04-19 12:11:32
        #print(nowDate)
        dateAndPrice = nowDate + "\t\t" + "\\" + (str)(self.totalPrice) + "\n"

        #가격정보 출력
        #print(dateAndPrice)

        #f.write(nowDate)
        f.write(dateAndPrice)
        #data = "\n" + (str)( self.totalPrice ) + "\n"
        #f.write(data)
        f.close()

    def sendData(self): #가격정보 전송
        s.connect((HOST,PORT))
        s.send( setMileage )#s.send(b'Hello, python') #문자를 보냄
        data=s.recv(1024) #서버로 부터 정보를 받음
        s.close()
        print('Received',repr(data))

    def sendTotalPrice(self): #가격정보전송 ver2
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #소켓생성
        s.connect((HOST,PORT))
        message = "MONEY" + "@@" + MasterID + "@@" + (str)(self.totalPrice) + "@"
        s.send(message.encode()) #계산가격전송
        s.close()
        print("\\" + (str)(self.totalPrice) + "원 계산되었습니다")

    def setMileage(self):
        return totalPrice * 0.5
        #self.mileage = totalPrice * 0.5

    def sendImage(self):

        f = open("test01.jpg", "r", encoding='UTF8' )
        img = f.read()
        #print(line)

        ###
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((HOST,PORT))
        s.send(img)
        s.close()


    def receiveID(self):

        self.entryIDList.delete(0, END) # 기존리스트박스항목 정리
        self.serialIDList.clear() # 기존에 저장한 3개의 ID 제거


        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((HOST,PORT))
        message = "COPIC" + "@@" + MasterID + "@"
        s.send(message.encode())
        #s.send("COPIC".encode())
        data=s.recv(1024) # ID 3개 받기
        #data = data.decode("utf-8","ignore")
        data = data.decode("utf-8")
        print(data)
        s.close()
        self.serialIDList = data
        self.serialIDList = self.serialIDList.split() #공백기준 문자열 나누기

        #리스트박스에 저장
        for i in range( len( self.serialIDList ) ):
            self.entryIDList.insert(tk.END, self.serialIDList[i] ) #itemList[ len(itemList) -1 ] )
        
        print(self.serialIDList)


    def sendIDPlusPrice(self):
        selectedItems = self.entryIDList.curselection()
        print(selectedItems)
        print(self.var.get())

        ###
        # 첫번째 ~ 세번째 리스트박스 항목에 대해서 선택된 항목에대한 ID를 반환
        if (str)(selectedItems) == "(0,)":
            #print( self.serialIDList[0] )
            self.userID = self.serialIDList[0]
            print(self.userID)
        elif (str)(selectedItems) == "(1,)":
            #print( self.serialIDList[1] )
            self.userID = self.serialIDList[1]
            print(self.userID)
        elif (str)(selectedItems) == "(2,)":
            #print( self.serialIDList[2] )
            self.userID = self.serialIDList[2]
            print(self.userID)
        else:
            print("NNNNNNNOOOOOOOOOOOOOo")
        #print(self.serialIDList)

        #COUSD@@사업자아이디@@회원아이디@@쿠폰사용정보@@가격정보@@
        #message = 'COUSD' + '@@' + MasterID + '@@' + self.userID + '@@' + (str)(self.var.get()) + '@@' + (str)(self.totalPrice) + '@'
        message = "COUSE" + "@@" + MasterID + "@@" + (str)(self.userID) + "@@" + (str)(self.var.get()) + "@@" + (str)(self.totalPrice) + "@"
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((HOST,PORT))
        s.send(message.encode())
        data=s.recv(1024) #서버로 부터 정보를 받음
        s.close()

        #케이스분기
        data = data.decode("utf-8")
        #print('Received',repr(data) )
        if data == 'success':
            print("COUSE success")
            self.entryIDList.delete(0, END) # 기존리스트박스항목 정리
            tk.messagebox.showinfo("", "결제가 완료되었습니다")
        elif data == 'fail':
            tk.messagebox.showinfo("", "결제실패")
        else:
            tk.messagebox.showinfo("", "통신실패")

        
        pass

        

    
        
    


    #일매출액 관리
    """
    def updateSalesPerDay(self):
        self.salesPerDay = self.totalPrice
        
        pass
    """

        


#매출관리 페이지
class PageManagement(Page):

    salesBookListBox = None
    salesBookScrollbar = None

    def __init__(self, *args, **kwargs):
        Page.__init__(self, *args, **kwargs)
        label = tk.Label(self, bg = "#4980B3")
        label.pack(side="top", fill="both", expand=True)
        label = tk.Label(self, text = "매출조회페이지", bg = "#4980B3")
        label.place(width = 200, height = 50, x = 100, y = 0)

        self.salesBookListBox = tk.Listbox(self, selectmode = EXTENDED)
        self.salesBookScrollbar = tk.Scrollbar(self)

        self.salesBookListBox.place(width = 400, height = 280, x = 200, y = 100)
        self.salesBookScrollbar.place(width = 20, height = 280, x = 600, y = 100)

        self.salesBookScrollbar['command'] = self.salesBookListBox.yview
        self.salesBookListBox['yscrollcommand'] = self.salesBookScrollbar.set

        self.loadSalesBook #판매장부 로드

        

        #판매장부 로드
    def loadSalesBook(self):
        
        f = open("SalesBook.txt", "r")
        #line = f.readline() + "\t\t" + f.readline()
        #print(line)
        #self.itemListBox.insert(tk.END, itemList[ len(itemList) -1 ] )
        while True :
            #line = f.readline() + "\t\t" + f.readline()
            line = f.readline()
            if not line : break # 모든라인읽기
            self.salesBookListBox.insert(tk.END, line)
            print(line)
            print("\n")
            
        f.close()

        
    pass
    
        

#결제페이지
class Page1(Page):

   itemOrderBox = None
   itemOrderList = list()
   itemScrollbar = None
   frameButtonGrid = None #버튼그리드
   count = 0; #리스트 카운트용
   lblTotalPrice = None
   totalPrice = 0 # 총구매액

   

   pageCam = None # 촬영프레임

   #미구현
   #vsb = None #프레임스크롤바
   #canvas = None

   btnScrollbar = None
   
    
    
   def __init__(self, *args, **kwargs):
       Page.__init__(self, *args, **kwargs) #, background = cCOU)# "Green")
       label = tk.Label(self, text="", bg = "#4980B3")
       label.pack(side="top", fill="both", expand=True)
       #label.pack(side="top", expand=True)
       

       label = tk.Label(self, text="상품관리", bg = "#4980B3")
       label.pack(side="top", fill="both", expand=True)
    
       self.count = 0


       """ #배경레이블
       coupaceBG = tk.PhotoImage(file ="C:\\Users\\macaw\\Documents\\4-1\\capstone\\app\\test800480.png")
       bgLabel = tk.Label(self, image = coupaceBG)
       bgLabel.place(x = 0, y  = 0, width = 100 ,height = 300)
    """

       
       label.place(width = 200, height = 50, x = 100, y = 300) #레이블은 Page프레임의 전체공간을 사용가능

       buttonPay = tk.Button(self, text ="결제", bg = "#4980B3", command = self.showMessageBox, font = ("나눔스퀘어", 15))
       buttonPay.place(width = 100, height = 50, x = 200, y = 350 )


       """저장된 아이템수에 맞춘 버튼그리드 생성 버튼을누르면 리스트박스에 해당 아이템 저장
"""

       #리스트박스 & 스크롤바
       self.itemOrderBox = tk.Listbox(self, selectmode = EXTENDED)
       self.itemScrollbar = tk.Scrollbar(self)
       """
       self.itemOrderBox.place(width = 150, height = 200, x = 170, y = 100)
       self.itemScrollbar.place(width = 20, height = 200, x = 300, y = 100)
       """
       self.itemOrderBox.place(width = 200, height = 200, x = 170, y = 100)
       self.itemScrollbar.place(width = 20, height = 200, x = 350, y = 100)

       #연결
       self.itemScrollbar['command'] = self.itemOrderBox.yview
       self.itemOrderBox['yscrollcommand'] = self.itemScrollbar.set

       

       #버튼을 담을 프레임
       self.frameButtonGrid = tk.Frame(self, width = 400, height = 200, bg = "Red")
       #self.frameButtonGrid.place(x = 400, y = 100) 추후 함수호출로 초기화시 place가 의미없음
       #self.frameButtonGrid.place(x = 325, y = 100)

       ####################################
       """
       self.canvas = tk.Canvas(self.frameButtonGrid, borderwidth=0, background="#ffffff")
       self.vsb = tk.Scrollbar(self.frameButtonGrid, orient="vertical", command=self.canvas.yview)
       self.canvas.configure(yscrollcommand=self.vsb.set)

       self.vsb.place( x = 100, y = 200, width = 20, height = 100 )#pack(side="right", fill="y")
       self.canvas.place( x = 325, y = 100, width = 400, height = 200)#side="left", fill="both", expand=True)
       """


      
       # create a canvas object and a vertical scrollbar for scrolling it
       """
       vscrollbar = Scrollbar(self, orient=VERTICAL)
       vscrollbar.pack(fill=Y, side=RIGHT, expand=FALSE)

       canvas = Canvas(self, bd=0, highlightthickness=0, yscrollcommand=vscrollbar.set)
       canvas.pack(side=LEFT, fill=BOTH, expand=TRUE)
       vscrollbar.config(command=canvas.yview)

       # reset the view
       canvas.xview_moveto(0)
       canvas.yview_moveto(0)

       # create a frame inside the canvas which will be scrolled with it
       self.interior = interior = Frame(canvas)
       interior_id = canvas.create_window(0, 0, window=interior, anchor=NW)
       """

       """ 미구현 프레임스크롤
       vscrollbar = Scrollbar(self.frameButtonGrid, orient=VERTICAL)
       vscrollbar.pack(fill=Y, side=RIGHT, expand=FALSE)
       #vscrollbar.place( width = 20, height = 200, x = 725, y = 100 )

       #canvas = Canvas(self, bd=0, highlightthickness=0, yscrollcommand=vscrollbar.set)
       canvas = Canvas(self.frameButtonGrid, bd=0, highlightthickness=0, yscrollcommand=vscrollbar.set, bg = "Blue")
       canvas.pack(side=LEFT, fill=BOTH, expand=TRUE)
       #canvas.place( width = 400, height = 200, x = 0, y = 0 )
       

       vscrollbar.config(command=canvas.yview)
       #vscrollbar.config(command = self.frameButtonGrid.yview )

       # reset the view
       canvas.xview_moveto(0)
       canvas.yview_moveto(0)

       # create a frame inside the canvas which will be scrolled with it
       self.interior = interior = Frame(canvas)
       interior_id = canvas.create_window(0, 0, window=interior, anchor=NW)

       """


       



       ####################################

       #총금액레이블
       self.lblTotalPrice = tk.Label(self, text = "Total: 0", bg = "#4980B3", font = (15))
       self.lblTotalPrice.place(width = 200, height = 50, x = 150, y = 300)


       """
       # 촬영페이지 생성
       self.pageCam = PageCam(self)
       #self.pageCam.place(self, x = 0, y = 0)#, relwidth = 1, relheight = 1)

       container=tk.Frame(self)
       #container.pack(side="top", fill="both", expand=True)
       #self.pageCam.place(in_=container, x = 0, y = 0, relwidth = 1, relheight = 1)
       """


       """
       btn = tk.Button(frameButtonGrid, text = "메롱", width = 10, height = 5)
       btn.grid(row = 0, column = 0)

       btn = tk.Button(frameButtonGrid, text = "감자", width = 10, height = 5)
       btn.grid(row = 0, column = 1)
       """

       """
       self.btnScrollbar = tk.Scrollbar(self.frameButtonGrid, orient = "vertical")
       self.btnScrollbar.place( width = 20, height = 200, x = 250, y = 100)
       self.btnScrollbar['command'] = tk.Y
       self.frameButtonGrid['yscrollcommand'] = self.btnScrollbar.set
       """


       """
       if( len(itemList) != 0 ): #리스트에 항목이존재
           for i in itemList:
               button[i] = tk.Button(self, text = itemList[i][1], width = 100, height = 50)
               button.grid(row = 5, column = 0, padx = 5, pady = 5)
       """

       #geometry manager
       #button = tk.Button(frameButtonGrid, text = itemList)
       #button.grid(column = 0, row = 0, sticky = W)






   def _configure_interior(event):
       # update the scrollbars to match the size of the inner frame
       size = (interior.winfo_reqwidth(), interior.winfo_reqheight())
       canvas.config(scrollregion="0 0 %s %s" % size)
       if interior.winfo_reqwidth() != canvas.winfo_width():
           # update the canvas's width to fit the inner frame
           canvas.config(width=interior.winfo_reqwidth())
           interior.bind('<Configure>', _configure_interior)

       def _configure_canvas(event):
           if interior.winfo_reqwidth() != canvas.winfo_width():
               # update the inner frame's width to fill the canvas
               canvas.itemconfigure(interior_id, width=canvas.winfo_width())
               canvas.bind('<Configure>', _configure_canvas)




     

   def buttonLoad(self):
           self.frameButtonGrid = tk.Frame(self, width = 400, height = 200, bg = "Red")
           #self.frameButtonGrid.place(x = 325, y = 100) # 빨간부분배치
           self.frameButtonGrid.place( x = 380, y = 100)
           indexer = 0 #버튼위치 반환용
           for row in range( 5 ): 
              for col in range( 3 ): #기존의 5열에서
                   if self.count < len(itemList):
                       buttonTxt = (str)(itemList[self.count][1]) + "\n \\"+ (str)(itemList[self.count][2])
                       #button = tk.Button(self.frameButtonGrid, text = itemList[self.count][1]+itemList[self.count][2] % (row, col) )
                       #이름과 가격을 버튼텍스트로
                       """
                       button = tk.Button(self.frameButtonGrid, text = self.count, width = 100, height = 50 )
                       button.grid(column = col, row = row, sticky = W)
                       count+=1
                       """
                       
                       #button = tk.Button(self.frameButtonGrid, text = buttonTxt, width = 10, height = 5, command = lambda: self.consolePrint(self.count)   )
                       #button = tk.Button(self.frameButtonGrid, text = buttonTxt, width = 10, height = 5, command = lambda row = row, col = col: self.addListBox(self.count, row, col) )
                       button = tk.Button(self.frameButtonGrid, text = buttonTxt, width = 8, height = 4, command = lambda indexer = indexer: self.addListBox( indexer), wraplength=80, justify=LEFT ) #wraplength = 80
                       button.grid(column = col, row = row) #width = 10, height = 5

                       
                       self.count+=1
                       indexer+=1
               
   def liftPage(self):
       """
       if len( self.itemOrderList ) != 0:
           del self.itemOrderList[:]
           print("itemOderList cleared")
           """
       del self.itemOrderList[:] #구매목록리스트초기화

       #del self.itemOrderBox[:] #리스트박스 초기화

       self.itemOrderBox.delete(0, self.itemOrderBox.size() )
       

       self.count = 0; #카운트초기화
       self.lift()
       self.buttonFrameDestroy()    #버튼그리드리셋
       self.buttonLoad()            #버튼그리드로드
       self.updateTotalPrice()      #가격레이블리셋
       print("page1 up & buttonLoaded\n")
       for i in self.itemOrderList:
            print(i)
       print("\n")
       pass


   def buttonReset(self):
       for row in range( 5 ):
              for col in range( 5 ):
                   if self.count < len(itemList):
                       
                       #buttonTxt = (str)(itemList[self.count][1]) + (str)(itemList[self.count][2])
                       #button = tk.Button(self.frameButtonGrid, text = itemList[self.count][1]+itemList[self.count][2] % (row, col) )
                       #이름과 가격을 버튼텍스트로
                       """
                       button = tk.Button(self.frameButtonGrid, text = self.count, width = 100, height = 50 )
                       button.grid(column = col, row = row, sticky = W)
                       count+=1
                       """
                       #button = tk.Button(self.frameButtonGrid, text = self.count, width = 10, height = 5)
                       #button = tk.Button(self.frameButtonGrid, text = buttonTxt, width = 10, height = 5)
                       #button.grid(column = col, row = row)
                       button.destroy()
                       self.count+=1
       pass

   def buttonFrameDestroy(self):
       #self.frameButtonGrid.Destroy()
       self.frameButtonGrid.grid_forget()
       self.frameButtonGrid.destroy()

   def consolePrint(self, count):
       print("count %d", count)

   #def addListBox(self, count, row, col):
   def addListBox(self, index):

       #buttonTxt = (str)(itemList[ index ][
       
       #self.itemOrderList.append(index)
       
       self.itemOrderList.append( [ itemList[ index ][0], itemList[ index ][1], itemList[ index ][2] ]) #[1] == 상품명, [2] == 가격
       orderBoxTxt =  (str)( self.itemOrderList[ len(self.itemOrderList) -1 ][1] ) +"   "+(str)( self.itemOrderList[ len(self.itemOrderList) -1 ][2] ) # 리스트에서 상품명과 가격만을 받아와 박스에저장
       #self.itemOrderBox.insert(tk.END, self.itemOrderList[ len(self.itemOrderList) -1 ] )
       self.itemOrderBox.insert(tk.END, orderBoxTxt)
       print("addListBox activated")
       print("index: %s" % (index) )
       #print("row: %s      col: %d" % (row, col) )


       for i in self.itemOrderList:
            print(i)
       print("\n")

       self.updateTotalPrice()
       pass

    
   # 가격레이블갱신
   def updateTotalPrice(self):
       self.totalPrice = 0
       for i in range( len( self.itemOrderList) ):
           self.totalPrice += (int)( self.itemOrderList[i][2] )
    
       lblTxt = "Total: " + (str)( self.totalPrice )
       self.lblTotalPrice.config(text = lblTxt)



   def showMessageBox(self):
       response = None
       if len(self.itemOrderList) != 0:     
           messageBoxTxt = "총구매액\n" + "\\" + (str)( self.totalPrice )
           response = tk.messagebox.askyesno("결제", messageBoxTxt ) #askyesnocancel
       else:
           tk.messagebox.showinfo("결제", "선택된 상품이 없습니다")

       if response == True:
           print("Yes")
           #self.pageCam.lift()
           self.showPageCam(self.totalPrice) #총금액도 넘겨준다 


   def setPageCam(self, camPage):
       self.pageCam = camPage
       
       pass

   def showPageCam(self, totalPrice):
       self.pageCam.setTotalPrice(totalPrice)
       self.pageCam.show()
       pass
           

    
       
#관리페이지
class Page2(Page):

   sentence1 = None
   sentence2 = None
   sentence3 = None
   
   itemListBox = None
   itemScrollbar = None

   # 상품명 가격 상품번호 엔트리박스
   txtTradeName = None
   txtTradePrice = None
   txtTradeID = None

   
   isItInitialUpdated = False

   pageManage = None #매출관리페이지

    
   def __init__(self, *args, **kwargs):
       Page.__init__(self, *args, **kwargs)
       label = tk.Label(self, text="상품관리", bg = "#4980B3")
       label.pack(side="top", fill="both", expand=True)

       btnRegItem = tk.Button(self, text = "상품등록", bg = cCOU, command = self.saveItems, font = (15))
       btnRegItem.place(width = 100, height = 50, x = 530, y = 330)

       btnClsItem = tk.Button(self, text = "상품삭제", bg = cCOU, command = self.delSavedItems, font = (15))
       btnClsItem.place(width = 100, height = 50, x = 660, y = 330)
        
       btnManagement = tk.Button(self, text = "매출조회", bg = cCOU, command = self.showPageManagement, font = (15))
       #btnManagement.place(width = 100, height = 50, x = 100, y = 330)
       btnManagement.place(width = 100, height = 50, x = 40, y = 330)
       
       
       btnClearEntry = tk.Button(self, text = "취소", bg = cCOU, command = self.clearEntryBox, font = (15))
       #btnClearEntry.place(width = 100, height = 30, x = 650, y = 380)
       btnClearEntry.place(width = 100, height = 50, x = 170, y = 330)
       self.sentence1 = tk.StringVar() # 입력값
       self.sentence2 = tk.StringVar()
       self.sentence3 = tk.StringVar()

       self.txtTradeName = tk.Entry(self, textvariable = self.sentence1, font = (15))
       self.txtTradeName.place(width = 150, height = 20, x = 350, y = 300)

       self.txtTradePrice = tk.Entry(self, textvariable = self.sentence2, font = (15))
       self.txtTradePrice.place(width = 150, height = 20, x = 350, y = 330 )

       self.txtTradeID = tk.Entry(self, textvariable = self.sentence3, font = (15))
       self.txtTradeID.place(width = 150, height = 20, x = 350, y = 360)

       lblName = tk.Label(self, bg = cCOU, text = "상품명", font = (15))
       lblName.place(width = 60, height =20, x = 280, y = 300)
       lblPrice = tk.Label(self, bg = cCOU, text = "가격", font = (15))
       lblPrice.place(width = 60, height = 20, x = 280, y = 330)
       lblID = tk.Label(self, bg = cCOU, text = "상품번호", font = (15))
       lblID.place(width = 60, height = 20, x = 280, y = 360)

       #리스트박스 & 스크롤바
       self.itemListBox = tk.Listbox(self, selectmode = EXTENDED )
       self.itemScrollbar = tk.Scrollbar(self)
       
       self.itemListBox.place(width = 280, height = 170, x = 270, y = 110)
       self.itemScrollbar.place( width = 20, height = 170, x = 550, y = 110)
       
       

       #리스트 + 스크롤 연결
       self.itemScrollbar['command'] = self.itemListBox.yview
       self.itemListBox['yscrollcommand'] = self.itemScrollbar.set

       

   def updateList(self):
       print("UPDATE ITEM SCROLL LIST")
       pass

       

   def saveItems(self): #상품저장

        #리스트저장
        #itemList.append([self.sentence1.get(), self.sentence2.get(), self.sentence3.get()] )

        #입력이 있을경우 리스트와, 리스트박스에 저장
        if (self.sentence3.get() != "" and self.sentence1.get() != "" and self.sentence2.get() != "" ) :
            itemList.append([self.sentence3.get(), self.sentence1.get(), self.sentence2.get()] )
            self.itemListBox.insert(tk.END, itemList[ len(itemList) -1 ] )
            f = open("itemList.txt", 'a')
            data = self.sentence3.get() + "\n"
            f.write(data)
            data = self.sentence1.get() + "\n"
            f.write(data)
            data = self.sentence2.get() + "\n"
            f.write(data)
            f.close()
        """
        f = open("itemList.txt", 'a')
        data = self.sentence3.get() + "\n"
        f.write(data)
        data = self.sentence1.get() + "\n"
        f.write(data)
        data = self.sentence2.get() + "\n"
        f.write(data)
        f.close()
        """

        #콘솔출력
        #print(itemList[0]);
        for i in itemList:
            print(i)
        print("\n")
        pass


   def delSavedItems(self):
       print("delSavedItems activated")


       #리스트박스상 제거
       selectedItems = self.itemListBox.curselection() # 선택된 항목을 튜플로 반환
       print("SELECTEDITEMS : ")
       print(selectedItems)
       print("\n")

       pos = 0
       for i in selectedItems: # 튜플을 순회하며 해당원소를 삭제
           idx = int(i) - pos
           self.itemListBox.delete(idx, idx)

           itemList.pop(idx) #리스트상 제거
           
           pos = pos + 1


       #itemList.txt 갱신
       f = open("itemList.txt", "w")
       for i in range( len(itemList) ):
           itemNum = itemList[i][0] #[번호] [이름] [가격]
           f.write( itemNum + "\n" )
           itemName = itemList[i][1]
           f.write( itemName + "\n" )
           f.write( itemList[i][2] + "\n" )
       f.close()

       for i in itemList:
            print(i)
       print("\n")


       
       pass

   def liftPage(self):
       print("UPDATE ITEM SCROLL LIST")
       idx = 0

       if self.isItInitialUpdated == False: #초기 1회 호출
           self.isItInitialUpdated = True
           for i in range( len(itemList) ):
               self.itemListBox.insert(tk.END, itemList[i] )
       self.lift()
       pass

   #MainVeiw에서 페이지 받아오기
   def setPageManagement(self, managePage):
       self.pageManage = managePage


   def showPageManagement(self):
       self.pageManage.loadSalesBook()
       self.pageManage.show()
       
   def clearEntryBox(self):
       self.txtTradeName.delete(0, 'end')
       self.txtTradePrice.delete(0, 'end')
       self.txtTradeID.delete(0, 'end')


       
#회원관리페이지
class Page3(Page):

   txtInputID = None
   txtInputPWD = None
   txtInputName = None
   txtInputPhone = None
   txtInputFamily = None
   familyID = None

   sentence1 = None
   sentence2 = None
   sentence3 = None
   sentence4 = None
   sentence5 = None
   #sentence = []


    
   def __init__(self, *args, **kwargs):
       Page.__init__(self, *args, **kwargs)
       label = tk.Label(self, text="", bg = "#4980B3")
       label.pack(side="top", fill="both", expand=True)


       label = tk.Label(self, text = "신규회원등록",fg ="White", bg = "Red")#"#4980bB3")
       label.place(width = 100, height = 20, x = 350 ,y = 100 )

       #입력창안내레이블
       #sf= tkFont.Font(family='Helvetica', size=36, weight='bold')
       lblMemberID = tk.Label(self, text = "ID", bg = cCOU, font = (15))
       lblMemberID.place(width = 100, height = 30, x = 260, y = 150)
       lblPWD = tk.Label(self, text = "PWD", bg = cCOU, font = (15))
       lblPWD.place(width = 100, height = 30, x = 260, y = 190)
       lblName = tk.Label(self, text = "이름", bg = cCOU, font = (15))
       lblName.place(width = 100, height = 30, x = 260, y = 230)
       lblPhone = tk.Label(self, text = "HP", bg = cCOU, font = (15))
       lblPhone.place(width = 100, height = 30, x = 260, y = 270)
       lblFamily = tk.Label(self, text = "Family ID", bg = cCOU, font = (15))
       lblFamily.place(width = 100, height = 30, x = 260, y = 310)

       #가입정보입력
       
       self.sentence1 = tk.StringVar() #ID
       self.sentence2 = tk.StringVar() #PWD
       self.sentence3 = tk.StringVar() #이름
       self.sentence4 = tk.StringVar() #전화
       self.sentence5 = tk.StringVar() #가족ID
       
       """
       for i in range(self.sentence[i]):
           self.sentence[i] = tk.StringVar()
       """
       
       self.txtInputID = tk.Entry(self, textvariable = self.sentence1, font = (15))
       self.txtInputID.place(width = 150, height = 30, x = 360, y = 150)
       self.txtInputPWD = tk.Entry(self, show = "*", textvariable = self.sentence2, font = (15))
       self.txtInputPWD.place(width = 150, height = 30, x = 360, y = 190 )
       self.txtInputName = tk.Entry(self, textvariable = self.sentence3, font = (15))
       self.txtInputName.place(width = 150, height = 30, x = 360, y = 230)
       self.txtInputPhone = tk.Entry(self, textvariable = self.sentence4, font = (15))
       self.txtInputPhone.place(width = 150, height = 30, x = 360, y = 270)
       self.txtInputFamily = tk.Entry(self, textvariable = self.sentence5, font = (15))
       self.txtInputFamily.place(width = 150, height = 30, x = 360, y = 310)

        

       buttonOk = tk.Button(self, text ="가입", bg = "#4980B3", command = self.join, font=(15))
       buttonOk.place(width = 100, height = 50, x = 280, y = 360 )
       buttonCns = tk.Button(self, text ="취소", bg = "#4980B3", command = self.clearEntryBox, font = (15))
       buttonCns.place(width = 100, height = 50, x = 420, y = 360 )

       #text_widget.place(width = 100, height = 50, x = 400, y = 200 )
       


   def join(self):
       #가족ID칸을 제외한 나머지칸에 값이 있을경우 작동
       if (self.sentence1.get() != "" and self.sentence2.get() != ""
           and self.sentence3.get() != "" and self.sentence4.get() != "" ):
           s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #소켓생성
           s.connect((HOST,PORT))
           if (self.sentence5.get() != None): #가족ID가 입력되어있을경우
               self.familyID = self.sentence5.get()
           else:
               self.familyID = self.sentence1().get() #가족ID가 없을경우 자기ID로 대체
           #message = (str)("ADDUS" + "@@" + (str)(self.sentence1.get()) + "@@" + (str)(self.sentence3.get())+ "@@" + (str)(self.sentence2.get()) + "@@" + self.sentence4.get() + "@@" + self.familyID + "@@" + "사진1" + "@@" + "사진2" + "@")
           message = 'ADDUS' + '@@' + self.sentence1.get() + '@@' + self.sentence3.get()+ '@@' + self.sentence2.get() + '@@' + self.sentence4.get() + '@@' + self.familyID + '@@' + '사진1' + '@@' + '사진2' + '@'    
           s.send(message.encode())
           """
        #'euc_kr'
        
        #s.send( message2.encode('utf-8') )
        #s.send(b'LOGIN@@sejong@@1234@@1@')
        #s.sendall(message2)
        #s.send(b'Hello, python') #문자를 보냄
        data=s.recv(1024) #서버로 부터 정보를 받음
        s.close()
        #케이스분기
        data = data.decode("utf-8","ignore")
        print('Received',repr(data) )
        """
           print("JOIN!")
           s.close()

       else:
           tk.messagebox.showinfo("회원가입", "빈칸을 입력하세요")
           #s.close()


   def clearEntryBox(self):
       self.txtInputID.delete(0, 'end')
       self.txtInputPWD.delete(0, 'end')
       self.txtInputName.delete(0, 'end')
       self.txtInputPhone.delete(0, 'end')
       self.txtInputFamily.delete(0, 'end')
       pass


#로그인 페이지
class PageLogin(Page):

    passed = False
    b1 = None
    b2 = None
    b3 = None
    page1 = None
    txtID = None
    txtPWD = None

    
    def __init__(self, *args, **kwargs):
    #def __init__(self, button):
        Page.__init__(self, *args, **kwargs)
        label = tk.Label(self, text = "", bg = "#4980B3")
        label.pack(side = "top", fill = "both", expand = True)

        label = tk.Label(self, text = "사업자로그인", fg = "White", bg = "Red")
        label.place(width = 100, height = 20, x = 350, y = 100)

        #buttonOk = tk.Button(self, text = "로그인", bg = "#4980B3", command = self.checkID )
        buttonOk = tk.Button(self, text = "로그인", bg = "#4980B3", command = self.checkID, font=(15) )
        buttonOk.place(width = 100, height = 50, x = 280, y = 310 )
        buttonCns = tk.Button(self, text ="취소", bg = "#4980B3", command = self.clearEntryBox, font=(15) )
        buttonCns.place(width = 100, height = 50, x = 420, y = 310 )

        self.sentence1 = tk.StringVar() #id pwd저장
        self.sentence2 = tk.StringVar()

        self.txtID = tk.Entry(self, textvariable = self.sentence1, font = (15), width = 1)
        self.txtID.place(width = 140, height = 30, x = 350, y = 215)

        self.txtPWD = tk.Entry(self,show = "*", textvariable = self.sentence2, font = (15))
        self.txtPWD.place(width = 140, height = 30, x = 350, y = 255 )

        label = tk.Label(self, text = "ID", bg = cCOU, font=(15) )
        label.place(width = 40, height = 30, x = 300, y = 215)
        label = tk.Label(self, text = "PWD", bg = cCOU, font=(15) )
        label.place(width = 40, height = 30, x= 300, y = 255)
        

        """
        imgMan = PhotoImage(file = 'blueboy2.png')
        label = Label(self, image = imgMan, borderwidth = 0)
        label.place(x = 645, y = 75)

        imgGirl = PhotoImage(file = 'bluegirl2.png')
        label = Label(self, image = imgGirl, borderwidth = 0)
        label.place(x = -5, y = 50)
        """
        

    def checkID(self):

        print("checkID activated")


        if (self.sentence1.get() == strOwnID) and (self.sentence2.get() == strOwnPWD):
            #b1.config(state = "normal")

            self.passed = True
            print(self.passed)
            global unlockButton #전역변수 바꾸기전 호출
            unlockButton = True

            global MasterID
            MasterID = self.sentence1.get() #아이디를 저장
            print("MasterID : " + MasterID)


            #MainView.buttonUnlock_static(self)
            self.buttonUnlock()

            self.loadItemList() #저장된 제품로드
            self.loadSalesBook() #판매장부 로드
            self.liftPage1() #결제창 메뉴버튼 갱신
            self.destroy() # 창제거
            pass
            #exit()
            return True
        else:
            self.passed = False
            tk.messagebox.showinfo("", "ID 비밀번호를 확인하세요")
            print("재입력\n")
            print(self.passed)
            return False
        pass


    #로그인 연결
    def checkID2(self):

        print("checkID_2 has activated")
        #message = (str)('LOGIN' + '@@' + 'sejong' + '@@' + '1234' + '@@' + '1' + '@')
        message = 'LOGIN' + '@@' + 'sejong' + '@@' + '1234' + '@@' + '0' + '@'
        message2 = 'LOGIN' + '@@' + self.sentence1.get() + '@@' + self.sentence2.get() + '@@' + '0' + '@'
        #s.send(message)
        #s.send(b'LOGIN' + '@@' + self.sentence1.get() + '@@' + self.sentence2.get() + '@@' + '1' + '@')
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #소켓생성
        s.connect((HOST,PORT)) #연결
    
        s.send(message2.encode())
        #'euc_kr'
        
        #s.send( message2.encode('utf-8') )
        #s.send(b'LOGIN@@sejong@@1234@@1@')
        #s.sendall(message2)
        #s.send(b'Hello, python') #문자를 보냄
        data=s.recv(1024) #서버로 부터 정보를 받음
        s.close()
        #케이스분기
        data = data.decode("utf-8","ignore")
        print('Received',repr(data) )
        
        
        #if data.decode() == 'success':
        if data == 'success':
            global unlockButton #전역변수 바꾸기전 호출
            unlockButton = True

            global MasterID
            MasterID = self.sentence1.get() #아이디를 저장
            print("MasterID : " + MasterID)

            #MainView.buttonUnlock_static(self)
            self.buttonUnlock()

            self.loadItemList() #저장된 제품로드
            self.loadSalesBook() #판매장부 로드
            self.liftPage1() #결제창 메뉴버튼 갱신
            self.destroy() # 창제거

            
        else:
            tk.messagebox.showinfo("", "ID 비밀번호를 확인하세요")
            

        pass
        
    

    def setButtons(self,btn1,btn2,btn3):
        print("setButtons")
        self.b1 = btn1
        self.b2 = btn2
        self.b3 = btn3

    def setPage1(self, p1):
        self.page1 = p1

        pass

    def buttonUnlock(self):
        if(unlockButton == True):
            self.b1.config(state = "normal")
            self.b2.config(state = "normal")
            self.b3.config(state = "normal")
        else:
            print("fail unlock Button")

            
    def loadItemList(self):
        """
        #f = open("C:/Python/새파일.txt", 'r')
        f = open("itemList.txt", 'r')
        #line = f.readline()
        #print(line)
        f.close()
        """
        
        if os.path.isfile("itemList.txt"): #물품목록체크
            print("itemlist y")
            f = open("itemList.txt", 'r')
            #line = f.readline()
            #itemList.append(line)
            """            
            while True:
                line = f.readline()
                #line.replace("\n","a")
                #linek = (str)(line)
                print("line start\n")
                print(line)
                print("\nline end")
                line2 = f.readline()
                #line2.replace("\n","a")
                
                line3 = f.readline()
                #line3.replace("\n","")
                if not line: break
                itemList.append([line, line2, line3] )
            """
            """
            while True:
                line = f.readline()
                #line.replace("\n","")
                if not line: break
                line.replace(line[-1],"")
                print(line)
                #print(line[-1])
                itemList.append([line, line2, line3] )
                """
            count = 0;
            while True:
                line = f.readline()
                modifiedLine = line.replace("\n","")
                #linek = (str)(line)
                
                line2 = f.readline()
                modifiedLine2 = line2.replace("\n","")
                
                line3 = f.readline()
                modifiedLine3 = line3.replace("\n","")
                if not line: break
                """
                print("line start\n")
                print(modifiedLine)
                print("\nline end")
                print("line start\n")
                print(modifiedLine2)
                print("\nline end")
                print("line start\n")
                print(modifiedLine3)
                print("\nline end")
                """
                itemList.append([modifiedLine, modifiedLine2, modifiedLine3] )
            
        else :
            print("itemlist n")
            f = open("itemList.txt", 'w')
            f.close()
        
        pass

    def loadSalesBook(self):
        if os.path.isfile("SalesBook.txt"):
            print("SalesBook y")
            f = open("SalesBook.txt", 'r')
            
        else :
            print("SalesBook n")
            f = open("SalesBook.txt", 'w')
            f.close()
            
        pass



    def liftPage1(self):
        print("page1 Lifted")
        self.page1.liftPage()
        pass

        
    def clearEntryBox(self):
        self.txtID.delete(0, 'end')
        self.txtPWD.delete(0, 'end')

       

class MainView(tk.Frame): # MinView는 Frame을 상속받음
    global unlockButton


    
    def __init__(self, *args, **kwargs):
        tk.Frame.__init__(self, *args, **kwargs)
        p1 = Page1(self)
        p2 = Page2(self)
        p3 = Page3(self)

        #pS = PageLogin(self) #로그인페이지

        buttonframe = tk.Frame(self) # 버튼프레임 객체
        #buttonframe.config(bg = "Red") # 버튼프레임색변경
        #buttonframe.config(width = 200, height = 300, x = 200, y = 250)
        container = tk.Frame(self)

        buttonframe.pack(side="bottom", fill="x", expand=False)
        #buttonframe.pack(x = 100, y = 200)
        #buttonframe.place(x = 200, y = 50 )


        """
        coupaceBG = tk.PhotoImage(file ="C:\\Users\\macaw\\Documents\\4-1\\capstone\\app\\test800480.png")
        bgLabel = tk.Label(self, image = coupaceBG)
        bgLabel.place(x = 0, y  = 0, width = 100 ,height = 300)
        """



        
        container.pack(side="top", fill="both", expand=True)

        p1.place(in_=container, x=0, y=0, relwidth=1, relheight=1)
        p2.place(in_=container, x=0, y=0, relwidth=1, relheight=1)
        p3.place(in_=container, x=0, y=0, relwidth=1, relheight=1)
        

        #b1 = tk.Button(buttonframe, text="주문", command=p1.lift) #버튼이 버튼프레임위에 올라감
        b1 = tk.Button(buttonframe, text="주문", command=p1.liftPage, width = 37, height = 2)
        #b2 = tk.Button(buttonframe, text="상품관리", command=p2.lift)
        b2 = tk.Button(buttonframe, text = "상품관리", command = p2.liftPage, width = 37, height = 2)
        b3 = tk.Button(buttonframe, text="고객가입", command=p3.lift, width = 37, height = 2)

        
        b1.pack(side="left", fill = "x")
        b2.pack(side="left", fill = "x")
        b3.pack(side="left", fill = "x")

        #b1.state(["disabled"])
        b1.config(state = "disabled") #비활성화
        b2.config(state = "disabled")
        b3.config(state = "disabled")

        
        """
        b1.place(width = 150, height = 50, x = 100, y = 300)
        b2.place(width = 150, height = 50, x = 100, y = 350)
        b3.place(width = 150, height = 50, x = 100, y = 400)
        """
        #b1.config(buttonframe, x = 100, y = 200)

        pS = PageLogin(self) #로그인페이지
        pS.place(in_=container, x = 0 , y = 0 , relwidth = 1, relheight=1)
        pS.setButtons(b1,b2,b3) # 버튼주기
        pS.setPage1(p1) #페이지1전달, 로그인프레임 소멸전 판매버튼갱신

        pCam = PageCam(self) #촬영페이지
        pCam.place(in_=container, x = 0, y = 0, relwidth = 1, relheight = 1)

        p1.setPageCam(pCam) # 결제페이지에 캠페이지주기

        pManage = PageManagement(self) # 페이지2의 매출관리페이지
        pManage.place(in_=container, x = 0, y = 0, relwidth = 1, relheight = 1)
        p2.setPageManagement(pManage)
        
        p1.show() #결제페이지
        pS.show() #로그인페이지
        #pCam.show()

        ##???
        #check=pS.checkID()
        #label = Label(window, text = MasterID)
        #label.place(width = 100, height = 30, x = 0, y = 0)
        

        """
        if pS.passed == True:
            b1.config(state = "normal")
            b2.config(state = "normal")
            b3.config(state = "normal")
            print("passed\n")

        else:
            print("afsadfafsa")

        
        print("abcdefg")
        """

        


    def mainCheck(self):

        if check == True:
            b1.config(state = "normal")
            b2.config(state = "normal")
            b3.config(state = "normal")
        else:
            print("fail")

        if unlockButton == True:
            b1.config(state = "normal")
            b2.config(state = "normal")
            b3.config(state = "normal")
        else:
            print("fail check global var")

        pass

"""
    @staticmethod
    def buttonUnlock_static(self):
        if(unlockButton == True):
            b1.config(state = "normal")
            b2.config(state = "normal")
            b3.config(state = "normal")
        else:
            print("fail unlock Button")
            """
            
            


        

if __name__ == "__main__":
    window = tk.Tk()
    main = MainView(window)
    main.pack(side="top", fill="both", expand=True)
    window.wm_geometry("800x480")

    window.title("COUPACE")
    window.resizable(width = False, height = False)
    #window.attributes("-fullscreen",True) #전체화면


    #메뉴라벨
    """
    label = tk.Label(window, text = "COUPACE", fg = "white", bg = "#4980B3")
    label.place(x = 300, y = 40)
    label.config(font = ("나눔바른고딕", 30) )
    """
    img = PhotoImage(file = 'coupace.png')
    label = Label(window, image = img, borderwidth = 0, highlightthickness = 0)
    label.place(x = 290 , y = 50 )
    #label.pack()
    #make_label(window, img)

    
    imgMan = PhotoImage(file = 'blueboy3.png')
    label = Label(window, image = imgMan, borderwidth = 0)
    label.place(x = 650, y = 75)

    
    imgGirl = PhotoImage(file = 'bluegirl3.png')
    label = Label(window, image = imgGirl, borderwidth = 0)
    label.place(x = -5, y = 50)
    
    

    
    window.mainloop()
