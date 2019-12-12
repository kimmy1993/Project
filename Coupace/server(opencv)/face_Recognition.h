#ifndef _FACE_RECOGNITION_H_

#define _FACE_RECOGNITION_H_

#include "Detection.h"
#include "preprocess.h"

class face_Recognition
{
	private:
		//�н� �̹��� ����
		IplImage** train_img;
		//������ ����
		IplImage** eigen_img;
		//��վ� ����
		IplImage* avg;
		//�翵�� �� ����
		IplImage* proj;
		//������ ����
		float* eigen_Value;
		//��������
		float** coeffs;
		//�ݺ� ����
		CvTermCriteria criteria;
		//�� ����
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