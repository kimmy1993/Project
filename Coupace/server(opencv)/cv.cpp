#include "cv.h"

using namespace std;
int* cv_data::cv_main(char set, int pic_num)
{
	switch (set)																		//기능을 선택한다.
	{
	case '1' :																	//전체 고객의 전처리를 수행한다.
		function.Allpreprocessing(num);
		break;
	default:
		image_num = pic_num;
		function.Recognition(num, image_num, user, face_recognition);
		
		cout << "finsh" << endl;
		break;
	}

	if (!user_num.close_num())															//현재까지의 고객의 수를 입력받으면 종료한다.
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
	num = user_num.read_num();															//현재까지 고객수를 입력받는다.

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

		if (!user_num.close_num())															//현재까지의 고객의 수를 입력받으면 종료한다.
		{
			cout << "close error";
		}
		return true;
}