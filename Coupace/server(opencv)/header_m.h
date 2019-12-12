#pragma once
#pragma comment(lib,"ws2_32")                                       //ws2_32.lib 라이브러리 파일을 컴파일러에 포함(컴파일러 지시자) 
#include <WinSock2.h>                                             // sys/socket.h 윈도우 버전
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h> 
#include <stdio.h>
#include <my_global.h>
#include <winsock2.h>
#include <mysql.h>
#include "cv.h"
#pragma comment(lib,"libmySQL.lib")


char* query_message(char*, int, cv_data, SOCKET);
void login(char* , char*);
void add_adder(char*, char*,SOCKET);
void add_pic(char*, char*, SOCKET, char*, int);
void del_adder(char* , char*);
int coupon_add(char* , char* , int , char*, int);
int coupon_use(char* , char* , int , int, char*, int);
int coupon_pic(char*, SOCKET, cv_data);
void coupon_search(char*, char*);
void coupon_load(char* , char* , int, char*);
void add_own_sells(char*, char*, int);
void sells_search(char*, char*);
void store_data(char* , char*,int);						//회원아이디로 접속, 쿠폰 정보 요청시 사업자 가게이름을 찾는 함수
void pic_query(char*,  int);
void search_fam_id(char*, char*);
void pic_search(char*, char*);
void login_app(char*, char*);
void file_return();