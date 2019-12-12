#ifndef _DETECTION_H_
#define _DETECTION_H_

#include "header.h"
#include <opencv2/opencv.hpp>
#include <vector>

class Detection
{
	private:
		const char* faceClassifer;
		const char* face2Classifer;
		const char* leftEyeClassifer;
		const char* rightEyeClassifer;

	public:
		Detection();																																					//������, �з��� ��ġ �ʱ�ȭ

		bool face(IplImage* image, CvRect* faceRect);

		bool faceDetection(IplImage* image, CvRect* faceRect,CvHaarClassifierCascade* cascade_face, CvMemStorage* storage_face);										//�� ������ ó���� �ش�.(image, rect�� ����ü, �� �з���,�Ҵ�� �����)

		bool eye(IplImage* faceImage, CvRect* leftEyeRect, CvRect* rightEyeRect);

		//�� ������ ó���� �ش�(�� Ȯ�� �̹���, ���� �� ���� ���� rect�� ����ü, ������ �� ���� ����ü,���� �� �з���, ���ʴ� ó�� �����, ������ �з��� ,������ ����� )
		bool eyeDetection(IplImage* faceImage, CvRect* leftEyeRect, CvRect* rightEyeRect,CvHaarClassifierCascade* cascade_leftEye, CvMemStorage* storage_leftEye,CvHaarClassifierCascade* cascade_rightEye, CvMemStorage* storage_rightEye);


};

#endif