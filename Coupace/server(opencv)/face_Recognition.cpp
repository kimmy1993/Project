#include "face_Recognition.h"

using namespace std;

void face_Recognition::faceses(int num)
{
	this->num_Faces = num;
}
//GALLERY 이미지들을 PCA 분석을 통하여 특징추출
void face_Recognition::training()
{
	char path[100];
	// 메모리 할당
	eigen_Value = (float *)malloc(sizeof(float)*num_Faces);
	coeffs = (float **)malloc(sizeof(float*)*num_Faces);

	//고유벡터 계산
	for (int i = 0; i<num_Faces; i++)
	{
		coeffs[i] = (float*)malloc(sizeof(float)*PRE_HEIGHT*PRE_WIDTH);
	}

	//메모리 할당
	//전처리된 학습이미지 저장
	train_img = (IplImage**)cvAlloc(sizeof(IplImage*)*num_Faces);

	//고유얼굴 저장
	eigen_img = (IplImage**)cvAlloc(sizeof(IplImage*)*num_Faces);

	avg = cvCreateImage(cvSize(PRE_WIDTH, PRE_HEIGHT), IPL_DEPTH_32F, 1);			//고유영상
	proj = cvCreateImage(cvSize(PRE_WIDTH, PRE_HEIGHT), IPL_DEPTH_8U, 1);			//평균영상
	criteria = cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, num_Faces, NULL);	//반복적으로 동작하는 함수의 종료조건 초기화 cvTermCriteria(int type, int max_iter, double epsilon(가장 큰 고유값에 대해 현재의 고유값의 대한 비율))
																					//CV_TERMCRIT_ITER(최대 반복 횟수)+CV_TERMCRIT_EPS(고유값의 임계값)을 고려하자는 뜻
	for (int i = 0; i<num_Faces; i++)
	{
		memset(path, '\0', 100);
		sprintf_s(path, "./FERET/PROB/_%d.png", i + 1);

		train_img[i] = cvLoadImage(path, 0);
		eigen_img[i] = cvCreateImage(cvGetSize(train_img[i]), IPL_DEPTH_32F, 1);
	}

	//고유값 계산
	cvCalcEigenObjects(num_Faces, train_img, eigen_img, 0, 0, 0, &criteria, avg, eigen_Value);		//입력영상들에 대한 고유얼굴과 평균영상을 계산한다.

																									//cvCalcEigenObjects(int nObjects(갯수), void* input, void* output(고유얼굴), int ioFlags, int ioBufSize(버퍼크기), void* userData(콜백함수시사용), CvTermCriteria* calcLimit(종료조건 고유얼굴 갯수와 비율에 임계 값), IPlImage* avg(평균영상), float* eigVals(고유값))
	//고유값 분해
	for (int i = 0; i<num_Faces; i++)
	{
		cvEigenDecomposite(train_img[i], num_Faces, eigen_img, 0, 0, avg, coeffs[i]);				//입력영상을 고유얼굴 공간에 투영시켰을때 분해계수를 계산하는 것
																									//cvEigenDecomposite( IPlImage* obj(입력영상), int nEigObjs, void* eigInput(고유얼굴), int ioFlags, void* userData, IplImage* avg(평균영상), float* coeffs(분해계수))
	}
}

bool face_Recognition::recognition(IplImage* image, int *matchId)
{
	long double dis[3] = { DBL_MAX - 1,DBL_MAX - 1,DBL_MAX - 1 };
	double tmpDis = 0;
	//int matchId[3] = { 0};

	//교유벡터 저장
	float* inVec = (float*)malloc(sizeof(float)*PRE_HEIGHT*PRE_WIDTH);
	cvEigenDecomposite(image, num_Faces, eigen_img, 0, 0, avg, inVec);

	//유사도 계산
	
	if (!(coeffs == NULL || inVec == NULL))
	{
		dis[2] = distances(coeffs[0], inVec);
		matchId[2] = 1;
		for (int i = 1; i<num_Faces; i++)
		{
			tmpDis = distances(coeffs[i], inVec);				
			//가장 유사도가 높은 이미지 id를 리턴
			//cout << tmpDis << endl<<dis[0]<<endl;
			if (tmpDis < dis[0])
			{
				if (tmpDis < dis[1])
				{
					if (tmpDis < dis[2])
					{
						dis[0] = dis[1];
						matchId[0] = matchId[1];
						dis[1] = dis[2];
						matchId[1] = matchId[2];
						dis[2] = tmpDis;
						matchId[2] = i + 1;
					}
					else
					{
						dis[0] = dis[1];
						matchId[0] = matchId[1];
						dis[1] = tmpDis;
						matchId[1] = i + 1;
					}
				}
				else
				{
					dis[0] = tmpDis;
					matchId[0] = i + 1;
				}
			}
		}
	}

	cout << matchId[2] << endl;
	cout << matchId[1] << endl;
	cout << matchId[0] << endl<<endl;
	return true;
}
long double face_Recognition::distances(float* trainVec, float* inVec)
{
	long double dis = 0.0;

	float num;
	/*
	for (int j = 15; j < 30; j++)
	{
		for (int i = 5; i < 20; i++)
		{
			cout << i << " = " << trainVec[i + 100 * j] << endl;
			dis += pow(inVec[i + 100 * j] - trainVec[i + 100 * j], 2);
		}
		for (int i = 75; i < 90; i++)
		{
			cout << i << " = " << trainVec[i + 100 * j] << endl;
			dis += pow(inVec[i + 100 * j] - trainVec[i + 100 * j], 2);
		}
	}

	for (int j = 60; j < 80; j++)
	{
		for (int i = 45; i < 55; i++)
		{
			cout << i << " = " << trainVec[i + 100 * j] << endl;
			dis += pow(inVec[i + 100 * j] - trainVec[i + 100 * j], 2);
		}
	}
	*/

	for (int j =0; j < 300; j += 100)
	{
		for (int i = 3; i < 97; i++)
		{
			if ((trainVec[i+j] <= FLT_MIN+1))
			{
				trainVec[i+j] = FLT_MIN+1;
			}
			else if ((trainVec[i+j] >= FLT_MAX-1))
			{
				trainVec[i+j] = FLT_MAX-1;
			}
			else if (_isnanf(trainVec[i+j]))
			{
				trainVec[i+j] = 0;
				//return dis;
			}
			if ((inVec[i+j] <= FLT_MIN+1))
			{
				inVec[i+j] = FLT_MIN+1;
			}
			else if (inVec[i+j] >= FLT_MAX-1)
			{
				inVec[i+j] = FLT_MAX-1;
			}
			else if (_isnanf(inVec[i+j]))
			{
				inVec[i + j] = FLT_MAX - 1;
				//return dis;
			}

			num = inVec[i+j] - trainVec[i+j];
			dis += (double)pow(num, 2);
			//cout << i << " = " << trainVec[i] << "----"<<inVec[i]<<endl;

			if (dis >= DBL_MAX-1)
			{
				dis = DBL_MAX-1;
			}
			else if (dis <= DBL_MIN+1)
			{
				dis = DBL_MIN+1;
			}
			else if (_isnan(dis))
			{
				dis = 0;
			}

		}
	}
	return dis;
}

IplImage* face_Recognition::face_preprocess(IplImage* image, int set)
{
	Detection detection;
	preprocess preprocess;
	CvRect faceRect = { 0,0,0,0 }, leftEyeRect = { 0,0,0,0 }, rightEyeRect = { 0,0,0,0 };

	IplImage* preImage = cvCreateImage(cvSize(240, 320), image->depth, image->nChannels);
	IplImage* ovalImage = cvCreateImage(cvSize(240, 320), image->depth, image->nChannels);
	IplImage* faces = cvCreateImage(cvSize(240, 320), image->depth, image->nChannels);

	if (set != 1)
	{
		if (!detection.face(image, &faceRect))
		{
			cout << ("얼굴 예외처리\n");
			return false;
		}
		else
		{
			//전체 얼굴 표기 초록색
			//cvRectangle(image, cvPoint(faceRect.x, faceRect.y), cvPoint(faceRect.x + faceRect.width, faceRect.y + faceRect.height), cvScalar(0, 255, 0), 1, CV_AA, 0);

			//얼굴 영역 살펴보기 위한 확대
			cvSetImageROI(image, faceRect);
		}
	}
	cvResize(image, faces, CV_INTER_LINEAR);//입력영상, 결과영상, 필터
	preprocess.faceHist(faces);
	if (!detection.eye(faces, &leftEyeRect, &rightEyeRect))
	{
		cout << ("눈 예외처리\n");
		return false;
	}
	else
	{
		//검출한 왼쪽 눈을 빨간색 사각형 영역으로 표현
		//cvRectangle(faces, cvPoint(leftEyeRect.x, leftEyeRect.y), cvPoint(leftEyeRect.x + leftEyeRect.width, leftEyeRect.y + leftEyeRect.height), cvScalar(0, 0, 255), 1, CV_AA, 0);

		//검출한 오른쪽 눈을 파란색 사각형 영역으로 표현
		//cvRectangle(faces, cvPoint(rightEyeRect.x, rightEyeRect.y), cvPoint(rightEyeRect.x + rightEyeRect.width, rightEyeRect.y + rightEyeRect.height), cvScalar(255, 0, 0), 1, CV_AA, 0);
	}


	if (!(preprocess.preprocessing(faces, preImage, ovalImage, &leftEyeRect, &rightEyeRect)))
	{
		cout << "전처리 에러";
		return false;
	}

	cvResetImageROI(image);//확대했던 화면 축소

	cvReleaseImage(&faces);
	cvReleaseImage(&ovalImage);

	return preImage;
}
void face_Recognition::setnum(int num)
{
	num_Faces = num;
}