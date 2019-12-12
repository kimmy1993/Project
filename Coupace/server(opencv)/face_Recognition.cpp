#include "face_Recognition.h"

using namespace std;

void face_Recognition::faceses(int num)
{
	this->num_Faces = num;
}
//GALLERY �̹������� PCA �м��� ���Ͽ� Ư¡����
void face_Recognition::training()
{
	char path[100];
	// �޸� �Ҵ�
	eigen_Value = (float *)malloc(sizeof(float)*num_Faces);
	coeffs = (float **)malloc(sizeof(float*)*num_Faces);

	//�������� ���
	for (int i = 0; i<num_Faces; i++)
	{
		coeffs[i] = (float*)malloc(sizeof(float)*PRE_HEIGHT*PRE_WIDTH);
	}

	//�޸� �Ҵ�
	//��ó���� �н��̹��� ����
	train_img = (IplImage**)cvAlloc(sizeof(IplImage*)*num_Faces);

	//������ ����
	eigen_img = (IplImage**)cvAlloc(sizeof(IplImage*)*num_Faces);

	avg = cvCreateImage(cvSize(PRE_WIDTH, PRE_HEIGHT), IPL_DEPTH_32F, 1);			//��������
	proj = cvCreateImage(cvSize(PRE_WIDTH, PRE_HEIGHT), IPL_DEPTH_8U, 1);			//��տ���
	criteria = cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, num_Faces, NULL);	//�ݺ������� �����ϴ� �Լ��� �������� �ʱ�ȭ cvTermCriteria(int type, int max_iter, double epsilon(���� ū �������� ���� ������ �������� ���� ����))
																					//CV_TERMCRIT_ITER(�ִ� �ݺ� Ƚ��)+CV_TERMCRIT_EPS(�������� �Ӱ谪)�� ������ڴ� ��
	for (int i = 0; i<num_Faces; i++)
	{
		memset(path, '\0', 100);
		sprintf_s(path, "./FERET/PROB/_%d.png", i + 1);

		train_img[i] = cvLoadImage(path, 0);
		eigen_img[i] = cvCreateImage(cvGetSize(train_img[i]), IPL_DEPTH_32F, 1);
	}

	//������ ���
	cvCalcEigenObjects(num_Faces, train_img, eigen_img, 0, 0, 0, &criteria, avg, eigen_Value);		//�Է¿���鿡 ���� �����󱼰� ��տ����� ����Ѵ�.

																									//cvCalcEigenObjects(int nObjects(����), void* input, void* output(������), int ioFlags, int ioBufSize(����ũ��), void* userData(�ݹ��Լ��û��), CvTermCriteria* calcLimit(�������� ������ ������ ������ �Ӱ� ��), IPlImage* avg(��տ���), float* eigVals(������))
	//������ ����
	for (int i = 0; i<num_Faces; i++)
	{
		cvEigenDecomposite(train_img[i], num_Faces, eigen_img, 0, 0, avg, coeffs[i]);				//�Է¿����� ������ ������ ������������ ���ذ���� ����ϴ� ��
																									//cvEigenDecomposite( IPlImage* obj(�Է¿���), int nEigObjs, void* eigInput(������), int ioFlags, void* userData, IplImage* avg(��տ���), float* coeffs(���ذ��))
	}
}

bool face_Recognition::recognition(IplImage* image, int *matchId)
{
	long double dis[3] = { DBL_MAX - 1,DBL_MAX - 1,DBL_MAX - 1 };
	double tmpDis = 0;
	//int matchId[3] = { 0};

	//�������� ����
	float* inVec = (float*)malloc(sizeof(float)*PRE_HEIGHT*PRE_WIDTH);
	cvEigenDecomposite(image, num_Faces, eigen_img, 0, 0, avg, inVec);

	//���絵 ���
	
	if (!(coeffs == NULL || inVec == NULL))
	{
		dis[2] = distances(coeffs[0], inVec);
		matchId[2] = 1;
		for (int i = 1; i<num_Faces; i++)
		{
			tmpDis = distances(coeffs[i], inVec);				
			//���� ���絵�� ���� �̹��� id�� ����
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
			cout << ("�� ����ó��\n");
			return false;
		}
		else
		{
			//��ü �� ǥ�� �ʷϻ�
			//cvRectangle(image, cvPoint(faceRect.x, faceRect.y), cvPoint(faceRect.x + faceRect.width, faceRect.y + faceRect.height), cvScalar(0, 255, 0), 1, CV_AA, 0);

			//�� ���� ���캸�� ���� Ȯ��
			cvSetImageROI(image, faceRect);
		}
	}
	cvResize(image, faces, CV_INTER_LINEAR);//�Է¿���, �������, ����
	preprocess.faceHist(faces);
	if (!detection.eye(faces, &leftEyeRect, &rightEyeRect))
	{
		cout << ("�� ����ó��\n");
		return false;
	}
	else
	{
		//������ ���� ���� ������ �簢�� �������� ǥ��
		//cvRectangle(faces, cvPoint(leftEyeRect.x, leftEyeRect.y), cvPoint(leftEyeRect.x + leftEyeRect.width, leftEyeRect.y + leftEyeRect.height), cvScalar(0, 0, 255), 1, CV_AA, 0);

		//������ ������ ���� �Ķ��� �簢�� �������� ǥ��
		//cvRectangle(faces, cvPoint(rightEyeRect.x, rightEyeRect.y), cvPoint(rightEyeRect.x + rightEyeRect.width, rightEyeRect.y + rightEyeRect.height), cvScalar(255, 0, 0), 1, CV_AA, 0);
	}


	if (!(preprocess.preprocessing(faces, preImage, ovalImage, &leftEyeRect, &rightEyeRect)))
	{
		cout << "��ó�� ����";
		return false;
	}

	cvResetImageROI(image);//Ȯ���ߴ� ȭ�� ���

	cvReleaseImage(&faces);
	cvReleaseImage(&ovalImage);

	return preImage;
}
void face_Recognition::setnum(int num)
{
	num_Faces = num;
}