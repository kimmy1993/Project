#include "preprocess.h"

using namespace std;
using namespace cv;

bool preprocess::preprocessing(IplImage* image, IplImage* preImage, IplImage* ovalImage, CvRect* leftEyeRect, CvRect* rightEyeRect)			//��ó�� �Լ�
{
	CvMat* mat = cvCreateMat(2, 3, CV_32FC1);
	cvSetZero(preImage);

	IplImage* save_image = cvCreateImage(cvSize(PRE_WIDTH, PRE_HEIGHT), image->depth, image->nChannels);

	//���� Ʋ���� ������ ���
	double X = (rightEyeRect->x) - (leftEyeRect->x);
	double Y = (rightEyeRect->y) - (leftEyeRect->y);
	double angle = atan(Y / X);

	//���� ���̸� �����ϰ� �ϱ� ���� ���
	double scale = EYE_LEN / X;

	//���� ���� ��ġ�� ��� ���� ������ ������Ű�� ���ؼ� ������ ��ġ��ŭ ��� �����̵� �ؾߵǴ��� ���
	int movX = (leftEyeRect->x - SET_EYE_X)*-1;
	int movY = (leftEyeRect->y - SET_EYE_Y)*-1;

	//ȸ�� �� �����ϸ� 
	CvPoint2D32f center = cvPoint2D32f(leftEyeRect->x, leftEyeRect->y);
	cv2DRotationMatrix(center, angle * 180 / PI, scale, mat);
	cvWarpAffine(image, image, mat);

	//�����̵�
	for (int n = 0; n<image->height; n++)
	{
		for (int m = 0; m<image->width; m++)
		{
			CvScalar pix;
			if ((n - movY >= 0 && n - movY < image->height) && ((m - movX >= 0 && m - movX < image->width)))
			{
				pix = cvGet2D(image, n - movY, m - movX);
				cvSet2D(preImage, n, m, pix);
			}
		}
	}
	
	//Ÿ�� ����ũ�� ��ó�� �̹����� AND����
	ovalImage = makeOvalImage(image->height, image->width);
	cvAnd(ovalImage, preImage, preImage);

	//Ÿ�������� �ش��ϴ� �κ�(�󱼿���)�� ��Ȱȭ
	if (!ovalSpaceHistEqual(preImage, ovalImage))
	{
		cout << "��źȭ ����";
		return false;
	}
	
	//��ó���� �̹����� ũ�⸦ ����
	CvRect rect;
	rect.x = SET_EYE_X -8;
	rect.y = SET_EYE_Y-17;
	rect.height = PRE_HEIGHT;
	rect.width = PRE_WIDTH;

	cvSetImageROI(preImage, rect);														//�̹����� Ÿ�� ����ũ �κ����� Ȯ���Ѵ�.

	cvResize(preImage, save_image, CV_INTER_LINEAR);										//�Է¿���, �������, ����
																							//cvResetImageROI(preImage);															//Ȯ���ߴ� ȭ�� ���

	return true;
}

IplImage* preprocess::makeOvalImage(int height, int width)
{

	int centerX = width / 2;
	int centerY = (height / 2) - 15;

	int longAxis = (height*height) / LONGAXIS_RATE;
	int shortAxis = (width*width) / SHORTAXIS_RATE;

	int topOVal = (int)(centerY - (sqrt((double)longAxis)));
	
	//Ÿ���̹����� ���� �̹��� ����
	IplImage* ovalImage = cvCreateImage(cvSize(width, height), 8, 1);
	cvSetZero(ovalImage);

	//Ÿ�� �̹��� ����
	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			if (((i - centerY)*(i - centerY)*shortAxis) + ((j - centerX)*(j - centerX)*longAxis) <= longAxis*shortAxis)
			{
				cvSet2D(ovalImage, i, j, cvScalar(255));
			}
		}
	}

	//�̸��κ� ����
	for (int i = topOVal; i<topOVal + SIZE_FOREHEAD; i++) {
		for (int j = 0; j<width; j++) {
			cvSet2D(ovalImage, i, j, cvScalar(0));
		}
	}
	return ovalImage;
}

//Ÿ�� ���ο��� ������׷� ��Ȱȭ �Լ�
bool preprocess::ovalSpaceHistEqual(IplImage* src, IplImage* ovalImage)
{
	double SUM = 0.0, HIGH = 0.0;
	double HIST[HIST_SIZE] = { 0 };
	double sum_hist[HIST_SIZE] = { 0 };

	int ovalSpace = 0;

	//Ÿ�� ������ �ش��ϴ� �ȼ����� �󵵼� ���
	for (int i = 0; i<src->height; i++)
	{
		for (int j = 0; j<src->width; j++)
		{
			CvScalar pix;
			pix = cvGet2D(ovalImage, i, j);

			if (pix.val[0] == 255)
			{
				CvScalar temp;
				temp = cvGet2D(src, i, j);
				HIST[(int)temp.val[0]]++;
				ovalSpace++;
			}
		}
	}

	//������ ���
	for (int i = 0; i<HIST_SIZE; i++)
	{
		if (HIST[i] != 0)
			HIGH = i;

		SUM += HIST[i];
		sum_hist[i] = SUM;
	}

	//����ȭ
	for (int i = 0; i<ovalImage->height; i++)
	{
		for (int j = 0; j<ovalImage->width; j++)
		{
			CvScalar pix;
			pix = cvGet2D(ovalImage, i, j);

			if (pix.val[0] == 255)
			{
				CvScalar temp = cvGet2D(src, i, j);
				cvSet2D(src, i, j, cvScalar(sum_hist[(int)temp.val[0]] * HIGH / (ovalSpace)));
			}
		}
	}
	return true;
}

bool preprocess::saveimage(IplImage* image, int num)
{
	char path[100];									//����� ��Ҹ� �����ϱ� ���� char�� ����

	sprintf(path, "./FERET/PROB/_%d.png", num);

	cvSaveImage(path, image);

	return true;
}
bool preprocess::faceHist(IplImage* image)
{
	IplImage* histImage = cvCreateImage(cvSize(240, 320), image->depth, image->nChannels);
	IplImage* leftImage = cvCreateImage(cvSize(120, 320), image->depth, image->nChannels);
	IplImage* rightImage = cvCreateImage(cvSize(120, 320), image->depth, image->nChannels);
	IplImage* creatImage = cvCreateImage(cvSize(240, 320), image->depth, image->nChannels);
	IplImage* filterImage;

	CvRect leftRect = { 0,0,120,320 };
	CvRect rightRect = { 120,0,240,320 };

	//Contrast Limited Adaptive Histogram Equalization�� ���,8,8������� ������׷��� ó��, �Ѱ谪�� �Ѿ�� �����ϰ� ����Ͽ� ����
	Ptr<CLAHE> clahe;
	clahe = createCLAHE(10.0, cvSize(4, 4));

	//���� Ȯ���ߴ� ȭ�� ��� �� ���� Contrast Limited Adaptive Histogram Equalization ����
	cvSetImageROI(image, leftRect);

	//cvEqualizeHist(image,leftImage);
	Mat src_left = Mat(image, true);
	clahe->apply(src_left, src_left);
	leftImage = &IplImage(src_left);
	//src_left.release();

	cvResetImageROI(image);

	//������ Ȯ���ߴ� ȭ�� ��� �� ���� Contrast Limited Adaptive Histogram Equalization ����
	cvSetImageROI(image, rightRect);

	Mat src_right = Mat(image, true);
	clahe->apply(src_right, src_right);
	rightImage = &IplImage(src_right);
	//src_right.release();

	cvResetImageROI(image);

	//�߾� Contrast Limited Adaptive Histogram Equalization ����
	Mat src_middle = Mat(image, true);
	clahe->apply(src_middle, src_middle);
	histImage = &IplImage(src_middle);
	//src_middle.release();

	//���� ��, �߾�, ��� ���� ������׷��� �����͸� ��ģ��(�� ����ġ�� �ּ�) ���� ���� �ּ�ȭ
	for (int y = 0; y<320; y++)
	{
		for (int x = 0; x<240; x++)
		{
			float value = 0;
			if (x<240 / 4)
			{
				int left = leftImage->imageData[y * 120 + x];
				if (left<0)
					left = left + 256;
				value = left;
			}
			else if (x<240 / 2)
			{
				int left = leftImage->imageData[y * 120 + x];
				int middle = histImage->imageData[y * 240 + x];

				float f = (x - 240 * 1 / 4) / (float)(240 / 4);
				if (left<0)
					left = left + 256;
				if (middle<0)
					middle = middle + 256;
				value = cvRound((1.0f - f)*left + (f)*middle);
			}
			else if (x<240 * 3 / 4)
			{
				int right = rightImage->imageData[y * 120 + (x - 120)];
				int middle = histImage->imageData[y * 240 + x];

				float f = (x - 240 * 1 / 2) / (float)(240 / 4);
				if (right<0)
					right = right + 256;
				if (middle<0)
					middle = middle + 256;
				value = cvRound((1.0f - f)*middle + (f)*right);
			}
			else
			{
				int right = rightImage->imageData[y * 120 + (x - 120)];
				if (right<0)
					right = right + 256;
				value = right;
			}
			if (value<0.0)
			{
				value = 0;
			}
			else if (value>255.0)
				value = 255;

			if (value>127)
				value = value - 256;
			creatImage->imageData[y * 240 + x] = value;
		}
	}
	//bilateralFilter�� ������ �߰��� ���� ȭ�� ������ �����Ѵ�
	Mat src = Mat(creatImage, true);
	Mat dst = src.clone();

	bilateralFilter(src, dst, 0, 20.0, 1.0);
	filterImage = &IplImage(dst);

	cvWaitKey();
	cvDestroyAllWindows();

	cvResize(filterImage, image, CV_INTER_LINEAR);
	dst.release();
	src.release();

	//cvReleaseImage(&histImage);
	//cvReleaseImage(&leftImage);
	//cvReleaseImage(&rightImage);
	return true;
}