#include "Detection.h"

using namespace std;

Detection::Detection()
{
	//ȯ�濡 �°� ��μ���
	this->faceClassifer = "./cascade/haarcascade_frontalface_default.xml";
	this->face2Classifer = "./cascade/lbpcascade_frontalface.xml";
	this->leftEyeClassifer = "./cascade/haarcascade_mcs_lefteye.xml";
	this->rightEyeClassifer = "./cascade/haarcascade_mcs_righteye.xml";
}

bool Detection::face(IplImage* image, CvRect* faceRect)													//�� ����
{
	vector<cv::Rect> rect;
	//�з��� �ε�
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

	//�޸� �Ҵ�
	CvMemStorage* storage_face = 0;

	if (!(storage_face = cvCreateMemStorage(0)))
	{
		cvReleaseHaarClassifierCascade(&cascade_face);
		cout << ("error : storage error \n");
		return false;
	}
	cv::Mat src = cv::Mat(image, true);
	//�� ����(lbp->harr)
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

	//�޸� ����
	cvReleaseHaarClassifierCascade(&cascade_face);
	cvReleaseMemStorage(&storage_face);

	return true;
}

bool Detection::faceDetection(IplImage* image, CvRect* faceRect, CvHaarClassifierCascade* cascade_face, CvMemStorage* storage_face)
{
	CvSeq* faces = 0;																						//���� ����ü ����Ʈ ���⼭�� rect�� �����ϱ� ���� ���(����� �󱼵��� ����)
	CvRect* tempR = 0;																						//����� ���� x,y��ǥ�� ���� ������ ���� ����ü

																											//�󱼰��� (����� �󱼵� �� ���� ū ��ü ��ȯ)
	faces = cvHaarDetectObjects(image, cascade_face, storage_face, 1.1, 5, CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH, cvSize(50, 50));

	//����� ���� ���� 0�̸�
	if (faces->total == 0)
	{
		return false;
	}

	//����� ���� x,y��ǥ height,width ����
	tempR = (CvRect*)cvGetSeqElem(faces, 0);
	faceRect->x = tempR->x;
	faceRect->y = tempR->y;
	faceRect->height = tempR->height + 5;
	faceRect->width = tempR->width;

	return true;
}

bool Detection::eye(IplImage* faceImage, CvRect* leftEyeRect, CvRect* rightEyeRect)
{
	//�з��� �ε�
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

	//�޸� �Ҵ�
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

	//�޸� ����
	cvReleaseHaarClassifierCascade(&cascade_leftEye);
	cvReleaseHaarClassifierCascade(&cascade_rightEye);
	cvReleaseMemStorage(&storage_leftEye);
	cvReleaseMemStorage(&storage_rightEye);

	return true;
}

bool Detection::eyeDetection(IplImage* faceImage, CvRect* leftEyeRect, CvRect* rightEyeRect, CvHaarClassifierCascade* cascade_leftEye, CvMemStorage* storage_leftEye, CvHaarClassifierCascade* cascade_rightEye, CvMemStorage* storage_rightEye)
{
	CvSeq* leftEye = 0;																	//���� ����ü ����Ʈ ���⼭�� rect�� �����ϱ� ���� ���(����� ������ ����)
	CvSeq* rightEye = 0;
	CvRect* tempLEye = 0;
	CvRect* tempREye = 0;

	//���ʴ� ���� (�� ���� �̹���, cascade, storage, scale_factor(������ ũ�⺯ȯ),min_neighber(������)(CV_HAAR_FIND_BIGGEST_OBJECT) , flag, min_size(�ּ� ���������), CV_HAAR_FIND_BIGGEST_OBJECT )
	leftEye = cvHaarDetectObjects(faceImage, cascade_leftEye, storage_leftEye, 1.1, 15, CV_HAAR_SCALE_IMAGE , cvSize(30, 30), cvSize(70, 70));
	if (!leftEye->total)
	{
		cout << "���� �� ���� ����\n";
		return false;
	}
	else
	{															//����� ù��° ���� ���� ����
		tempLEye = (CvRect*)cvGetSeqElem(leftEye, 0);
		leftEyeRect->x = tempLEye->x;
		leftEyeRect->y = tempLEye->y;
		leftEyeRect->height = tempLEye->height;
		leftEyeRect->width = tempLEye->width;
	}
	//������ �� ����
	rightEye = cvHaarDetectObjects(faceImage, cascade_rightEye, storage_rightEye, 1.1, 5, CV_HAAR_SCALE_IMAGE, cvSize(30, 30), cvSize(70, 70));
	if (!rightEye->total)
	{
		cout << "������ �� ���� ����\n";
		return false;
	}
	else																					//����� ù��° ������ ���� ����
	{
		tempREye = (CvRect*)cvGetSeqElem(rightEye, 0);
		rightEyeRect->x = tempREye->x;
		rightEyeRect->y = tempREye->y;
		rightEyeRect->height = tempREye->height;
		rightEyeRect->width = tempREye->width;
	}

	//����� ���� ������ ��ǥ�� �̿��Ͽ� ��Ȯ�� ���� ���� ��ǥ�� ���� �� �ֵ��� ������ ����
	for (int i = 1; i<leftEye->total; i++)
	{
		tempLEye = (CvRect*)cvGetSeqElem(leftEye, i);
		if (((leftEyeRect->x>faceImage->width / 2) || (leftEyeRect->y>(faceImage->height * 1 / 2))) || (leftEyeRect->x + leftEyeRect->width>faceImage->width * 3 / 5 || leftEyeRect->y + leftEyeRect->height>faceImage->height * 3 / 5))																//���ʴ� �ȿ� �������� �ִ� ���
		{
			leftEyeRect->x = tempLEye->x;
			leftEyeRect->y = tempLEye->y;
			leftEyeRect->height = tempLEye->height;
			leftEyeRect->width = tempLEye->width;
		}
	}

	//����� ������ ������ ��ǥ�� �̿��Ͽ� ��Ȯ�� ������ ���� ��ǥ�� ���� �� �ֵ��� ������ ����
	for (int i = 1; i<rightEye->total; i++)																		//�������� ���� ���� ���̾ȿ� �ְų� �� ũ���� 1/3��ŭ ���Ʒ��� ��� �ִ� ��찡 �ƴѰ�� �ٲ۴�.
	{																											//������ ��Ȯ���� �ʹ� �����Ƽ� ���
		tempREye = (CvRect*)cvGetSeqElem(rightEye, i);
		if ((leftEyeRect->x + leftEyeRect->width>rightEyeRect->x) || (leftEyeRect->y - (leftEyeRect->height / 3) >= rightEyeRect->y) || (leftEyeRect->y + leftEyeRect->height / 3 <= rightEyeRect->y))
		{
			if ((((leftEyeRect->x<faceImage->width / 2) && (tempREye->x>faceImage->width / 2))) && ((leftEyeRect->y - (leftEyeRect->height / 3 + 5) <= tempREye->y) && ((leftEyeRect->y + leftEyeRect->height / 3 + 5) >= tempREye->y)))		//���ʴ��� ���� ������
			{
				rightEyeRect->x = tempREye->x;
				rightEyeRect->y = tempREye->y;
				rightEyeRect->height = tempREye->height;
				rightEyeRect->width = tempREye->width;
			}
			else if (((leftEyeRect->x>faceImage->width / 2) && (rightEyeRect->x>faceImage->width / 2)) && ((leftEyeRect->y - (leftEyeRect->height / 3 + 5) <= tempREye->y) && ((leftEyeRect->y + leftEyeRect->height / 3 + 5) >= tempREye->y)))		//���ʴ��� �����ʿ� ������ �������� �����ʿ� ������
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
		if ((leftEyeRect->x <= tempLEye->x&&leftEyeRect->x + leftEyeRect->width >= tempLEye->x + tempLEye->width) && ((leftEyeRect->y <= tempLEye->y&&leftEyeRect->y + leftEyeRect->height * 2 / 5 >= tempLEye->y) && leftEyeRect->y + leftEyeRect->height >= tempLEye->y + tempLEye->height))						//�� ���׸��� �� �����
		{
			leftEyeRect->x = tempLEye->x;
			leftEyeRect->y = tempLEye->y;
			leftEyeRect->height = tempLEye->height;
			leftEyeRect->width = tempLEye->width;
		}
	}


	if (leftEyeRect->x>rightEyeRect->x)																		//���ʴ��� ������ ���� �ٲ���
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
		if ((rightEyeRect->x <= tempREye->x && rightEyeRect->x + rightEyeRect->width >= tempREye->x + tempREye->width) && ((rightEyeRect->y <= tempREye->y && rightEyeRect->y + rightEyeRect->height * 2 / 5 >= tempREye->y) && rightEyeRect->y + rightEyeRect->height >= tempREye->y + tempREye->height))						//�� ���׸��� �� �����
		{
			rightEyeRect->x = tempREye->x;
			rightEyeRect->y = tempREye->y;
			rightEyeRect->height = tempREye->height;
			rightEyeRect->width = tempREye->width;
		}
	}
	return true;
}