#include "preprocess.h"

using namespace std;
using namespace cv;

bool preprocess::preprocessing(IplImage* image, IplImage* preImage, IplImage* ovalImage, CvRect* leftEyeRect, CvRect* rightEyeRect)			//전처리 함수
{
	CvMat* mat = cvCreateMat(2, 3, CV_32FC1);
	cvSetZero(preImage);

	IplImage* save_image = cvCreateImage(cvSize(PRE_WIDTH, PRE_HEIGHT), image->depth, image->nChannels);

	//얼굴이 틀어진 각도를 계산
	double X = (rightEyeRect->x) - (leftEyeRect->x);
	double Y = (rightEyeRect->y) - (leftEyeRect->y);
	double angle = atan(Y / X);

	//눈의 길이를 일정하게 하기 위해 계산
	double scale = EYE_LEN / X;

	//왼쪽 눈의 위치를 모두 같은 곳으로 고정시키기 위해서 지정된 위치만큼 어마나 평행이동 해야되는지 계산
	int movX = (leftEyeRect->x - SET_EYE_X)*-1;
	int movY = (leftEyeRect->y - SET_EYE_Y)*-1;

	//회전 및 스케일링 
	CvPoint2D32f center = cvPoint2D32f(leftEyeRect->x, leftEyeRect->y);
	cv2DRotationMatrix(center, angle * 180 / PI, scale, mat);
	cvWarpAffine(image, image, mat);

	//평행이동
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
	
	//타원 마스크와 전처리 이미지의 AND연산
	ovalImage = makeOvalImage(image->height, image->width);
	cvAnd(ovalImage, preImage, preImage);

	//타원영역에 해당하는 부분(얼굴영역)을 평활화
	if (!ovalSpaceHistEqual(preImage, ovalImage))
	{
		cout << "평탄화 에러";
		return false;
	}
	
	//전처리된 이미지의 크기를 지정
	CvRect rect;
	rect.x = SET_EYE_X -8;
	rect.y = SET_EYE_Y-17;
	rect.height = PRE_HEIGHT;
	rect.width = PRE_WIDTH;

	cvSetImageROI(preImage, rect);														//이미지를 타원 마스크 부분으로 확대한다.

	cvResize(preImage, save_image, CV_INTER_LINEAR);										//입력영상, 결과영상, 필터
																							//cvResetImageROI(preImage);															//확대했던 화면 축소

	return true;
}

IplImage* preprocess::makeOvalImage(int height, int width)
{

	int centerX = width / 2;
	int centerY = (height / 2) - 15;

	int longAxis = (height*height) / LONGAXIS_RATE;
	int shortAxis = (width*width) / SHORTAXIS_RATE;

	int topOVal = (int)(centerY - (sqrt((double)longAxis)));
	
	//타원이미지를 만들 이미지 생성
	IplImage* ovalImage = cvCreateImage(cvSize(width, height), 8, 1);
	cvSetZero(ovalImage);

	//타원 이미지 생성
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

	//이마부분 제거
	for (int i = topOVal; i<topOVal + SIZE_FOREHEAD; i++) {
		for (int j = 0; j<width; j++) {
			cvSet2D(ovalImage, i, j, cvScalar(0));
		}
	}
	return ovalImage;
}

//타원 내부영역 히스토그램 평활화 함수
bool preprocess::ovalSpaceHistEqual(IplImage* src, IplImage* ovalImage)
{
	double SUM = 0.0, HIGH = 0.0;
	double HIST[HIST_SIZE] = { 0 };
	double sum_hist[HIST_SIZE] = { 0 };

	int ovalSpace = 0;

	//타원 영역에 해당하는 픽셀들의 빈도수 계산
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

	//누적합 계산
	for (int i = 0; i<HIST_SIZE; i++)
	{
		if (HIST[i] != 0)
			HIGH = i;

		SUM += HIST[i];
		sum_hist[i] = SUM;
	}

	//정규화
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
	char path[100];									//저장될 장소를 지정하기 위한 char형 변수

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

	//Contrast Limited Adaptive Histogram Equalization를 사용,8,8사이즈로 히스토그램을 처리, 한계값을 넘어가면 균일하게 배분하여 적용
	Ptr<CLAHE> clahe;
	clahe = createCLAHE(10.0, cvSize(4, 4));

	//왼쪽 확대했던 화면 축소 그 동안 Contrast Limited Adaptive Histogram Equalization 적용
	cvSetImageROI(image, leftRect);

	//cvEqualizeHist(image,leftImage);
	Mat src_left = Mat(image, true);
	clahe->apply(src_left, src_left);
	leftImage = &IplImage(src_left);
	//src_left.release();

	cvResetImageROI(image);

	//오른쪽 확대했던 화면 축소 그 동안 Contrast Limited Adaptive Histogram Equalization 적용
	cvSetImageROI(image, rightRect);

	Mat src_right = Mat(image, true);
	clahe->apply(src_right, src_right);
	rightImage = &IplImage(src_right);
	//src_right.release();

	cvResetImageROI(image);

	//중앙 Contrast Limited Adaptive Histogram Equalization 적용
	Mat src_middle = Mat(image, true);
	clahe->apply(src_middle, src_middle);
	histImage = &IplImage(src_middle);
	//src_middle.release();

	//얼굴의 좌, 중앙, 우로 나눠 히스토그램된 데이터를 합친다(각 가중치를 둬서) 빛의 영향 최소화
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
	//bilateralFilter를 적용해 중간에 생긴 화소 잡음을 제거한다
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