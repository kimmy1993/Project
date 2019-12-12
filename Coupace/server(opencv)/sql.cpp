#include "header_m.h"

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "PassWord"
#define DB_NAME "Capston"
#define CHOP(x) x[strlen(x)-1] = ' '

BSTR bstrWide;
int nLength;

char* query_message(char *message, int num, cv_data cv_load, SOCKET sock)
{
	MYSQL *connection, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;

	int pic_num;
	int select_coupon;
	int coupon_data;
	int cou_use;
	int sells;
	int *cv_int;
	int smaile;

	char pic_name[21];

	char fam_id[21];

	char return_data[2048];

	char query[2048];

	memset(query, '\0', 2048);

	memset(return_data, '\0', 2048);

	mysql_init(&conn);

	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

	if (connection == NULL)
	{
		fprintf(stderr, "MYSQL connection error : %s", mysql_error(&conn));
		return NULL;
	}


	switch (num)
	{
	case 0:									//login
		login(message, query);
		query_stat = mysql_query(connection, query);

		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
			break;
		}
		sql_result = mysql_store_result(connection);
		if ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			strcpy(return_data, "success");
			printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
		}
		else
			strcpy(return_data, "fail");
		mysql_free_result(sql_result);

		printf("%s", return_data);
		break;
	case 1:									//addus
		add_adder(message, query, sock);
		if (cv_load.cv_input() == false)
		{
			strcpy(return_data, "fail");
			file_return();
			break;
		}
		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
		}
		else
			strcpy(return_data, "success");
		break;
	case 2:									//uspic
		pic_search(message, query);

		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
			break;
		}

		sql_result = mysql_store_result(connection);

		if ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			if (sql_row[0] == NULL)
				pic_num = 0;
			else if (sql_row[1] == NULL)
				pic_num = 1;
			else
			{
				strcpy(pic_name, sql_row[1]);
				pic_num = 2;
			}
		}

		add_pic(message, query, sock, pic_name, pic_num);
		query_stat = mysql_query(connection, query);

		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
		}
		else
			strcpy(return_data, "success");
		break;
	case 3:									//delus
		del_adder(message, query);
		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
		}
		else
			strcpy(return_data, "success");
		break;
	case 4:
		login_app(message, query);
		query_stat = mysql_query(connection, query);

		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
			break;
		}
		sql_result = mysql_store_result(connection);

		for (int ln = 0; ln < 5; ln++)
		{
			if ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				strcat(return_data, sql_row[0]);
				strcat(return_data, "@@");
				strcat(return_data, sql_row[1]);
				strcat(return_data, "@@");
			}
		}
		if (return_data[0] == '\0')
		{
			strcpy(return_data, "fail");
		}

		mysql_free_result(sql_result);

		break;

	case 5:									//couse
		coupon_search(message, query);
		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
			break;
		}
		sql_result = mysql_store_result(connection);
		sql_row = mysql_fetch_row(sql_result);
		select_coupon = atoi(sql_row[1]);
		memset(query, '\0', 2048);
		mysql_free_result(sql_result);

		search_fam_id(message, query);
		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
			break;
		}
		sql_result = mysql_store_result(connection);

		if ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			strcpy(fam_id, sql_row[0]);
		}

		memset(query, '\0', 2048);
		mysql_free_result(sql_result);

		coupon_load(message, query, select_coupon, fam_id);
		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
			break;
		}
		sql_result = mysql_store_result(connection);

		smaile = cv_load.cv_get_smaile();

		cv_load.cv_smaile_clear();

		if ((sql_row = mysql_fetch_row(sql_result)) == NULL)
		{
			memset(query, '\0', 2048);
			cou_use = coupon_add(message, query, select_coupon, fam_id,smaile);
		}
		else
		{
			coupon_data = atoi(sql_row[2]);
			memset(query, '\0', 2048);

			cou_use = coupon_use(message, query, select_coupon, coupon_data, fam_id,smaile);
		}

		if (cou_use == -1)
		{
			strcpy(return_data, "fail");
			break;
		}

		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
			break;
		}
		else
			strcpy(return_data, "success");
		
		sells_search(message, query);

		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
			break;
		}
		sql_result = mysql_store_result(connection);
		sql_row = mysql_fetch_row(sql_result);
		sells = atoi(sql_row[5]);
		memset(query, '\0', 2048);

		add_own_sells(message, query, sells);
		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
			break;
		}
		else
			strcpy(return_data, "success");

		mysql_free_result(sql_result);
		break;
	case 6:									//copic
		pic_num = coupon_pic(message, sock,cv_load);

		cv_int = cv_load.cv_main('3', pic_num);
		for (int i = 0; i < 3; i++)
		{
			if (i == -1)
				break;
			else
			{
				pic_query(query, cv_int[i]);

				query_stat = mysql_query(connection, query);

				if (query_stat != 0)
				{
					fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
					strcpy(return_data, "fail");
					break;
				}
				sql_result = mysql_store_result(connection);
				if ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					strcat(return_data, sql_row[0]);
					strcat(return_data, " ");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						strcat(return_data, sql_row[0]);
						strcat(return_data, " ");
					}
				}

				mysql_free_result(sql_result);
			}
		}
		if (return_data[0] == '\0')
			strcpy(return_data, "fail");

		break;
	case 7:									//cosee
		coupon_search(message, query);
		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
			break;
		}
		sql_result = mysql_store_result(connection);
		sql_row = mysql_fetch_row(sql_result);
		select_coupon = atoi(sql_row[1]);
		memset(query, '\0', 2048);
		mysql_free_result(sql_result);

		store_data(message, query, select_coupon);

		printf("%s", query);
		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			strcpy(return_data, "fail");
			break;
		}
		sql_result = mysql_store_result(connection);
		if ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			strcat(return_data, sql_row[1]);
			strcat(return_data, "@@");
			strcat(return_data, sql_row[2]);
			strcat(return_data, "@@");
			if (select_coupon == 1)
				strcat(return_data, "1");
			else if (select_coupon == 2)
				strcat(return_data, "2");
			strcat(return_data, "@@");
		}


		break;
	}

	mysql_close(connection);

	return return_data;
}

void login(char * message, char * query)								//login
{
	int i = 7;
	int k = 0;
	char id[21];
	char pass[21];
	char user[2][4] = { "OWN","MEM" };
	char que[2048];


	memset(id, '\0', 21);
	memset(pass, '\0', 21);
	memset(que, '\0', 2048);


	for (k = 0; k < 21; k++)
	{
		id[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	i += 2;
	for (k = 0; k < 21; k++)
	{
		pass[k] = message[i];
		i++;

		if (message[i] == '@')
			break;
	}

	i += 2;

	if (message[i] == '0')
		sprintf(que, "select * from %s where O_ID = '%s' and O_Passward = '%s'", user[0], id, pass);
	else
		sprintf(que, "select * from %s where M_ID = '%s' and M_Passward = '%s'", user[1], id, pass);



	strcpy(query, que);

	printf("%s", query);
}

void add_adder(char * message, char * query, SOCKET socket)							//addus
{
	int i = 7;
	int k = 0;
	int l;
	int msg;
	int num;
	int file_siz;
	char pic_num[2];
	char load_data[20];
	char id[21];
	char save[30];
	char name[20];
	char pass[21];
	char phone[15];
	char famid[21];
	char pic[20];
	char que[2048];
	char return_data[2] = "1";
	char rcv_data[4097];
	char data[4097];
	char open_pic[40];

	FILE *save_num = fopen("S_P_Data.bin", "rb+");

	FILE * pic_data;

	memset(load_data, '\0', 20);
	memset(id, '\0', 21);
	memset(save, '\0', 30);
	memset(name, '\0', 20);
	memset(pass, '\0', 21);
	memset(phone, '\0', 15);
	memset(famid, '\0', 21);
	memset(pic, '\0', 40);
	memset(que, '\0', 2048);

	fread(load_data, sizeof(char), 20, save_num);

	num = atoi(load_data);

	fclose(save_num);

	for (k = 0; k < 21; k++)
	{
		id[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	i += 2;
	for (k = 0; k < 21; k++)
	{
		save[k] = message[i];
		i++;

		if (message[i] == '@')
			break;
	}

	i += 2;
	for (k = 0; k < 21; k++)
	{
		pass[k] = message[i];
		i++;

		if (message[i] == '@')
			break;
	}

	i += 2;
	for (k = 0; k < 21; k++)
	{
		phone[k] = message[i];
		i++;

		if (message[i] == '@')
			break;
	}

	i += 2;
	for (k = 0; k < 21; k++)
	{
		if (message[i] == '@')
			break;
		famid[k] = message[i];
		i++;

	}
	i += 2;
	if (famid[0] == '\0' || strcmp(famid, "NULL") == 0)
		strcpy(famid, id);

	send(socket, return_data, strlen(return_data), 0);

	sprintf(pic, "_%d.png", num);
	sprintf(open_pic, "./FERET/GALLERY/%s", pic);
	num++;
	pic_data = fopen(open_pic, "wb+");

	memset(rcv_data, '\0', 4097);
	msg = recv(socket, rcv_data, 4096, 0);

	file_siz = atoi(rcv_data);

	send(socket, return_data, strlen(return_data), 0);

	
	while (1)
	{
		memset(data, '\0', 4097);
		memset(rcv_data, '\0', 4097);
		msg = recv(socket, rcv_data, 4096, 0);                                             //메세지를 받는다. msg는 받은 메세지의 크기를 나타낸다.
		if (msg == -1 || msg == 0)
			break;
		for (l = 0; l < msg; l++)
		{
			data[l] = rcv_data[l];
		}
		printf("%d\n", l);
		file_siz -= l;
		/*if (file_siz >= 0 && l < 4096)
		{
			return_data[0] = '0';
			send(socket, return_data, strlen(return_data), 0);
			file_siz += l;
		}
		else
		{*/
			return_data[0] = '1';
			send(socket, return_data, strlen(return_data), 0);
			fwrite(data, sizeof(char), l, pic_data);
			printf("%d\n", file_siz);
			if (file_siz == 0)
				break;
		//}
	}

	fclose(pic_data);

	send(socket, return_data, strlen(return_data), 0);
	
	save_num = fopen("S_P_Data.bin", "wb+");

	memset(load_data, '\0', 20);

	sprintf(load_data, "%d", num);

	fwrite(load_data, sizeof(char), strlen(load_data), save_num);

	fclose(save_num);


	nLength = MultiByteToWideChar(CP_UTF8, 0, save, (int)strlen(save) + 1, NULL, NULL);

	bstrWide = SysAllocStringLen(NULL, nLength);

	MultiByteToWideChar(CP_UTF8, 0, save, (int)strlen(save) + 1, bstrWide, nLength);

	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, name, nLength, NULL, NULL);

	sprintf(que, "insert into Mem(M_id, M_name, m_passward, m_phone, M_famid, m_pic1) values('%s', '%s', '%s', '%s', '%s', '%s')", id, name, pass, phone, famid, pic);

	strcpy(query, que);

	printf("%s", query);
}

void add_pic(char* message, char * query, SOCKET socket,char * pic_name, int pic_num)						//pisus
{
	int i = 7;
	int k = 0;
	int l;
	int num;
	int msg;
	int file_size;
	char id[21];
	char pic[20];
	char que[2048];
	char data[4097];
	char rcv_data[4097];
	char load_data[20];
	char return_data[2] = "1";
	char open_pic[40];

	FILE * pic_data;
	FILE * save_num = fopen("S_P_Data.bin", "rb+");

	memset(load_data, '\0', 20);
	
	fread(load_data, sizeof(char), 20, save_num);
	
	num = atoi(load_data);

	fclose(save_num);

	memset(id, '\0', 21);
	memset(pic, '\0', 40);
	memset(que, '\0', 2048);

	for (k = 0; k < 21; k++)
	{
		id[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	i += 2;


	memset(open_pic, '\0', 40);
	send(socket, return_data, strlen(return_data), 0);
	if (pic_num == 0)
	{
		sprintf(pic, "_%d.png", num);
		sprintf(open_pic, "./FERET/GALLERY/%s", pic);
		num++;
		pic_data = fopen(open_pic, "wb+");
	}
	else if (pic_num == 1)
	{
		sprintf(pic, "_%d.png", num);
		sprintf(open_pic, "./FERET/GALLERY/%s", pic);
		num++;
		pic_data = fopen(open_pic, "wb+");

	}
	else
	{
		sprintf(pic, pic_name);
		sprintf(open_pic, "./FERET/GALLERY/%s", pic);
		pic_data = fopen(open_pic, "wb+");
	}

	memset(rcv_data, '\0', 4097);
	msg = recv(socket, rcv_data, 4096, 0);
	send(socket, return_data, strlen(return_data), 0);

	file_size = atoi(rcv_data);
	while (1)
	{
		memset(data, '\0', 4097);
		memset(rcv_data, '\0', 4097);
		msg = recv(socket, rcv_data, 4096, 0);                                             //메세지를 받는다. msg는 받은 메세지의 크기를 나타낸다.
		if (msg == -1 || msg == 0)
			break;
		for (l = 0; l < msg; l++)
		{
			if (rcv_data[l] == '@')
			{
				if (l + 1 != msg && rcv_data[l + 1] == '@')
					break;
			}
			data[l] = rcv_data[l];
		}
		file_size -= l;
		/*if (file_size > 0 && l < 4096)
		{
			return_data[0] = '0';
			send(socket, return_data, strlen(return_data), 0);
			file_size += l;
		}
		else
		{*/
			return_data[0] = '1';
			send(socket, return_data, strlen(return_data), 0);
			fwrite(data, sizeof(char), l, pic_data);
			if (file_size <= 0)
				break;
		//}
	}
	fclose(pic_data);

	if (k > 0)
		send(socket, return_data, strlen(return_data), 0);

	save_num = fopen("S_P_Data.bin", "wb+");

	memset(load_data, '\0', 20);

	sprintf(load_data, "%d", num);

	fwrite(load_data, sizeof(char), strlen(load_data), save_num);

	fclose(save_num);

	if (pic_num == 0)
		sprintf(que, "Update Mem set M_pic1 = '%s' where M_ID ='%s'", pic, id);

	else
		sprintf(que, "Update Mem set M_pic2 = '%s' where M_ID ='%s'", pic, id);

	strcpy(query, que);

	printf("%s", query);
}

void del_adder(char* message, char* query)				//delus
{

	int i = 7;
	int k = 0;
	char id[21];
	char pass[21];
	char que[2048];


	memset(id, '\0', 21);
	memset(pass, '\0', 21);
	memset(que, '\0', 2048);


	for (k = 0; k < 21; k++)
	{
		id[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	i += 2;
	for (k = 0; k < 21; k++)
	{
		pass[k] = message[i];
		i++;

		if (message[i] == '@')
			break;
	}

	i += 2;


	sprintf(que, "delete from Mem where M_ID = '%s' and M_Passward = '%s'", id, pass);

	strcpy(query, que);

	printf("%s", query);
}

int coupon_add(char* message, char* query, int coupon, char* fam_id, int smaile)						//coadd
{
	int i = 7;
	int k = 0;
	char id_own[21];
	char id_mem[21];
	char data[2];
	char coupon_data[100];
	char que[2048];


	memset(id_own, '\0', 21);
	memset(id_mem, '\0', 21);
	memset(coupon_data, '\0', 100);
	memset(que, '\0', 2048);


	for (k = 0; k < 21; k++)
	{
		id_own[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	i += 2;

	for (k = 0; k < 21; k++)
	{
		id_mem[k] = message[i];
		i++;

		if (message[i] == '@')
			break;
	}

	i += 2;

	for (k = 0; k < 2; k++)
	{
		data[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	i += 2;

	for (k = 0; k < 200; k++)
	{
		coupon_data[k] = message[i];
		i++;

		if (message[i] == '@')
			break;
	}

	if (data[0] == '1')
		return -1;

	if (coupon == 1)
	{
		if (smaile == 0)
			sprintf(que, "INSERT INTO S_Coupon (O_Id, M_FamId, S_Point) VALUES ('%s', '%s', '%d')", id_own, fam_id, atoi(coupon_data) / 5000);
		else if (smaile == 1)
			sprintf(que, "INSERT INTO S_Coupon (O_Id, M_FamId, S_Point) VALUES ('%s', '%s', '%d')", id_own, fam_id, (atoi(coupon_data) / 5000) +1);
	}
	else if (coupon == 2)
	{
		if (smaile == 0)
			sprintf(que, "INSERT INTO P_Coupon (O_Id, M_FamId, P_Point) VALUES ('%s', '%s', '%d')", id_own, fam_id, atoi(coupon_data) / 10);
		else if (smaile == 1)
			sprintf(que, "INSERT INTO P_Coupon (O_Id, M_FamId, P_Point) VALUES ('%s', '%s', '%d')", id_own, fam_id, atoi(coupon_data) * 1.1 / 10);
	}
	else
		return -1;


	strcpy(query, que);

	printf("%s", query);
	return 0;
}

int coupon_use(char* message, char* query, int coupon, int cou_data, char * fam_id,int smaile)						//cousd
{
	int i = 7;
	int k = 0;
	char id_own[21];
	char id_mem[21];
	char data[2];
	char coupon_data[100];
	char que[2048];
	int after_coupon;


	memset(id_own, '\0', 21);
	memset(id_mem, '\0', 21);
	memset(coupon_data, '\0', 100);
	memset(que, '\0', 2048);


	for (k = 0; k < 21; k++)
	{
		id_own[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	i += 2;

	for (k = 0; k < 21; k++)
	{
		id_mem[k] = message[i];
		i++;

		if (message[i] == '@')
			break;
	}

	i += 2;

	for (k = 0; k < 2; k++)
	{
		data[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	i += 2;

	for (k = 0; k < 200; k++)
	{
		coupon_data[k] = message[i];
		i++;

		if (message[i] == '@')
			break;
	}

	if (data[0] == '1')
	{
		if (coupon == 1)
		{
			if (cou_data  < (atoi(coupon_data)+5000) / 5000)
				return -1;
			after_coupon = cou_data - (atoi(coupon_data)+5000) / 5000;
		}
		if (coupon == 2)
		{
			if (cou_data < atoi(coupon_data))
				return -1;
			after_coupon = cou_data - atoi(coupon_data);
		}
	}
	else if (data[0] == '0')
	{
		if (coupon == 1)
		{
			if (smaile == 0)
				after_coupon = cou_data + atoi(coupon_data) / 5000;
			else if (smaile == 1)
				after_coupon = cou_data + (atoi(coupon_data) / 5000) + 1;
		}
		else if(coupon == 2)
		{
			if (smaile == 0)
				after_coupon = cou_data + atoi(coupon_data) / 10;
			else if (smaile == 1)
				after_coupon = cou_data + atoi(coupon_data) * 1.1 / 10;
		}
	}

	if (coupon == 1)
	{
		sprintf(que, "update S_coupon set S_point = '%d' where O_id = '%s' and M_FamId = '%s'", after_coupon, id_own, fam_id);
	}
	else if (coupon == 2)
		sprintf(que, "update P_coupon set p_point = '%d' where O_id = '%s' and M_FamId = '%s'", after_coupon, id_own, fam_id);
	else
		return -1;


	strcpy(query, que);

	printf("%s", query);
	return 0;
}

int coupon_pic(char* message, SOCKET socket, cv_data cv_load)						//copic
{
	int i = 7;
	int k = 0;
	int j;
	int l;
	int file_size = 0;
	int msg;
	int smaile_data;
	char rcv_data[4097];
	char id[21];
	char return_data[2] = "1";
	char data_num[2];
	char smaile[2];
	char data[4097];
	char file_name[40];
	FILE* fp;

	memset(id, '\0', 21);
	memset(data_num, '\0', 2);

	for (k = 0; k < 21; k++)
	{
		id[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}
	i += 2;
	for (k = 0; k < 2; k++)
	{
		data_num[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}
	i += 2;
	for (k = 0; k < 2; k++)
	{
		smaile[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}
	j = atoi(data_num);

	smaile_data = atoi(smaile);

	if (smaile_data == 1)
		cv_load.cv_smaile();
	
	for (k = 0; k < j; k++)
	{
		send(socket, return_data, strlen(return_data), 0);		

		return_data[0] = '1';

		memset(file_name, '\0', 40);
		sprintf(file_name, "./FERET/recong/a%d.png", k+1);

		memset(rcv_data, '\0', 4097);
		msg = recv(socket, rcv_data, 4096, 0);
		send(socket, return_data, strlen(return_data), 0);

		file_size = atoi(rcv_data);

		fp = fopen(file_name, "wb+");
		while (1)
		{
			memset(data, '\0', 4097);
			memset(rcv_data, '\0', 4097);
			msg = recv(socket, rcv_data, 4096, 0);                                             //메세지를 받는다. msg는 받은 메세지의 크기를 나타낸다.
			printf("%d\n", msg);
			if (msg == -1||msg == 0)
				break;
			for (l = 0; l < msg; l++)
			{
				data[l] = rcv_data[l];
			}
			file_size -= l;
			printf("%d\n", l);
			
			/*if (file_size != 0 && l < 4096)
			{
				return_data[0] = '0';
				send(socket, return_data, strlen(return_data), 0);
				file_size += l;
			}
			else
			{*/
				return_data[0] = '1';
				send(socket, return_data, strlen(return_data), 0);
				fwrite(data, sizeof(char), l, fp);
				if (file_size <=0)
					break;
			//}
		}
		fclose(fp);
	}

	send(socket, return_data, strlen(return_data), 0);
	
	return k;
}

void store_data(char* message, char* query, int coupon)						//회원아이디로 접속, 쿠폰 정보 요청시 사업자 가게이름을 찾는 함수
{
	int i = 7;
	int k = 0;
	char id_own[21];
	char id_mem[21];
	char que[2048];


	memset(id_own, '\0', 21);
	memset(id_mem, '\0', 21);
	memset(que, '\0', 2048);


	for (k = 0; k < 21; k++)
	{
		id_own[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}
	i += 2;

	for (k = 0; k < 21; k++)
	{
		id_mem[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	if (coupon == 1)
		sprintf(que, "select * from s_coupon where O_id = '%s' and M_FamId = (select M_Famid from Mem where M_ID = '%s')", id_own, id_mem);
	else if (coupon == 2)
		sprintf(que, "select * from P_coupon where O_id = '%s' and M_FamId = (select M_Famid from Mem where M_ID = '%s')", id_own, id_mem);

	strcpy(query, que);

	printf("%s", query);
}

void coupon_search(char* message, char* query)
{
	int i = 7;
	int k = 0;
	char id_own[21];
	char que[2048];


	memset(id_own, '\0', 21);
	memset(que, '\0', 2048);


	for (k = 0; k < 21; k++)
	{
		id_own[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	i += 2;

	sprintf(que, "select * from K_coupon where O_id = '%s'", id_own);

	strcpy(query, que);

	printf("%s", query);
}

void coupon_load(char* message, char* query, int coupon, char* fam_id)
{

	int i = 7;
	int k = 0;
	char id_own[21];
	char id_mem[21];
	char que[2048];

	memset(id_own, '\0', 21);
	memset(id_mem, '\0', 21);
	memset(que, '\0', 2048);


	for (k = 0; k < 21; k++)
	{
		id_own[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	i += 2;
	for (k = 0; k < 21; k++)
	{
		id_mem[k] = message[i];
		i++;

		if (message[i] == '@')
			break;
	}

	i += 2;

	if (coupon == 1)
		sprintf(que, "select * from s_coupon where O_id = '%s' and M_FamId = (select M_Famid from Mem where M_ID = '%s')", id_own, fam_id);
	else if (coupon == 2)
		sprintf(que, "select * from P_coupon where O_id = '%s' and M_FamId = (select M_Famid from Mem where M_ID = '%s')", id_own, fam_id);
	else
		return;

	strcpy(query, que);

	printf("%s", query);
}

void add_own_sells(char* message, char* query, int sells)
{
	int i = 7;
	int k = 0;
	int sell_data;
	char id_own[21];
	char id_mem[21];
	char used[2];
	char sells_data[200];
	char que[2048];

	memset(id_own, '\0', 21);
	memset(id_mem, '\0', 21);
	memset(used, '\0', 21);
	memset(sells_data, '\0', 200);
	memset(que, '\0', 2048);

	for (k = 0; k < 21; k++)
	{
		id_own[k] = message[i];
		i++;
		if (message[i] == '@' && message[i + 1] == '@')
			break;
	}

	i += 2;

	for (k = 0; k < 21; k++)
	{
		id_mem[k] = message[i];
		i++;
		if (message[i] == '@'&& message[i + 1] == '@')
			break;
	}

	i += 2;
	for (k = 0; k < 2; k++)
	{
		used[k] = message[i];
		i++;
		if (message[i] == '@' && message[i + 1] == '@')
			break;
	}
	i += 2;

	for (k = 0; k < 200; k++)
	{
		sells_data[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	sell_data = atoi(sells_data);
	
	sell_data += sells;


	sprintf(que, "update Own set O_sells = '%d' where o_id = '%s'", sell_data, id_own);

	strcpy(query, que);

	printf("%s", query);
}


void sells_search(char* message, char* query)
{
	int i = 7;
	int k = 0;
	char id_own[21];
	char que[2048];

	memset(id_own, '\0', 21);
	memset(que, '\0', 2048);

	for (k = 0; k < 21; k++)
	{
		id_own[k] = message[i];
		i++;
		if (message[i] == '@' && message[i + 1] == '@')
			break;
	}

	i += 2;

	sprintf(que, "select * from own where o_id = '%s' ", id_own);

	strcpy(query, que);

	printf("%s", query);
}

void pic_query(char* query, int data)
{
	char que[1024];
	char select_data[21];

	memset(select_data, '\0', 21);
	memset(que, '\0', 1024);
	
	sprintf(select_data, "_%d.png", data);

	sprintf(que, "select M_id from Mem Where M_pic1 = '%s' or M_pic2 ='%s'", select_data, select_data);

	strcpy(query, que);
	printf("%s", query);

}

void search_fam_id(char* message,char * query)
{
	int i = 7;
	int k = 0;
	char id_own[21];
	char id_mem[21];
	char que[2048];


	memset(id_own, '\0', 21);
	memset(id_mem, '\0', 21);
	memset(que, '\0', 2048);


	for (k = 0; k < 21; k++)
	{
		id_own[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	i += 2;

	for (k = 0; k < 21; k++)
	{
		id_mem[k] = message[i];
		i++;

		if (message[i] == '@')
			break;
	}

	sprintf(que, "select M_famid from mem where m_id = '%s'", id_mem);

	strcpy(query, que);

	printf("%s", query);

}


void pic_search(char* message, char* query)
{
	int i = 7;
	int k = 0;
	char id[21];
	char que[2048];

	for (k = 0; k < 21; k++)
	{
		id[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	sprintf(que, "select M_pic1, M_pic2 from mem where m_id = '%s'", id);

	strcpy(query, que);

	printf("%s", query);

}

void login_app(char* message, char* query)
{
	int i = 7;
	int k = 0;
	char id[21];
	char que[2048];


	memset(id, '\0', 21);
	memset(que, '\0', 2048);


	for (k = 0; k < 21; k++)
	{
		id[k] = message[i];
		i++;
		if (message[i] == '@')
			break;
	}

	sprintf(que, "select O_store ,o_id from Own where O_id = any(select O_id from P_Coupon where M_Famid = (select M_Famid from Mem where M_id = '%s'))  or  O_id = any(select O_id from S_Coupon where M_Famid = (select M_Famid from Mem where M_id = '%s'))", id, id);



	strcpy(query, que);

	printf("%s", query);
}

void file_return()
{
	FILE * save_num;
	int num;
	char load_data[20];

	save_num = fopen("S_P_Data.bin", "wb+");

	memset(load_data, '\0', 20);

	fread(load_data, sizeof(char), 20, save_num);

	fclose(save_num);

	num = atoi(load_data) - 1;

	save_num = fopen("S_P_Data.bin", "wb+");

	memset(load_data, '\0', 20);

	sprintf(load_data, "%d", num);

	fwrite(load_data, sizeof(char), strlen(load_data), save_num);

	fclose(save_num);

}