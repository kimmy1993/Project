#include "Function.h"

using namespace std;

bool Function::Allpreprocessing(int num)
{
	Detection detection;
	preprocess preprocess;

	CvRect faceRect = { 0,0,0,0 }, leftEyeRect = { 0,0,0,0 }, rightEyeRect = { 0,0,0,0 };

	for (int i_num = 1; i_num <= num; i_num++)
	{
		//이미지 선언 오리지날 이미지, 처리된 이미지
		IplImage* image;

		//불러올 위치 선언
		sprintf(path, "./FERET/GALLERY/_%d.png", i_num);

		//이미지 로드//CV_LOAD_IMAGE_GRAYSCALE
		if (!(image = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE)))
		{
			cout << ("imageload error\n");
			exit(0);
		}

		IplImage* preImage = cvCreateImage(cvSize(240, 320), image->depth, image->nChannels);
		IplImage* ovalImage = cvCreateImage(cvSize(240, 320), image->depth, image->nChannels);
		IplImage* faces = cvCreateImage(cvSize(240, 320), 8, image->nChannels);
		IplImage* save_faces = cvCreateImage(cvSize(PRE_WIDTH, PRE_HEIGHT), image->depth, image->nChannels);

		if (!detection.face(image, &faceRect))
		{
			cout << ("얼굴 예외처리\n");
			continue;
		}
		else
		{
			//전체 얼굴 표기 초록색
			//cvRectangle(image, cvPoint(faceRect.x, faceRect.y), cvPoint(faceRect.x+faceRect.width, faceRect.y+faceRect.height), cvScalar(0,255,0), 1, CV_AA, 0);

			//얼굴 영역 살펴보기 위한 확대
			cvSetImageROI(image, faceRect);
		}

		cvResize(image, faces, CV_INTER_LINEAR);//입력영상, 결과영상, 필터
		preprocess.faceHist(faces);

		if (!detection.eye(faces, &leftEyeRect, &rightEyeRect))
		{
			cout << ("눈 예외처리\n");
			continue;
		}
		else
		{
			//검출한 왼쪽 눈을 빨간색 사각형 영역으로 표현
			//cvRectangle(faces, cvPoint(leftEyeRect.x, leftEyeRect.y), cvPoint(leftEyeRect.x+leftEyeRect.width, leftEyeRect.y+leftEyeRect.height), cvScalar(0,0,255),1 , CV_AA, 0);

			//검출한 오른쪽 눈을 파란색 사각형 영역으로 표현
			//cvRectangle(faces, cvPoint(rightEyeRect.x, rightEyeRect.y), cvPoint(rightEyeRect.x+rightEyeRect.width, rightEyeRect.y+rightEyeRect.height), cvScalar(255,0,0), 1, CV_AA, 0);
		}

		if (!(preprocess.preprocessing(faces, preImage, ovalImage, &leftEyeRect, &rightEyeRect)))
		{
			cout << "전처리 에러";
			continue;
		}
		else
			preprocess.saveimage(preImage, i_num);

		cvResetImageROI(image);//확대했던 화면 축소
		/*
		cvShowImage("Result", faces);
		cvShowImage("Detection Result", preImage);
		*/
		cvWaitKey();
		cvDestroyAllWindows();
		cvReleaseImage(&image);
		cvReleaseImage(&faces);
		cvReleaseImage(&preImage);
		cvReleaseImage(&ovalImage);
	}

	return true;
}

bool Function::Recognition(int num, int image_num, int *user, face_Recognition face_Recognition)
{
	user_num user_dlist;
	user_num *p;
	p = &user_dlist;
	int result[3] = { 0 };
	char imagepath[100];
	user_dlist_init(&user_dlist);

	for (int i = 0; image_num > i; i++)
	{
		*result = 0;
		IplImage* recong_image;
		sprintf(imagepath, "./FERET/recong/a%d.png", i + 1);
		recong_image = cvLoadImage(imagepath, CV_LOAD_IMAGE_GRAYSCALE);

		IplImage* save_faces = cvCreateImage(cvSize(PRE_WIDTH, PRE_HEIGHT), recong_image->depth, recong_image->nChannels);
		
		if (!(save_faces = face_Recognition.face_preprocess(recong_image,1)))
		{
			cout << "검출 없음" << endl;
			continue;
		}
		
		if (!(face_Recognition.recognition(save_faces, result)))
		{
			cout << "검사 에러" << endl;
			continue;
		}

		cout << result[2] << endl;
		cout << result[1] << endl;
		cout << result[0] << endl;
		
		for (int n = 0; n < 3; n++)
		{
			p = &user_dlist;
			while (true)
			{
				if (p->id == 0 || p->next == NULL)
				{
					if (this->user_dlist(result[n],n, p) == false)
					{
						printf("list error");
						break;
					}
					break;
				}
				else
				{
					if (p->id == result[n] || p->next == NULL)
					{
						if (this->user_dlist(result[n],n, p) == false)
						{
							printf("list error");
							break;
						}
						break;
					}
				}
				p = p->next;
			}
		}
		/*
		cvShowImage("Detection Result", save_faces);

		cvWaitKey();
		cvDestroyAllWindows();
		*/
		cvReleaseImage(&save_faces);
		cvReleaseImage(&recong_image);
	}
	p = &user_dlist;
	sort(&user_dlist);

	while (true)
	{
		if (p->next != NULL)
		{
			cout << endl << "\n" << p->id << " : " << p->num;
			p = p->next;
		}
		else if (p->next == NULL)
		{
			cout << endl << "\n" << p->id << " : " << p->num;
			break;
		}
	}
	user[0] = user_dlist.id;
	if (user_dlist.next == NULL);
	else
	{
		user[1] = user_dlist.next->id;
		if (user_dlist.next->next == NULL);
		else
			user[2] = user_dlist.next->next->id;
	}
	user_dlist_init(&user_dlist);


	return true;
}
bool Function::Insertimage(int num)
{
	preprocess preprocess;
	face_Recognition face_recognition;
	face_recognition.faceses(num);

	IplImage* recong_image;
	char paths[100] = {0};
	sprintf(paths, "./FERET/GALLERY/_%d.png", num+1);
	recong_image = cvLoadImage(paths, CV_LOAD_IMAGE_GRAYSCALE);

	IplImage* save_faces = cvCreateImage(cvSize(PRE_WIDTH, PRE_HEIGHT), recong_image->depth, recong_image->nChannels);

	save_faces = face_recognition.face_preprocess(recong_image, 2);
	if (save_faces == NULL)
	{
		return false;
	}

	preprocess.saveimage(save_faces, ++num);
	face_recognition.setnum(num);
	
	/*
	cvShowImage("Detection Result", save_faces);
	cvWaitKey();
	cvDestroyAllWindows();
	*/
	
	cvReleaseImage(&save_faces);	
	cvReleaseImage(&recong_image);
	return true;
}
bool Function::user_dlist(int id,int n, user_num *dlist)
{
	if (dlist == NULL)
	{
		user_num *newnode;
		newnode = (user_num *)malloc(sizeof(user_num));
		if (newnode == NULL)
		{
			printf("memory error");
			return false;
		}
		newnode->id = id;
		newnode->num = (n+1);
		newnode->next = NULL;

		*dlist = *newnode;
	}
	else if (dlist->id == 0)
	{
		user_num *newnode;
		newnode = (user_num *)malloc(sizeof(user_num));
		if (newnode == NULL)
		{
			printf("memory error");
			return false;
		}
		newnode->id = id;
		newnode->num = (n+1);
		newnode->next = NULL;

		*dlist = *newnode;
	}
	else
	{
		if (dlist->id == id)
		{
			dlist->num= dlist->num + (n+1);
		}
		else if (dlist->id != id&&dlist->next == NULL)
		{
			user_num *newnode;
			newnode = (user_num *)malloc(sizeof(user_num));
			if (newnode == NULL)
			{
				printf("memory error");
				return false;
			}
			newnode->id = id;
			newnode->num = (n+1);
			newnode->next = NULL;

			dlist->next = newnode;
		}
	}

	return true;
}
bool Function::user_dlist_init(user_num *dlist)
{
	dlist->next = NULL;
	dlist->id = 0;
	dlist->num = 0;

	return true;
}
bool Function::sort(user_num *dlist)
{
	int num = 1;
	user_num *p = dlist;
	user_num buf;

	for (int i = 0; i<num; i++)
	{
		if (p->next != NULL)
		{
			num++;
			p = p->next;
		}
		else
			break;
	}

	for (int i = 0; i<num; i++)
	{
		p = dlist;
		for (int j = 0; j<num; j++)
		{
			if (p->next != NULL)
			{
				if (p->next->num>p->num)
				{
					buf.id = p->id;
					buf.num = p->num;
					p->id = p->next->id;
					p->num = p->next->num;
					p->next->id = buf.id;
					p->next->num = buf.num;
				}
				if (p->next != NULL)
					p = p->next;
				else
					break;
			}
			else
				break;
		}
	}
	return true;
}