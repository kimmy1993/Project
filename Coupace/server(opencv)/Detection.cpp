#include "Detection.h"

using namespace std;

Detection::Detection()
{
	//환경에 맞게 경로설정
	this->faceClassifer = "./cascade/haarcascade_frontalface_default.xml";
	this->face2Classifer = "./cascade/lbpcascade_frontalface.xml";
	this->leftEyeClassifer = "./cascade/haarcascade_mcs_lefteye.xml";
	this->rightEyeClassifer = "./cascade/haarcascade_mcs_righteye.xml";
}

bool Detection::face(IplImage* image, CvRect* faceRect)													//얼굴 검출
{
	vector<cv::Rect> rect;
	//분류기 로드
	CvHaarClassifierCascade* cascade_face = 0;
	cv::CascadeClassifier cascade_face2;
	if (!(cascade_face = (CvHaarClassifierCascade*)cvLoad(faceClassifer, 0, 0, 0)))
	{
		cout << ("error : face cascade error \n");
		return false;
	}
	if (!(cascade_face2.load(face2Classifer)))
	{
		cout << ("error : face cascade2 error \n");
		return false;
	}

	//메모리 할당
	CvMemStorage* storage_face = 0;

	if (!(storage_face = cvCreateMemStorage(0)))
	{
		cvReleaseHaarClassifierCascade(&cascade_face);
		cout << ("error : storage error \n");
		return false;
	}
	cv::Mat src = cv::Mat(image, true);
	//얼굴 검출(lbp->harr)
	cascade_face2.detectMultiScale(src, rect, 1.1, 5, cv::CASCADE_FIND_BIGGEST_OBJECT | cv::CASCADE_DO_ROUGH_SEARCH, cv::Size(50, 50));
	if (rect.size() == 0)
	{
		if (!faceDetection(image, faceRect, cascade_face, storage_face))
		{
			cout << "face zero\n";
			return false;
		}
	}
	else
	{
		faceRect->x = rect[0].x;
		faceRect->y = rect[0].y;
		faceRect->width = rect[0].width;
		faceRect->height = rect[0].height;
	}

	image = &IplImage(src);

	//메모리 해제
	cvReleaseHaarClassifierCascade(&cascade_face);
	cvReleaseMemStorage(&storage_face);

	return true;
}

bool Detection::faceDetection(IplImage* image, CvRect* faceRect, CvHaarClassifierCascade* cascade_face, CvMemStorage* storage_face)
{
	CvSeq* faces = 0;																						//다중 구조체 리스트 여기서는 rect를 저장하기 위해 사용(검출된 얼굴들을 저장)
	CvRect* tempR = 0;																						//검출된 얼굴의 x,y좌표와 길이 저장을 위한 구조체

																											//얼굴검출 (검출된 얼굴들 중 가장 큰 객체 반환)
	faces = cvHaarDetectObjects(image, cascade_face, storage_face, 1.1, 5, CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH, cvSize(50, 50));

	//검출된 얼굴의 수가 0이면
	if (faces->total == 0)
	{
		return false;
	}

	//검출된 얼굴의 x,y좌표 height,width 저장
	tempR = (CvRect*)cvGetSeqElem(faces, 0);
	faceRect->x = tempR->x;
	faceRect->y = tempR->y;
	faceRect->height = tempR->height + 5;
	faceRect->width = tempR->width;

	return true;
}

bool Detection::eye(IplImage* faceImage, CvRect* leftEyeRect, CvRect* rightEyeRect)
{
	//분류기 로드
	CvHaarClassifierCascade* cascade_leftEye = 0;
	CvHaarClassifierCascade* cascade_rightEye = 0;

	if (!(cascade_leftEye = (CvHaarClassifierCascade*)cvLoad(leftEyeClassifer, 0, 0, 0)))
	{
		cout << ("error : left cascade error \n");
		return false;
	}
	if (!(cascade_rightEye = (CvHaarClassifierCascade*)cvLoad(rightEyeClassifer, 0, 0, 0)))
	{
		cout << ("error : right cascade error \n");
		return false;
	}

	//메모리 할당
	CvMemStorage* storage_leftEye = 0;
	CvMemStorage* storage_rightEye = 0;

	if (!(storage_leftEye = cvCreateMemStorage(0)))
	{
		cvReleaseHaarClassifierCascade(&cascade_leftEye);
		cout << ("error : storage error \n");
		return false;
	}
	if (!(storage_rightEye = cvCreateMemStorage(0)))
	{
		cvReleaseHaarClassifierCascade(&cascade_rightEye);
		cout << ("error : storage error \n");
		return false;
	}

	if (!eyeDetection(faceImage, leftEyeRect, rightEyeRect, cascade_leftEye, storage_leftEye, cascade_rightEye, storage_rightEye))
	{
		return false;
	}

	//메모리 해제
	cvReleaseHaarClassifierCascade(&cascade_leftEye);
	cvReleaseHaarClassifierCascade(&cascade_rightEye);
	cvReleaseMemStorage(&storage_leftEye);
	cvReleaseMemStorage(&storage_rightEye);

	return true;
}

bool Detection::eyeDetection(IplImage* faceImage, CvRect* leftEyeRect, CvRect* rightEyeRect, CvHaarClassifierCascade* cascade_leftEye, CvMemStorage* storage_leftEye, CvHaarClassifierCascade* cascade_rightEye, CvMemStorage* storage_rightEye)
{
	CvSeq* leftEye = 0;																	//다중 구조체 리스트 여기서는 rect를 저장하기 위해 사용(검출된 눈들을 저장)
	CvSeq* rightEye = 0;
	CvRect* tempLEye = 0;
	CvRect* tempREye = 0;

	//왼쪽눈 검출 (원 영상 이미지, cascade, storage, scale_factor(영상의 크기변환),min_neighber(설정값)(CV_HAAR_FIND_BIGGEST_OBJECT) , flag, min_size(최소 검출사이즈), CV_HAAR_FIND_BIGGEST_OBJECT )
	leftEye = cvHaarDetectObjects(faceImage, cascade_leftEye, storage_leftEye, 1.1, 15, CV_HAAR_SCALE_IMAGE , cvSize(30, 30), cvSize(70, 70));
	if (!leftEye->total)
	{
		cout << "왼쪽 눈 검출 없음\n";
		return false;
	}
	else
	{															//검출된 첫번째 왼쪽 눈을 저장
		tempLEye = (CvRect*)cvGetSeqElem(leftEye, 0);
		leftEyeRect->x = tempLEye->x;
		leftEyeRect->y = tempLEye->y;
		leftEyeRect->height = tempLEye->height;
		leftEyeRect->width = tempLEye->width;
	}
	//오른쪽 눈 검출
	rightEye = cvHaarDetectObjects(faceImage, cascade_rightEye, storage_rightEye, 1.1, 5, CV_HAAR_SCALE_IMAGE, cvSize(30, 30), cvSize(70, 70));
	if (!rightEye->total)
	{
		cout << "오른쪽 눈 검출 없음\n";
		return false;
	}
	else																					//검출된 첫번째 오른쪽 눈을 저장
	{
		tempREye = (CvRect*)cvGetSeqElem(rightEye, 0);
		rightEyeRect->x = tempREye->x;
		rightEyeRect->y = tempREye->y;
		rightEyeRect->height = tempREye->height;
		rightEyeRect->width = tempREye->width;
	}

	//검출된 왼쪽 눈들의 좌표를 이용하여 정확한 왼쪽 눈의 좌표를 구할 수 있도록 조건을 설정
	for (int i = 1; i<leftEye->total; i++)
	{
		tempLEye = (CvRect*)cvGetSeqElem(leftEye, i);
		if (((leftEyeRect->x>faceImage->width / 2) || (leftEyeRect->y>(faceImage->height * 1 / 2))) || (leftEyeRect->x + leftEyeRect->width>faceImage->width * 3 / 5 || leftEyeRect->y + leftEyeRect->height>faceImage->height * 3 / 5))																//왼쪽눈 안에 오른쪽이 있는 경우
		{
			leftEyeRect->x = tempLEye->x;
			leftEyeRect->y = tempLEye->y;
			leftEyeRect->height = tempLEye->height;
			leftEyeRect->width = tempLEye->width;
		}
	}

	//검출된 오른쪽 눈들의 좌표를 이용하여 정확한 오른쪽 눈의 좌표를 구할 수 있도록 조건을 설정
	for (int i = 1; i<rightEye->total; i++)																		//우측눈이 왼쪽 눈의 넓이안에 있거나 눈 크기의 1/3만큼 위아래로 벗어나 있는 경우가 아닌경우 바꾼다.
	{																											//우측눈 정확도가 너무 않좋아서 사용
		tempREye = (CvRect*)cvGetSeqElem(rightEye, i);
		if ((leftEyeRect->x + leftEyeRect->width>rightEyeRect->x) || (leftEyeRect->y - (leftEyeRect->height / 3) >= rightEyeRect->y) || (leftEyeRect->y + leftEyeRect->height / 3 <= rightEyeRect->y))
		{
			if ((((leftEyeRect->x<faceImage->width / 2) && (tempREye->x>faceImage->width / 2))) && ((leftEyeRect->y - (leftEyeRect->height / 3 + 5) <= tempREye->y) && ((leftEyeRect->y + leftEyeRect->height / 3 + 5) >= tempREye->y)))		//왼쪽눈이 왼편에 있을시
			{
				rightEyeRect->x = tempREye->x;
				rightEyeRect->y = tempREye->y;
				rightEyeRect->height = tempREye->height;
				rightEyeRect->width = tempREye->width;
			}
			else if (((leftEyeRect->x>faceImage->width / 2) && (rightEyeRect->x>faceImage->width / 2)) && ((leftEyeRect->y - (leftEyeRect->height / 3 + 5) <= tempREye->y) && ((leftEyeRect->y + leftEyeRect->height / 3 + 5) >= tempREye->y)))		//왼쪽눈이 오른쪽에 있을시 우측눈도 오른쪽에 있을때
			{
				rightEyeRect->x = tempREye->x;
				rightEyeRect->y = tempREye->y;
				rightEyeRect->height = tempREye->height;
				rightEyeRect->width = tempREye->width;
			}
		}
	}

	for (int i = 1; i<leftEye->total; i++)
	{
		tempLEye = (CvRect*)cvGetSeqElem(leftEye, i);
		if ((leftEyeRect->x <= tempLEye->x&&leftEyeRect->x + leftEyeRect->width >= tempLEye->x + tempLEye->width) && ((leftEyeRect->y <= tempLEye->y&&leftEyeRect->y + leftEyeRect->height * 2 / 5 >= tempLEye->y) && leftEyeRect->y + leftEyeRect->height >= tempLEye->y + tempLEye->height))						//더 조그마한 눈 검출시
		{
			leftEyeRect->x = tempLEye->x;
			leftEyeRect->y = tempLEye->y;
			leftEyeRect->height = tempLEye->height;
			leftEyeRect->width = tempLEye->width;
		}
	}


	if (leftEyeRect->x>rightEyeRect->x)																		//왼쪽눈과 오른쪽 눈이 바뀐경우
	{
		leftEyeRect->x = rightEyeRect->x;
		leftEyeRect->y = rightEyeRect->y;
		leftEyeRect->height = rightEyeRect->height;
		leftEyeRect->width = rightEyeRect->width;

		rightEyeRect->x = tempLEye->x;
		rightEyeRect->y = tempLEye->y;
		rightEyeRect->height = tempLEye->height;
		rightEyeRect->width = tempLEye->width;
	}


	for (int i = 1; i<rightEye->total; i++)
	{
		tempREye = (CvRect*)cvGetSeqElem(rightEye, i);
		if ((rightEyeRect->x <= tempREye->x && rightEyeRect->x + rightEyeRect->width >= tempREye->x + tempREye->width) && ((rightEyeRect->y <= tempREye->y && rightEyeRect->y + rightEyeRect->height * 2 / 5 >= tempREye->y) && rightEyeRect->y + rightEyeRect->height >= tempREye->y + tempREye->height))						//더 조그마한 눈 검출시
		{
			rightEyeRect->x = tempREye->x;
			rightEyeRect->y = tempREye->y;
			rightEyeRect->height = tempREye->height;
			rightEyeRect->width = tempREye->width;
		}
	}
	return true;
}