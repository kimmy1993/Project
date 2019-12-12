#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include "face_Recognition.h"

class Function
{
	private:
		char path[100];									//장소를 지정하기 위한 char형 변수
		typedef struct user_num
		{
			int id;
			int num;
			struct user_num *next;
		}user_num;

	public:
		bool Allpreprocessing(int num);
		bool Recognition(int num,int image_num, int *user, face_Recognition face_Recognition);
		bool Insertimage(int num);
		bool user_dlist(int id,int n,user_num *dlist);
		bool user_dlist_init(user_num *dlist);
		bool sort(user_num *dlist);
};

#endif