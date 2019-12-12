#include "user_Num.h"

using namespace std;

user_Num::user_Num()
{
	if ((fnum = fopen("./FERET/set/user_num.bin", "rb+")) == NULL)
	{
		cout << "파일이 없습니다\n";
	}
	if (fread(&(this->user_num), sizeof(int), 1, fnum) <= 0)
	{
		cout << ("파일을 읽을 수 없습니다\n");
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
		cout << "파일이 없습니다\n";
		return false;
	}
	if (fwrite(&(this->user_num), sizeof(int), 1, fnum) <= 0)
	{
		cout << ("파일을 쓸 수 없습니다\n");
	}

	fclose(this->fnum);

	return true;
}
