#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct user
{
	char name[10];
	int phone[20];
	char Adress_Big[10];
	char Adress_small[50];
	int	PW[10];			//��������
	user* Next;
}user;

typedef struct user_head
{
	int Mambers;
	user* first;
}user_head;

typedef struct parcel_user
{
	char name[10];
	int phone[20];
	char Adress_Big[10];
	char Adress_small[50];
}parcel_user;

typedef struct parcel_ed
{
	parcel_user* to;
	parcel_user* from;
	parcel_ed* next;
}parcel_ed;

typedef struct parcel_ing
{
	parcel_user* to;				//�����»��
	parcel_user* from;				//�޴»��
	parcel_ing* next;
}parcel_ing;

typedef struct parcel_ready
{
	parcel_user* to;
	parcel_user* from;
	parcel_ready* next;
}parcel_ready;

typedef struct parcel
{
	int parcel_ed_num;
	int parcel_ing_num;
	int parcel_ready_num;
	parcel_ed* ed;
	parcel_ing* ing;
	parcel_ready* ready;
}parcel;

typedef struct worker
{
	char name[10];
	char Adress_Big[10];
	int successed;
	int PW[10];
	worker* next;
}worker;

typedef struct worker_head
{
	worker* head;
}worker_head;


void main_menu();
void parcel_menu_add(user_head**, parcel**);
void parcel_menu_search(parcel**);
void worker_menu(worker_head**, parcel**);
void work_menu(user_head**, parcel**, worker_head**, int*);
void input_user(user_head**);
void input_parcel(parcel**);
void input_worker(worker_head**);
void input_masterPW(int*);
void add_user(user_head**);
user* login_user(user_head*);
void add_parcel(parcel**, user*);
void search_parcel(parcel**, int);
void print_parcel(parcel_user*, int);
void change_from_parcel(parcel_user**);
void print_all_parcel(worker_head**, parcel**, int);
char* selec_area();
void change_state_parcel(parcel**, worker*, int);
void input_reday_to_ing(parcel_ing**, parcel_ready**);
void input_ing_to_ed(parcel_ed**, parcel_ing**);
void print_worker(worker_head**);
void input_user_data(user_head**, user);
void print_member(user_head**);
void input_worker_data(worker_head**, worker);
void input_parcel_data(parcel**, parcel_user, parcel_user, int);
void output_member(user_head**);
void output_parcel(parcel**);
void output_worker(worker_head**);



void main()
{
	user_head* User = (user_head*)malloc(sizeof(user_head));
	User->first = NULL;
	parcel* Parcel = (parcel*)malloc(sizeof(parcel));
	Parcel->ed = NULL;
	Parcel->ing = NULL;
	Parcel->ready = NULL;
	worker_head* Worker = (worker_head*)malloc(sizeof(worker_head));
	Worker->head = NULL;
	int Master_PW[10];

	input_masterPW(Master_PW);
	input_user(&User);
	input_worker(&Worker);
	input_parcel(&Parcel);

	int selec_num = 1;
	while (selec_num != 0)
	{
		main_menu();
		scanf("%d", &selec_num);
		switch (selec_num)
		{
		case 1:
			parcel_menu_add(&User, &Parcel);
			break;
		case 2:
			parcel_menu_search(&Parcel);
			break;
		case 3:
			worker_menu(&Worker, &Parcel);
			break;
		case 4:
			work_menu(&User, &Parcel, &Worker, Master_PW);
			break;
		case 0:
			printf("�����մϴ�\n");
			output_member(&User);
			output_parcel(&Parcel);
			output_worker(&Worker);
			break;
		default:
			printf("�߸��� �����Դϴ�\n");
			break;
		}
	}
}

void main_menu()
{
	printf("�ù� ���� �ý��ۿ� ���Ű��� ȯ���մϴ�.\n");
	printf("1. �ù� ����\n");
	printf("2. �ù� ��ȸ\n");
	printf("3. �ù� ���\n");
	printf("4. �ù��\n");
	printf("0. ����\n");
}

void parcel_menu_add(user_head** u_head, parcel** p_head)
{
	int num = -1;
	user* User;
	while (num != 0)
	{
		printf("1. ȸ������\n2. �ù�����\n0. ����\n");
		scanf("%d", &num);
		switch (num)
		{
		case 1:
			add_user(u_head);
			(*u_head)->Mambers = (*u_head)->Mambers + 1;
			break;
		case 2:
			User = login_user(*u_head);
			if (User == NULL)
				printf("�α��ο� �����Ͽ����ϴ�\n");
			else
				add_parcel(p_head, User);
			break;
		}
	}
}

void parcel_menu_search(parcel** head)
{
	int num = -1;
	while (num != 0)
	{
		printf("1. ��� ����\n2. ��� ����\n0. ����\n");
		scanf("%d", &num);
		switch (num)
		{
		case 1:
		case 2:
			search_parcel(head, num);
			break;
		}
	}
}

void worker_menu(worker_head** w_head, parcel** p_head)
{
	int num = -1;
	while (num != 0)
	{
		printf("1. �ù� ����\n2. ��� Ȯ��\n0. ����\n");
		scanf("%d", &num);
		switch (num)
		{
		case 1:
		case 2:
			print_all_parcel(w_head, p_head, num);
			break;
		}
	}
}

void work_menu(user_head** u_head, parcel** p_head, worker_head** w_head, int master_PW[])
{
	int PW[10];
	scanf("%s", PW);
	if (!strcmp((char*)PW, (char*)master_PW))
	{
		int num = -1;
		while (num != 0)
		{
			printf("1. ��ü �ù� ����\n2. �ù� ��� ����(Ȯ�θ� ����)\n3. ȸ�� ����(Ȯ�θ� ����)\n0. ����\n");
			scanf("%d", &num);
			switch (num)
			{
			case 1:
				print_all_parcel(NULL, p_head, 2);
				break;
			case 2:
				print_worker(w_head);
				break;
			case 3:
				print_member(u_head);
				break;
			}
		}
	}
}

void input_user(user_head** head)		//���Ϻҷ��� ȸ������
{
	(*head)->Mambers = 0;
	user* New = (user*)malloc(sizeof(user));
	FILE* fin;
	fin = fopen("member.txt", "r");
	if (fin != NULL)
	{
		while ((fscanf(fin, "%s", New->name) != EOF))
		{
			fscanf(fin, "%s", New->PW);
			fscanf(fin, "%s", New->phone);
			fscanf(fin, "%s", New->Adress_Big);
			fscanf(fin, "%s", New->Adress_small);
			input_user_data(head, *New);
			(*head)->Mambers++;
		}
	}
	fclose(fin);
}

void input_user_data(user_head** head, user New)
{
	user* New_in = (user*)malloc(sizeof(user));
	strcpy(New_in->Adress_Big, New.Adress_Big);
	strcpy(New_in->Adress_small, New.Adress_small);
	strcpy(New_in->name, New.name);
	strcpy((char*)New_in->phone, (char*)New.phone);
	strcpy((char*)New_in->PW, (char*)New.PW);
	New_in->Next = NULL;
	user* p = (*head)->first;
	if ((*head)->first == NULL)
		(*head)->first = New_in;
	else
	{
		while (p->Next != NULL)
			p = p->Next;
		p->Next = New_in;
	}

}

void input_parcel(parcel** head)		//���Ϻҷ��� �ù賻������
{
	int num;
	parcel_user* to = (parcel_user*)malloc(sizeof(parcel_user));
	parcel_user* from = (parcel_user*)malloc(sizeof(parcel_user));
	(*head)->parcel_ready_num = 0;
	(*head)->parcel_ing_num = 0;
	(*head)->parcel_ed_num = 0;
	FILE* fin;
	fin = fopen("parcel.txt", "r");
	if (fin != NULL)
	{
		while (1)
		{
			if (fscanf(fin, "%d", &num) == EOF)
				break;
			if (num == 1)
			{
				fscanf(fin, "%s", to->name);
				fscanf(fin, "%s", to->phone);
				fscanf(fin, "%s", to->Adress_Big);
				fscanf(fin, "%s", to->Adress_small);
				fscanf(fin, "%s", from->name);
				fscanf(fin, "%s", from->phone);
				fscanf(fin, "%s", from->Adress_Big);
				fscanf(fin, "%s", from->Adress_small);
			}
			else if (num == 2)
			{
				fscanf(fin, "%s", to->name);
				fscanf(fin, "%s", to->phone);
				fscanf(fin, "%s", to->Adress_Big);
				fscanf(fin, "%s", to->Adress_small);
				fscanf(fin, "%s", from->name);
				fscanf(fin, "%s", from->phone);
				fscanf(fin, "%s", from->Adress_Big);
				fscanf(fin, "%s", from->Adress_small);
			}
			else if (num == 3)
			{
				fscanf(fin, "%s", to->name);
				fscanf(fin, "%s", to->phone);
				fscanf(fin, "%s", to->Adress_Big);
				fscanf(fin, "%s", to->Adress_small);
				fscanf(fin, "%s", from->name);
				fscanf(fin, "%s", from->phone);
				fscanf(fin, "%s", from->Adress_Big);
				fscanf(fin, "%s", from->Adress_small);
			}
			input_parcel_data(head, *to, *from, num);
		}
		fclose(fin);
	}
}

void input_parcel_data(parcel** head, parcel_user to_data, parcel_user from_data, int num)
{
	parcel_ing* new_ing;
	parcel_ed* new_ed;
	parcel_ready* new_ready;
	parcel_ready* ready_p = (*head)->ready;
	parcel_ing* ing_p = (*head)->ing;
	parcel_ed* ed_p = (*head)->ed;
	parcel_user* in_to = (parcel_user*)malloc(sizeof(parcel_user));
	parcel_user* in_from = (parcel_user*)malloc(sizeof(parcel_user));
	strcpy(in_to->Adress_Big, to_data.Adress_Big);
	strcpy(in_to->Adress_small, to_data.Adress_small);
	strcpy(in_to->name, to_data.name);
	strcpy((char*)in_to->phone, (char*)to_data.phone);
	strcpy(in_from->Adress_Big, from_data.Adress_Big);
	strcpy(in_from->Adress_small, from_data.Adress_small);
	strcpy(in_from->name, from_data.name);
	strcpy((char*)in_from->phone, (char*)from_data.phone);

	if (num == 1)
	{
		new_ready = (parcel_ready*)malloc(sizeof(parcel_ready));
		new_ready->from = in_from;
		new_ready->to = in_to;
		new_ready->next = NULL;

		if ((*head)->ready == NULL)
			(*head)->ready = new_ready;
		else
		{
			while (ready_p->next != NULL)
				ready_p = ready_p->next;
			ready_p->next = new_ready;
		}
		(*head)->parcel_ready_num = (*head)->parcel_ready_num + 1;
	}
	else if (num == 2)
	{
		new_ing = (parcel_ing*)malloc(sizeof(parcel_ing));
		new_ing->from = in_from;
		new_ing->to = in_to;
		new_ing->next = NULL;
		if ((*head)->ing == NULL)
			(*head)->ing = new_ing;
		else
		{
			while (ing_p->next != NULL)
				ing_p = ing_p->next;
			ing_p->next = new_ing;
		}
		(*head)->parcel_ing_num = (*head)->parcel_ing_num + 1;
	}
	else if (num == 3)
	{
		new_ed = (parcel_ed*)malloc(sizeof(parcel_ed));
		new_ed->from = in_from;
		new_ed->to = in_to;
		new_ed->next = NULL;
		if ((*head)->ed == NULL)
			(*head)->ed = new_ed;
		else
		{
			while (ed_p->next != NULL)
				ed_p = ed_p->next;
			ed_p->next = new_ed;
		}
		(*head)->parcel_ed_num = (*head)->parcel_ed_num + 1;
	}
}

void input_worker(worker_head** head)	//���Ϻҷ��� �ù��� ����
{
	worker* p = (*head)->head;
	worker* New = (worker*)malloc(sizeof(worker));
	FILE* fin;
	fin = fopen("worker.txt", "r");
	if (fin != NULL)
	{
		while ((fscanf(fin, "%s", New->name) != EOF))
		{
			fscanf(fin, "%s\n", New->PW);
			fscanf(fin, "%s\n", New->Adress_Big);
			fscanf(fin, "%d\n", (&New->successed));
			input_worker_data(head, *New);
		}

		fclose(fin);
	}
}

void input_worker_data(worker_head** head, worker data)
{
	worker* p = (*head)->head;
	worker* New = (worker*)malloc(sizeof(worker));
	strcpy(New->Adress_Big, data.Adress_Big);
	strcpy(New->name, data.name);
	strcpy((char*)New->PW, (char*)data.PW);
	New->successed = data.successed;
	New->next = NULL;
	if ((*head)->head == NULL)
		(*head)->head = New;
	else
	{
		while (p->next != NULL)
			p = p->next;
		p->next = New;
	}
}

void input_masterPW(int MasterPW[10])	//���Ϻҷ��� �����ͺ�й�ȣ ����
{
	FILE* fin;
	fin = fopen("master.txt", "r");
	if (fin != NULL)
	{
		fscanf(fin, "%s", MasterPW);
		fclose(fin);
	}
}

void add_user(user_head** head)
{
	int num;
	user* p = (*head)->first;
	user* New = (user*)malloc(sizeof(user));
	printf("�����Է�\n");
	printf("�̸�: ");
	scanf("%s", New->name);
	printf("��ȭ��ȣ: ");
	scanf("%s", New->phone);
	printf("�ּ�(����)�� ������\n");
	strcpy(New->Adress_Big, selec_area());
	printf("�ּ�(���ּ�): ");
	scanf("%s", New->Adress_small);
	printf("��й�ȣ: ");
	scanf("%s", New->PW);
	if ((*head)->first == NULL)
	{
		(*head)->first = New;
	}
	else
	{
		while (p->Next != NULL)
			p = p->Next;
		p->Next = New;
	}
}

void add_parcel(parcel** head, user* User)					//��ǲ������ �ʿ�(����)
{
	parcel_ready* p = (*head)->ready;
	parcel_ready* New = (parcel_ready*)malloc(sizeof(parcel_ready));
	parcel_user* to = (parcel_user*)malloc(sizeof(parcel_user));
	parcel_user* from = (parcel_user*)malloc(sizeof(parcel_user));

	strcpy(to->Adress_Big, User->Adress_Big);
	strcpy(to->Adress_small, User->Adress_small);
	strcpy(to->name, User->name);
	for (int i = 0; i < 20; i++)
		to->phone[i] = User->phone[i];
	printf("�޴»�� �����Է�\n");
	printf("�̸�: ");
	scanf("%s", from->name);
	printf("��ȭ��ȣ: ");
	scanf("%s", from->phone);
	printf("�ּ�(����)�� ������\n");
	strcpy(from->Adress_Big, selec_area());
	printf("�ּ�(���ּ�): ");
	scanf("%s", from->Adress_small);

	New->to = to;
	New->from = from;
	New->next = NULL;

	if ((*head)->ready == NULL)
		(*head)->ready = New;
	else
	{
		while (p->next != NULL)
			p = p->next;
		p->next = New;
	}
	(*head)->parcel_ready_num = (*head)->parcel_ready_num + 1;
}

user* login_user(user_head* head)
{
	int PW[10];
	char name[10];
	user* User = head->first;
	printf("�̸�: ");
	scanf("%s", name);
	printf("�н�����: ");
	scanf("%s", PW);
	while (User != NULL)
	{
		if (!strcmp(name, User->name))
		{
			if (!strcmp((char*)PW, (char*)User->PW))
				return User;
			else
				User = NULL;
		}

		else
		{
			if (User->Next != NULL)
				User = User->Next;
		}
	}
	return User;
}

void search_parcel(parcel** head, int num)
{
	int num2;
	char name[20];
	printf("������ ��� �̸� :");
	scanf("%s", name);
	parcel_ready* ready = (*head)->ready;
	parcel_ing* ing = (*head)->ing;
	parcel_ed* ed = (*head)->ed;
	while (ready)
	{
		if (!strcmp(ready->to->name, name))
		{
			print_parcel(ready->to, 1);
			printf("           ��\n");
			print_parcel(ready->from, 2);
			if (num == 2)
			{
				printf("1.���� 0.����: ");
				scanf("%d", &num2);
				if (num2 == 1)
					change_from_parcel(&(ready->from));
			}
			return;
		}
		ready = ready->next;
	}
	while (ing)
	{
		if (!strcmp(ing->to->name, name))
		{
			print_parcel(ing->to, 1);
			printf("           ��\n");
			print_parcel(ing->from, 2);
			if (num == 2)
			{
				printf("1.���� 0.����: ");
				scanf("%d", &num2);
				if (num2 == 1)
					change_from_parcel(&(ing->from));
			}
			return;
		}
		ing = ing->next;
	}
	while (ed && num != 2)
	{
		if (!strcmp(ed->to->name, name))
		{
			print_parcel(ed->to, 1);
			printf("           ��\n");
			print_parcel(ed->from, 2);
			return;
		}
		ed = ed->next;
	}
	printf("�������� �ù谡 �����ϴ�\n");
}

void print_parcel(parcel_user* User, int num)
{
	if (num == 1)
		printf("������");
	else
		printf("�޴�");
	printf(" ��� : %s   ��ȣ : %s \n", User->name, User->phone);

	printf("�ּ� : %s %s\n", User->Adress_Big, User->Adress_small);
}

void change_from_parcel(parcel_user** User)
{
	char name[10], Ad_small[50];
	int phone[20];
	int num = -1;
	while (num)
	{
		printf("�޴»���� ������ ���氡���մϴ�");
		printf("1.�̸����� 2. ��ȭ��ȣ ���� 3. �ּҺ��� 4. ���ּҺ��� 0.����");
		printf("�����ϼ���: ");
		scanf("%d", &num);
		switch (num)
		{
		case 1:
			printf("�̸�: ");
			scanf("%s", name);
			strcpy((*User)->name, name);
			break;
		case 2:
			printf("��ȭ��ȣ: ");
			scanf("%s", phone);
			strcpy((char*)(*User)->phone, (char*)phone);
			break;
		case 3:
			printf("�ּ�(����)�� ������\n");
			strcpy((*User)->Adress_Big, selec_area());
			break;
		case 4:
			printf("�ּ�(���ּ�): ");
			scanf("%s", Ad_small);
			strcpy((*User)->Adress_small, Ad_small);
			break;
		case 0:
			break;
		default:
			printf("�߸����̽��ϴ�\n");
		}
	}


}

void print_all_parcel(worker_head** w_head, parcel** p_head, int num1)
{
	int num2 = -1, num3, num4, num5;
	if ((*w_head) == NULL)
		num3 = 1;
	else
		num3 = 2;


	if (num1 == 2)
		num4 = 1;
	else if (num3 == 1)
		num4 = 2;
	else
		num4 = 0;

	parcel_ready* ready = (*p_head)->ready;
	parcel_ing* ing = (*p_head)->ing;
	parcel_ed* ed = (*p_head)->ed;

	while (num2 != 0)
	{
		printf("����� ���¸� ���ÿ�");
		printf("1.����غ��� 2.����� 3.����� 0.����");
		scanf("%d", &num2);
		switch (num2)
		{
		case 1:
			while (ready)
			{
				switch (num3)
				{
				case 1:
					printf("����غ���");
					print_parcel(ready->to, 1);
					printf("           ��\n");
					print_parcel(ready->from, 2);
					ready = ready->next;
					break;
				case 2:
					if (!strcmp((*w_head)->head->Adress_Big, ready->from->Adress_Big))
					{
						print_parcel(ready->to, 1);
						printf("           ��\n");
						print_parcel(ready->from, 2);
					}
					ready = ready->next;
					break;
				}
			}
			break;
		case 2:
			while (ing)
			{
				printf("��� ��");
				switch (num3)
				{
				case 1:
					print_parcel(ing->to, 1);
					printf("           ��\n");
					print_parcel(ing->from, 2);
					ing = ing->next;
					break;
				case 2:
					if (!strcmp((*w_head)->head->Adress_Big, ing->from->Adress_Big))
					{
						print_parcel(ing->to, 1);
						printf("           ��\n");
						print_parcel(ing->from, 2);
					}
					ing = ing->next;
					break;
				}
			}
			break;
		case 3:
			while (ed)
			{
				printf("��� �Ϸ�");
				switch (num3)
				{
				case 1:
					print_parcel(ed->to, 1);
					printf("           ��\n");
					print_parcel(ed->from, 2);
					ed = ed->next;
					break;
				case 2:
					if (!strcmp((*w_head)->head->Adress_Big, ed->from->Adress_Big))
					{
						print_parcel(ed->to, 1);
						printf("           ��\n");
						print_parcel(ed->from, 2);
					}
					ed = ed->next;
					break;
				}
			}
			break;
		}
		if (num4 && num2 != 3)
		{
			printf("���´� ���̴� ��� �ù��� ���°� ����˴ϴ�.(����غ���->�����, �����->��ۿϷ�");
			printf("���¸� �����Ͻ÷��� 1�� �Է����ּ���: ");
			scanf("%d", &num5);
			if (num5 == 1)
			{
				if (num4 == 1)
					change_state_parcel(p_head, (*w_head)->head, num2);

				else
					change_state_parcel(p_head, NULL, num2);
			}
		}

	}
}

char* selec_area()
{
	int num = -1;
	char area[10];
	while (num)
	{
		printf("1.���� 2.��⵵(��õ) 3.������\n4.��û�� 5.��� 6.���� 7.���ֵ�\n");
		printf("�����ϼ���: ");
		scanf("%d", &num);
		switch (num)
		{
		case 1:
			strcpy(area, "����");
			break;
		case 2:
			strcpy(area, "��⵵");
			break;
		case 3:
			strcpy(area, "������");
			break;
		case 4:
			strcpy(area, "��û��");
			break;
		case 5:
			strcpy(area, "���");
			break;
		case 6:
			strcpy(area, "����");
			break;
		case 7:
			strcpy(area, "���ֵ�");
			break;
		default:
			printf("�߸� ���˽��ϴ�\n");
			break;
		}
		if (num < 8 && num>0)
			break;
	}
	return area;
}

void change_state_parcel(parcel** p_head, worker* w_head, int num1)
{
	parcel_ready* ready = (*p_head)->ready;
	parcel_ing* ing = (*p_head)->ing;
	parcel_ed* ed = (*p_head)->ed;

	if (w_head == NULL)
	{
		switch (num1)
		{
		case 1:
			parcel_ready * del_ready;
			while (ing)
				ing = ing->next;
			while (ready)
			{
				del_ready = ready;
				input_reday_to_ing(&ing, &ready);
				ing = ing->next;
				ready = ready->next;
				free(del_ready);
			}
			(*p_head)->parcel_ing_num = (*p_head)->parcel_ing_num + (*p_head)->parcel_ready_num;
			(*p_head)->parcel_ready_num = 0;
			(*p_head)->ready = NULL;
			break;
		case 2:
			parcel_ing * del_ing;
			while (ed)
				ed = ed->next;
			while (ing)
			{
				del_ing = ing;
				input_ing_to_ed(&ed, &ing);
				ed = ed->next;
				ing = ing->next;
				free(del_ing);
			}
			(*p_head)->parcel_ed_num = (*p_head)->parcel_ing_num + (*p_head)->parcel_ing_num;
			(*p_head)->parcel_ing_num = 0;
			(*p_head)->ing = NULL;
			break;
		}
	}
	else
	{
		switch (num1)
		{
		case 1:
			parcel_ready * del_ready;
			while (ing)
				ing = ing->next;
			while (ready)
			{

				if (!strcmp(w_head->Adress_Big, ed->from->Adress_Big))
				{
					del_ready = ready;
					input_reday_to_ing(&ing, &ready);
					free(del_ready);
					(*p_head)->parcel_ing_num++;
					(*p_head)->parcel_ready_num--;
					ing = ing->next;
				}
				ready = ready->next;
			}
			break;
		case 2:
			parcel_ing * del_ing;
			while (ed)
				ed = ed->next;
			while (ing)
			{
				if (!strcmp(w_head->Adress_Big, ed->from->Adress_Big))
				{
					del_ing = ing;
					input_ing_to_ed(&ed, &ing);
					free(del_ing);
					ed = ed->next;
					(*p_head)->parcel_ed_num++;
					(*p_head)->parcel_ing_num--;
				}
				ing = ing->next;
			}
			break;
		}
	}
}

void input_reday_to_ing(parcel_ing** ing, parcel_ready** ready)
{
	parcel_ing* New = (parcel_ing*)malloc(sizeof(parcel_ing));
	New->to = (*ready)->to;
	(*ready)->to = NULL;
	New->from = (*ready)->from;
	(*ready)->from = NULL;
	(*ing) = New;
}

void input_ing_to_ed(parcel_ed** ed, parcel_ing** ing)
{
	parcel_ed* New = (parcel_ed*)malloc(sizeof(parcel_ed));
	New->to = (*ing)->to;
	(*ing)->to = NULL;
	New->from = (*ing)->from;
	(*ing)->from = NULL;
	(*ed) = New;
}

void print_worker(worker_head** head)
{
	worker* Worker = (*head)->head;
	while (Worker)
	{
		printf("�̸�: %s ����: %s ", Worker->name, Worker->Adress_Big);
		Worker = Worker->next;
	}
}

void print_member(user_head** head)
{
	user* User = (*head)->first;
	while (User)
	{
		printf("�̸�: %s, ����: %s, ��ȭ��ȣ: %s", User->name, User->Adress_Big, User->phone);
		User = User->Next;
	}
}

void output_member(user_head** head)
{
	int i = 0;
	FILE* fout;
	fout = fopen("member.txt", "w");
	user_head* New = *head;
	i = New->Mambers;

	while (i != 0)
	{
		fprintf(fout, "%s\n", New->first->name);
		fprintf(fout, "%s\n", New->first->PW);
		fprintf(fout, "%s\n", New->first->phone);
		fprintf(fout, "%s\n", New->first->Adress_Big);
		fprintf(fout, "%s\n", New->first->Adress_small);
		New->first = New->first->Next;
		i--;
	}
	fclose(fout);
}

void output_parcel(parcel** head)
{
	int i = 0;
	parcel* p = *head;
	FILE* fout;
	fout = fopen("parcel.txt", "w");

	for (i = 0; i < p->parcel_ready_num; i++)
	{
		fprintf(fout, "1\n");
		fprintf(fout, "%s\n", p->ready->to->name);
		fprintf(fout, "%s\n", p->ready->to->phone);
		fprintf(fout, "%s\n", p->ready->to->Adress_Big);
		fprintf(fout, "%s\n", p->ready->to->Adress_small);
		fprintf(fout, "%s\n", p->ready->from->name);
		fprintf(fout, "%s\n", p->ready->from->phone);
		fprintf(fout, "%s\n", p->ready->from->Adress_Big);
		fprintf(fout, "%s\n", p->ready->from->Adress_small);
		if (p->ready->next != NULL)
			p->ready = p->ready->next;
	}
	for (i = 0; i < p->parcel_ing_num; i++)
	{
		fprintf(fout, "2\n");
		fprintf(fout, "%s\n", p->ing->to->name);
		fprintf(fout, "%s\n", p->ing->to->phone);
		fprintf(fout, "%s\n", p->ing->to->Adress_Big);
		fprintf(fout, "%s\n", p->ing->to->Adress_small);
		fprintf(fout, "%s\n", p->ing->from->name);
		fprintf(fout, "%s\n", p->ing->from->phone);
		fprintf(fout, "%s\n", p->ing->from->Adress_Big);
		fprintf(fout, "%s\n", p->ing->from->Adress_small);
		if (p->ing->next != NULL)
			p->ing = p->ing->next;
	}
	for (i = 0; i < p->parcel_ed_num; i++)
	{
		fprintf(fout, "3");
		fprintf(fout, "%s", p->ed->to->name);
		fprintf(fout, "%s", p->ed->to->phone);
		fprintf(fout, "%s", p->ed->to->Adress_Big);
		fprintf(fout, "%s", p->ed->to->Adress_small);
		fprintf(fout, "%s", p->ed->from->name);
		fprintf(fout, "%s", p->ed->from->phone);
		fprintf(fout, "%s", p->ed->from->Adress_Big);
		fprintf(fout, "%s", p->ed->from->Adress_small);

		p->ed = p->ed->next;
	}
	fclose(fout);
}

void output_worker(worker_head** head)
{
	worker_head* p = *head;
	FILE* fout;
	fout = fopen("worker.txt", "w");
	while (1)
	{
		fprintf(fout, "%s\n", p->head->name);
		fprintf(fout, "%s\n", p->head->PW);
		fprintf(fout, "%s\n", p->head->Adress_Big);
		fprintf(fout, "%d\n", p->head->successed);
		if (p->head->next == NULL)
			break;
		p->head = p->head->next;
	}
	fclose(fout);
}