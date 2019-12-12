#include "header_m.h"

#define MYPORT 10203                                                //���� ��Ʈ ��ȣ
#define BUFSIZ 4097                                               //���� ������ ����
#define WAIT 10                                                   //���� ����� ��

HANDLE hWriteEvent;                                                //�� �����尡 ������϶� �������� ���ϰ� �ϱ� ���� �ڵ鷯(���ÿ� ���� ���� ���� ����)
cv_data cv_load;

char* analysis_message(char *message, int msg_len, SOCKET sock);               //���� �޼����� �м����ִ� �Լ�
void creatmessage(char *rec_msg, SOCKET sock);                           //�޼��� ����� �Լ�(���� �޼���, ���� ����)
DWORD WINAPI Connect_process(LPVOID socket);                           //���� ��û�� ������� �и��ϱ� ���� �Լ�


int main()
{
	SOCKET sockfd, new_fd;                                          //socket ����� ����
	struct sockaddr_in my_addr;                                       // my address �ּҸ� ǥ���ϴ� ����ü 
	struct sockaddr_in connect_addr;                                 // connector's address information 

	int sin_size;                                                //socket_addr ����ü�� ũ��
	WSADATA wsaDATA;                                             //WSADATA����ü �Լ�
	int rec_msg_lens = 0;
	HANDLE hThread;                                                //������ �ڵ��� ������ ����

	cv_load.cv_clear();
	cv_load.cv_trainning();

	WSAStartup(MAKEWORD(2, 2), &wsaDATA);                                 //tcp/ip�� ����Ҽ� �ֵ��� �ü�� �Լ� ȣ��(�������)

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if ((sockfd) == INVALID_SOCKET)                                    //make socket name= sockfd, domain=AF_INET(�ּ� ü��,pp_inet��밡�� (�������� ü��)), type= SOCK_STREAM(tcp), protocol =0
	{
		perror("socket");
		exit(1);
	}

	my_addr.sin_family = AF_INET;                                       // �ּ� �ǰ��̹Ƿ� �׻� AF_INET����
	my_addr.sin_port = htons(MYPORT);                                    // port number-> network byte�� �ٲ� �����ؾ���
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);                           // IP adrress INADDR_ANY�� �����ϴ� ������ �ּҸ� �ڵ����� �Ҵ� ���� ���� ip�� �Ҵ� �޾����� port�� ������ ���� ���� �� ����
	memset(&(my_addr.sin_zero), 0, 8);                                 //bzero(�ּ�,ũ��)�ش�� �ּҺ��� 0�� ����� �ý��� ��(8����Ʈ)->����������ϴµ� memset���� ��ü

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) // sockfd(���� �����), my_addr�� ip�ּ������� ������ �ִ� struct sockaddr_in �� ���� ������ sockaddr ������ ��ȯ,����ü�� ������
	{                                                         // �̰��� ���� �����(������ ����Ű�� ��)�� �ּ������� �������ִ� �Լ��̴� �� ���Ͽ� ip�ּҿ� portnum�� �Ҵ�
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, WAIT) != 0)                                       // ������ ��ٸ��� �Լ�, ���� ����ڿ� �󸶳� ���� ����ڸ� ������ �� �ִ��� ����
	{
		perror("listen");
		exit(1);

	}

	u_long on = 0;																//0�ϰ�� ����, 1�ϰ�� �ȸ���
	if (ioctlsocket(sockfd, FIONBIO, &on) == SOCKET_ERROR)                     //�񵿱� ����(���� ����Ѵ� ���� �ƴ� ��� ����� ����� ���α׷��� �����ϰ� ����� �ֱ� ���� ��)�� ���� ����, accept�� ���� ���¸� ���� �ϱ� ����
	{
		perror("socket");
		exit(1);
	}

	printf("wait...\n");                                          //������ �����ؼ� ���ư��� �����Ҷ� ���

	hWriteEvent = CreateEvent(NULL, FALSE, TRUE, NULL);                        //�̺�Ʈ ��鷯 ����(���� �Ӽ�,Ŀ�ο�����Ʈ ����(auto-rest),signal����(��ȣ ����->���� ����),�̺�Ʈ ������Ʈ�� �̸�����)
	if (hWriteEvent == NULL) return 1;

	while (1)
	{
		sin_size = sizeof(struct sockaddr_in);                           //���� �ּ� ����ü ����ũ��� �ʱ�ȭ(������ ����)

		while (1)
		{

			if ((new_fd = accept(sockfd, (struct sockaddr *)&connect_addr, &sin_size)) == INVALID_SOCKET)         //���ο� ���� ���� ����ڷ� �����Ű�� �ȴ�. ������ ������ ��� listen��
			{
				Sleep(100);

				if (WSAGetLastError() != WSAEWOULDBLOCK)                                          //��ȯ���� ������ ���                                          
				{
					perror("accept");
					break;
				}
				continue;                                                               //��� �����Ѵ�
			}
			break;                                                                  //���ο� ������ ���ӽ� �����ϰ� �����带 �����Ű�� ���� while�� ������
		}
		printf("server: got connection from %s\n", inet_ntoa(connect_addr.sin_addr));               //������ ��ġ�� ������ش�.inet_ntoa�� ��Ʈ��ũ �� ����� �����͸� ��������� ���������� ��ȯ

		hThread = CreateThread(NULL, 0, Connect_process, (LPVOID)new_fd, 0, NULL);                        //������ Ŭ���̾�Ʈ�� ó���� ������ ���� �׸��� �װ��� ������ �� �ִ� ������ �ڵ� ����(���:���� ��ũ���� ����, �����忡 �Ҵ��ϴ� ����ũ��(�Ҵ� ���� ������ 1mb), ������ �Լ��� �����ּ�,������ �Լ��� ������ ����, ������ ������ �����ϴ� ��(0�Ͻ� �ٷ� ����), ������ id��ȯ����(DWORD��))
		if (hThread == NULL)                                                            //���� �߻��� �� ���� ����(���� ����)
		{
			closesocket(new_fd);
		}
		else                                                                     //������ �����Ȱ�� �ڵ鿡 ������ �ʿ� ������ �ڵ� ����(�ڵ� �����ص� �����尡 �������� �ʴ´�) 
		{
			CloseHandle(hThread);
		}
	}
	WaitForSingleObject(hThread, INFINITE);                                                //��� �����尡 ���������� ��ٷ� �ش�.
	closesocket(sockfd);                                                            //���� ����
	WSACleanup();                                                                  //wsacleanup �Լ� ����

	return 0;
}

DWORD WINAPI Connect_process(LPVOID socket)                                             //���� ��û�� ������� �и������� ����Ǵ� �Լ�
{
	WaitForSingleObject(hWriteEvent, INFINITE);                                          //�ٸ� �����尡 ����� �Ͻ� ����Ѵ� ��

	char buf[BUFSIZ];                                                            //���� �޼���
	int msg = 0;                                                                  //���޹��� �޼����� ũ��

	SOCKET connect_sock = (SOCKET)socket;                                                //���޹��� ���ڸ� socket���� ����ȯ�ؼ� ����Ѵ�.

	msg = recv(connect_sock, buf, BUFSIZ - 1, 0);                                             //�޼����� �޴´�. msg�� ���� �޼����� ũ�⸦ ��Ÿ����.
	if (msg == -1)
		printf("%d", msg);
	if (msg == SOCKET_ERROR)                                                         // ���� �� ���
	{
		closesocket(connect_sock);                                                   // ���� ����
		SetEvent(hWriteEvent);                                                      // �� ���ٰ� �˸�
		return 0;                                                               // �Լ� ����
	}
	buf[msg] = '\0';                                                               //�������� null���� �߰�

	creatmessage(buf, connect_sock);                                                   //�޼��� ����, ������ �Լ�	

	closesocket(connect_sock);
	printf("close_socket\n");
	SetEvent(hWriteEvent);                                                         //�а� �� �� �˸��� ��� ���ᶧ���� �ߺ� ���ɼ� �����ϱ� ����

	return 0;
}

void creatmessage(char *rec_msg, SOCKET sock)                                          //�޼����� ����� �����ִ� �Լ�
{
	static char sendmsg[4097];                                                      //������ �޼���
	int msg_len = strlen(rec_msg);                                                   //�޼��� ���� Ȯ�� �� �ݺ��� ����
	int set = 0;
	char* return_data;


	char* creat_buf = (char*)malloc(sizeof(char)*(msg_len + 1));


	memset(sendmsg, '\0', 4097);                                                      //opendata �ʱ�ȭ

	strcpy(creat_buf, rec_msg);                                                      //���� �޼����� ���ۿ�

	printf("\nrequest message:%s\n", rec_msg);      //�޴� ���� ��� ����

	return_data = analysis_message(creat_buf, msg_len, sock);                                       //���� �޼��� ���ۿ� ���̸� ������ �м��� �ʿ��� ��Ҹ� ����ü�� ����

	strcpy(sendmsg, return_data);


	printf("%s", return_data);

	send(sock, sendmsg, strlen(sendmsg), 0);

}

char* analysis_message(char *message, int msg_len, SOCKET sock)                                                //���� �޼����� �м��ϴ� �Լ�(���� �޼���, �޼��� ����)
{
	int i = 0;
	int j = 1;
	int k = 0;
	char save_met[9][6] = { "LOGIN","ADDUS","USPIC","DELUS","LOGIA","COUSE","COPIC","COSEE","MONEY" };
	char methood[6];
	char *arr;                                                                                 //�ӽ� ���� ����


	for (j = 0; j < 5; j++)
		methood[j] = message[j];

	methood[5] = '\0';

	for (j = 0; j < 10; j++)
		save_met[j][5] = '\0';


	if (strcmp(methood, save_met[0]) == 0)                                                            //�޼ҵ��
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