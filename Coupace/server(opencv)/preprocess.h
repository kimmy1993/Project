#ifndef _PREPROCESS_H_
#define _PREPROCESS_H_

#include "header.h"

//Ÿ���̹����� ���� ���� ����
#define LONGAXIS_RATE 13
#define SHORTAXIS_RATE 18

//�̸����� ����
#define SIZE_FOREHEAD 20

//�� ����
#define EYE_LEN 50

//���� ���� ��ġ�� ������ ��ǥ�� ����
#define SET_EYE_X 75
#define SET_EYE_Y 125

#define PI 3.141592
#define HIST_SIZE 256

//��ó���� �̹����� ũ�� ����
#define PRE_HEIGHT 120
#define PRE_WIDTH 100

class preprocess
{
	public:
		int num;
		IplImage* makeOvalImage(int height, int width);
		bool ovalSpaceHistEqual(IplImage* src, IplImage* ovalImage);
		bool preprocessing(IplImage* image,IplImage* preImage, IplImage* ovalImage, CvRect* leftEyeRect, CvRect* rightEyeRect);
		bool saveimage(IplImage* image, int num);
		bool faceHist(IplImage* image);

};

#endif