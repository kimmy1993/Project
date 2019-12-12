#ifndef _PREPROCESS_H_
#define _PREPROCESS_H_

#include "header.h"

//타원이미지의 장축 단축 비율
#define LONGAXIS_RATE 13
#define SHORTAXIS_RATE 18

//이마영역 제거
#define SIZE_FOREHEAD 20

//눈 길이
#define EYE_LEN 50

//왼쪽 눈의 위치를 다음의 좌표로 고정
#define SET_EYE_X 75
#define SET_EYE_Y 125

#define PI 3.141592
#define HIST_SIZE 256

//전처리된 이미지의 크기 지정
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