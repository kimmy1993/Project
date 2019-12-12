#ifndef _FACE_RECOGNITION_H_

#define _FACE_RECOGNITION_H_

#include "Detection.h"
#include "preprocess.h"

class face_Recognition
{
	private:
		//학습 이미지 저장
		IplImage** train_img;
		//고유얼굴 저장
		IplImage** eigen_img;
		//평균얼굴 저장
		IplImage* avg;
		//사영된 얼굴 저장
		IplImage* proj;
		//고유값 저장
		float* eigen_Value;
		//고유벡터
		float** coeffs;
		//반복 조건
		CvTermCriteria criteria;
		//얼굴 숫자
		int num_Faces;
	public:
		void faceses(int num);
		void training();
		bool recognition(IplImage* image, int *matchId);
		IplImage* face_preprocess(IplImage* image, int set);
		long double distances(float* trainVec, float* inVec);
		void setnum(int num);
};

#endif