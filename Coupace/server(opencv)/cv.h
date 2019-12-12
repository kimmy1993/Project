#pragma once
#include "user_Num.h"
#include "Function.h"

class cv_data
{
private :
	int smaile;
	int num;
	int image_num;
	char set;
	int user[3];
	user_Num user_num;
	Function function;
	face_Recognition face_recognition;
public:
	void cv_clear();
	int* cv_main(char,int);
	void cv_trainning();
	void cv_smaile();
	void cv_smaile_clear();
	int cv_get_smaile();
	bool cv_input();
};

