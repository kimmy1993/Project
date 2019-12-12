#include "user_Num.h"

using namespace std;

user_Num::user_Num()
{
	if ((fnum = fopen("./FERET/set/user_num.bin", "rb+")) == NULL)
	{
		cout << "������ �����ϴ�\n";
	}
	if (fread(&(this->user_num), sizeof(int), 1, fnum) <= 0)
	{
		cout << ("������ ���� �� �����ϴ�\n");
	}
	fclose(this->fnum);
}

int user_Num::read_num()
{
	return this->user_num;
}

bool user_Num::write_num(int num)
{
	this->user_num = num;

	return true;
}

bool user_Num::close_num()
{
	if ((fnum = fopen("./FERET/set/user_num.bin", "wb+")) == NULL)
	{
		cout << "������ �����ϴ�\n";
		return false;
	}
	if (fwrite(&(this->user_num), sizeof(int), 1, fnum) <= 0)
	{
		cout << ("������ �� �� �����ϴ�\n");
	}

	fclose(this->fnum);

	return true;
}
