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
		Detection();																																					//생성자, 분류기 위치 초기화

		bool face(IplImage* image, CvRect* faceRect);

		bool faceDetection(IplImage* image, CvRect* faceRect,CvHaarClassifierCascade* cascade_face, CvMemStorage* storage_face);										//얼굴 검출을 처리해 준다.(image, rect형 구조체, 얼굴 분류기,할당된 저장소)

		bool eye(IplImage* faceImage, CvRect* leftEyeRect, CvRect* rightEyeRect);

		//눈 검출을 처리해 준다(얼굴 확대 이미지, 왼쪽 눈 저장 위한 rect형 구조체, 오른쪽 눈 저장 구조체,왼쪽 눈 분류기, 왼쪽눈 처리 저장소, 오른쪽 분류기 ,오른쪽 저장소 )
		bool eyeDetection(IplImage* faceImage, CvRect* leftEyeRect, CvRect* rightEyeRect,CvHaarClassifierCascade* cascade_leftEye, CvMemStorage* storage_leftEye,CvHaarClassifierCascade* cascade_rightEye, CvMemStorage* storage_rightEye);


};

#endif