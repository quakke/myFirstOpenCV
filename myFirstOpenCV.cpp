#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

IplImage* image = 0;
IplImage* src = 0;
IplImage* gray = 0;
IplImage* bin = 0;
IplImage* dst = 0;

int radius = 1;
int iterations = 8;


int main()
{		
	setlocale(LC_ALL, "Russian");
	IplImage* image = cvLoadImage("D:\\ugol.jpg", 1);

	CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours=0;

	src = cvCloneImage(image);

// ���� ��� ����������� ��������
//    cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
//	  cvNamedWindow("binary",CV_WINDOW_AUTOSIZE);
//    cvNamedWindow("contours",CV_WINDOW_AUTOSIZE);


// ���������� ��������
//    cvShowImage("original",image);

	// ������ �������������� �����������:
    // ������ ����
    IplConvKernel* Kern = cvCreateStructuringElementEx(radius*2+1, radius*2+1, radius, radius, CV_SHAPE_ELLIPSE);
    // �������� ��� �������������� �������� ����������� cvCreateImage
    IplImage* Temp = 0;
    Temp = cvCreateImage(cvSize(image->width, image->height) , IPL_DEPTH_8U, 1);
        
	cvMorphologyEx(image, src, Temp, Kern, CV_MOP_OPEN, iterations);
	cvMorphologyEx(image, src, Temp, Kern, CV_MOP_GRADIENT, iterations);
	
	// ������ ������������� ��������
    gray = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
    bin = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
    // ���������
    dst = cvCloneImage(src);

    // ������� �������������
    CvMat* rot_mat = cvCreateMat(2, 3, CV_32FC1);    
    // ��������� �����������
    dst = cvCloneImage(src);

	// ������� �����������
    // ������� ������� ��������
    CvPoint2D32f center = cvPoint2D32f(src->width/2, src->height/2);
    double angle = -1.5;   // �� 1,5 ������a �� ������� �������
    double scale = 1;      // �������
    cv2DRotationMatrix(center,angle,scale,rot_mat);

    // ��������� ��������
    cvWarpAffine(src, dst, rot_mat);

	// �������� ��������� �������������
    cvCopy(dst, src);

	// ����������� � �������� ������
	cvCvtColor(src, gray, CV_RGB2GRAY);

	// ����������� � ��������
    cvInRangeS(gray, cvScalar(40), cvScalar(150), bin); // atoi(argv[2])

	// ������� �������
    int contoursCont = cvFindContours( bin, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE ,cvPoint(0,0));
		
	// �������� �������
    for(CvSeq* seq0 = contours; seq0 != 0; seq0 = seq0->h_next)
	{
		cvDrawContours(dst, seq0, CV_RGB(255,216,0), CV_RGB(0,0,250), 0, 1, 8); // ������ ������

		CvPoint* p = (CvPoint*)cvGetSeqElem ( seq0, 0 );
		cout << "������� ���������� ���� : ";
		cout << "(" <<  p->x << " ; " <<  p->y << ")" << endl;

		p = (CvPoint*)cvGetSeqElem ( seq0, image->height );
		cout << "������ ���������� ���� : ";
		cout << "(" <<  p->x << " ; " <<  p->y << ")" << endl;
	}	 

//	cvShowImage("binary", bin);
//	cvShowImage("contours", dst);
	
	system("pause");

    // ����������� �������
    cvReleaseImage(&image);
    cvReleaseImage(&src);
    cvReleaseStructuringElement(&Kern);
    cvReleaseImage(&Temp);
    cvReleaseImage(&gray);
    cvReleaseImage(&bin);
    cvReleaseImage(&dst);
	cvReleaseMat(&rot_mat);

    // ������� ����
    cvDestroyAllWindows();
    
	return 0;
}