#include "cv.h"

using namespace std;
int* cv_data::cv_main(char set, int pic_num)
{
	switch (set)																		//����� �����Ѵ�.
	{
	case '1' :																	//��ü ���� ��ó���� �����Ѵ�.
		function.Allpreprocessing(num);
		break;
	default:
		image_num = pic_num;
		function.Recognition(num, image_num, user, face_recognition);
		
		cout << "finsh" << endl;
		break;
	}

	if (!user_num.close_num())															//��������� ���� ���� �Է¹����� �����Ѵ�.
	{
		cout << "close error";
	}

	return user;
}

void cv_data::cv_clear()
{
	smaile = 0;
	num = 0;
	image_num = 0;
	set = 0;
	for (int i = 0; i < 3; i++)
		user[i] = 0;
	num = user_num.read_num();															//������� ������ �Է¹޴´�.

}

void cv_data::cv_trainning()
{

	face_recognition.faceses(num);

	face_recognition.training();

}

void cv_data::cv_smaile()
{
	smaile = 1;
}

int cv_data::cv_get_smaile()
{
	if (smaile == 1)
	{
		smaile = 0;
		return 1;
	}
	else
		return 0;
}

void cv_data::cv_smaile_clear()
{
	smaile = 0;
}

bool cv_data::cv_input()
{
		if (function.Insertimage(num) == FALSE)
		{
			printf("insert error");
			return false;
		}
		else
		{
			num++;
			if (user_num.write_num(num) == FALSE)
			{
				printf("insert write error");
				return false;
			}
			face_recognition.setnum(num);
			face_recognition.training();
		}
		cout << num;

		if (!user_num.close_num())															//��������� ���� ���� �Է¹����� �����Ѵ�.
		{
			cout << "close error";
		}
		return true;
}