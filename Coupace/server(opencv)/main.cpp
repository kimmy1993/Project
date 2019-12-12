#include "header_m.h"

#define MYPORT 10203                                                //현재 포트 번호
#define BUFSIZ 4097                                               //버퍼 사이즈 지정
#define WAIT 10                                                   //응답 대기자 수

HANDLE hWriteEvent;                                                //한 쓰래드가 사용중일때 접근하지 못하게 하기 위한 핸들러(동시에 같은 파일 접근 금지)
cv_data cv_load;

char* analysis_message(char *message, int msg_len, SOCKET sock);               //받은 메세지를 분석해주는 함수
void creatmessage(char *rec_msg, SOCKET sock);                           //메세지 만드는 함수(받은 메세지, 소켓 정보)
DWORD WINAPI Connect_process(LPVOID socket);                           //받은 요청을 쓰래드로 분리하기 위한 함수


int main()
{
	SOCKET sockfd, new_fd;                                          //socket 기술자 선언
	struct sockaddr_in my_addr;                                       // my address 주소를 표현하는 구조체 
	struct sockaddr_in connect_addr;                                 // connector's address information 

	int sin_size;                                                //socket_addr 구조체의 크기
	WSADATA wsaDATA;                                             //WSADATA구조체 함수
	int rec_msg_lens = 0;
	HANDLE hThread;                                                //스래드 핸들을 저장할 변수

	cv_load.cv_clear();
	cv_load.cv_trainning();

	WSAStartup(MAKEWORD(2, 2), &wsaDATA);                                 //tcp/ip를 사용할수 있도록 운영체제 함수 호출(원도우용)

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if ((sockfd) == INVALID_SOCKET)                                    //make socket name= sockfd, domain=AF_INET(주소 체계,pp_inet사용가는 (프로토콜 체계)), type= SOCK_STREAM(tcp), protocol =0
	{
		perror("socket");
		exit(1);
	}

	my_addr.sin_family = AF_INET;                                       // 주소 쳬계이므로 항상 AF_INET설정
	my_addr.sin_port = htons(MYPORT);                                    // port number-> network byte로 바꿔 전송해야함
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);                           // IP adrress INADDR_ANY는 동작하는 서버의 주소를 자동으로 할당 또한 여러 ip를 할당 받았을때 port만 같으면 수신 받을 수 있음
	memset(&(my_addr.sin_zero), 0, 8);                                 //bzero(주소,크기)해당된 주소부터 0을 만드는 시스템 콜(8바이트)->리눅스사용하는듯 memset으로 대체

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) // sockfd(소켓 기술자), my_addr는 ip주소정보를 가지고 있는 struct sockaddr_in 에 대한 포인터 sockaddr 형으로 변환,구조체의 사이즈
	{                                                         // 이것은 소켓 기술자(소켓을 가르키는 것)과 주소정보를 연결해주는 함수이다 즉 소켓에 ip주소와 portnum을 할당
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, WAIT) != 0)                                       // 연결을 기다리는 함수, 소켓 기술자에 얼마나 많은 대기자를 설정할 수 있는지 설정
	{
		perror("listen");
		exit(1);

	}

	u_long on = 0;																//0일경우 멈춤, 1일경우 안멈춤
	if (ioctlsocket(sockfd, FIONBIO, &on) == SOCKET_ERROR)                     //비동기 상태(멈춰 대기한는 것이 아닌 어떠한 결과든 출력해 프로그램이 동작하게 만들어 주기 위한 것)를 위한 선언, accept시 멈춘 상태를 방지 하기 위함
	{
		perror("socket");
		exit(1);
	}

	printf("wait...\n");                                          //서버가 시작해서 돌아가기 시작할때 출력

	hWriteEvent = CreateEvent(NULL, FALSE, TRUE, NULL);                        //이벤트 헨들러 설정(보안 속성,커널오브젝트 종류(auto-rest),signal종류(신호 상태->접근 가능),이벤트 오브젝트의 이름설정)
	if (hWriteEvent == NULL) return 1;

	while (1)
	{
		sin_size = sizeof(struct sockaddr_in);                           //소켓 주소 수조체 변수크기로 초기화(정수형 변수)

		while (1)
		{

			if ((new_fd = accept(sockfd, (struct sockaddr *)&connect_addr, &sin_size)) == INVALID_SOCKET)         //새로운 소켓 파일 기술자로 연결시키게 된다. 원래의 소켓은 계속 listen중
			{
				Sleep(100);

				if (WSAGetLastError() != WSAEWOULDBLOCK)                                          //반환값이 에러일 경우                                          
				{
					perror("accept");
					break;
				}
				continue;                                                               //계속 진행한다
			}
			break;                                                                  //새로운 데이터 접속시 종료하고 쓰래드를 연결시키기 위해 while을 나간다
		}
		printf("server: got connection from %s\n", inet_ntoa(connect_addr.sin_addr));               //접속한 위치를 출력해준다.inet_ntoa로 네트워크 빅 에디안 데이터를 스몰에디안 정수형으로 변환

		hThread = CreateThread(NULL, 0, Connect_process, (LPVOID)new_fd, 0, NULL);                        //접속한 클라이언트를 처리할 스레드 생성 그리고 그것을 참조할 수 있는 스레드 핸들 리턴(상속:보안 디스크립터 정보, 스레드에 할당하는 스택크기(할당 하지 않을시 1mb), 스레드 함수의 시작주소,스레드 함수에 전달한 인자, 스레드 생성을 제어하는 값(0일시 바로 실행), 스레드 id반환변수(DWORD형))
		if (hThread == NULL)                                                            //오류 발생시 새 소켓 닫음(연결 종료)
		{
			closesocket(new_fd);
		}
		else                                                                     //스레드 생성된경우 핸들에 접근할 필요 없을시 핸들 제거(핸들 제거해도 스레드가 삭제되지 않는다) 
		{
			CloseHandle(hThread);
		}
	}
	WaitForSingleObject(hThread, INFINITE);                                                //모든 스레드가 끝날때까지 기다려 준다.
	closesocket(sockfd);                                                            //소켓 종료
	WSACleanup();                                                                  //wsacleanup 함수 종료

	return 0;
}

DWORD WINAPI Connect_process(LPVOID socket)                                             //받은 요청을 쓰래드로 분리했을때 실행되는 함수
{
	WaitForSingleObject(hWriteEvent, INFINITE);                                          //다른 쓰래드가 사용중 일시 대기한느 것

	char buf[BUFSIZ];                                                            //받은 메세지
	int msg = 0;                                                                  //전달받은 메세지의 크기

	SOCKET connect_sock = (SOCKET)socket;                                                //전달받은 인자를 socket으로 형변환해서 사용한다.

	msg = recv(connect_sock, buf, BUFSIZ - 1, 0);                                             //메세지를 받는다. msg는 받은 메세지의 크기를 나타낸다.
	if (msg == -1)
		printf("%d", msg);
	if (msg == SOCKET_ERROR)                                                         // 에러 날 경우
	{
		closesocket(connect_sock);                                                   // 소켓 종료
		SetEvent(hWriteEvent);                                                      // 다 썻다고 알림
		return 0;                                                               // 함수 종료
	}
	buf[msg] = '\0';                                                               //마지막에 null문자 추가

	creatmessage(buf, connect_sock);                                                   //메세지 생성, 보내는 함수	

	closesocket(connect_sock);
	printf("close_socket\n");
	SetEvent(hWriteEvent);                                                         //읽고 쓴 후 알리는 기능 종료때까지 중복 가능성 존재하기 때문

	return 0;
}

void creatmessage(char *rec_msg, SOCKET sock)                                          //메세지를 만들고 보내주는 함수
{
	static char sendmsg[4097];                                                      //보내는 메세지
	int msg_len = strlen(rec_msg);                                                   //메세지 길이 확인 및 반복을 위함
	int set = 0;
	char* return_data;


	char* creat_buf = (char*)malloc(sizeof(char)*(msg_len + 1));


	memset(sendmsg, '\0', 4097);                                                      //opendata 초기화

	strcpy(creat_buf, rec_msg);                                                      //받은 메세지를 버퍼에

	printf("\nrequest message:%s\n", rec_msg);      //받는 정보 출력 복사

	return_data = analysis_message(creat_buf, msg_len, sock);                                       //받은 메세지 버퍼와 길이를 가지고 분석해 필요한 요소만 구조체에 저장

	strcpy(sendmsg, return_data);


	printf("%s", return_data);

	send(sock, sendmsg, strlen(sendmsg), 0);

}

char* analysis_message(char *message, int msg_len, SOCKET sock)                                                //받은 메세지를 분석하는 함수(받은 메세지, 메세지 길이)
{
	int i = 0;
	int j = 1;
	int k = 0;
	char save_met[9][6] = { "LOGIN","ADDUS","USPIC","DELUS","LOGIA","COUSE","COPIC","COSEE","MONEY" };
	char methood[6];
	char *arr;                                                                                 //임시 저장 공간


	for (j = 0; j < 5; j++)
		methood[j] = message[j];

	methood[5] = '\0';

	for (j = 0; j < 10; j++)
		save_met[j][5] = '\0';


	if (strcmp(methood, save_met[0]) == 0)                                                            //메소드비교
		arr = query_message(message, 0, cv_load, sock);
	else if (strcmp(methood, save_met[1]) == 0)
		arr = query_message(message, 1, cv_load, sock);
	else if (strcmp(methood, save_met[2]) == 0)
		arr = query_message(message, 2, cv_load, sock);
	else if (strcmp(methood, save_met[3]) == 0)
		arr = query_message(message, 3, cv_load, sock);
	else if (strcmp(methood, save_met[4]) == 0)
		arr = query_message(message, 4, cv_load, sock);
	else if (strcmp(methood, save_met[5]) == 0)
		arr = query_message(message, 5, cv_load, sock);
	else if (strcmp(methood, save_met[6]) == 0)
		arr = query_message(message, 6, cv_load, sock);
	else if (strcmp(methood, save_met[7]) == 0)
		arr = query_message(message, 7, cv_load, sock);
	else if (strcmp(methood, save_met[8]) == 0)
		arr = query_message(message, 8, cv_load, sock);
	else
		arr = "fail";

	return arr;


}