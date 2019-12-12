#ifndef _USER_NUM_H_
#define _USER_NUM_H_

#include "header.h"

class user_Num
{
	private:
		int user_num;
		FILE* fnum;
	public:
		user_Num();
		int read_num();
		bool write_num(int num);
		bool close_num();
};

#endif