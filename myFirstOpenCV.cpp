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

// окно для отображения картинки
//    cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
//	  cvNamedWindow("binary",CV_WINDOW_AUTOSIZE);
//    cvNamedWindow("contours",CV_WINDOW_AUTOSIZE);


// показываем картинку
//    cvShowImage("original",image);

	// Делаем преобразования изображения:
    // создаём ядро
    IplConvKernel* Kern = cvCreateStructuringElementEx(radius*2+1, radius*2+1, radius, radius, CV_SHAPE_ELLIPSE);
    // картинка для промежуточного хранения результатов cvCreateImage
    IplImage* Temp = 0;
    Temp = cvCreateImage(cvSize(image->width, image->height) , IPL_DEPTH_8U, 1);
        
	cvMorphologyEx(image, src, Temp, Kern, CV_MOP_OPEN, iterations);
	cvMorphologyEx(image, src, Temp, Kern, CV_MOP_GRADIENT, iterations);
	
	// создаём одноканальные картинки
    gray = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
    bin = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
    // клонируем
    dst = cvCloneImage(src);

    // матрицы трансформации
    CvMat* rot_mat = cvCreateMat(2, 3, CV_32FC1);    
    // клонируем изображение
    dst = cvCloneImage(src);

	// поворот изображения
    // рассчёт матрицы вращения
    CvPoint2D32f center = cvPoint2D32f(src->width/2, src->height/2);
    double angle = -1.5;   // на 1,5 градусa по часовой стрелке
    double scale = 1;      // масштаб
    cv2DRotationMatrix(center,angle,scale,rot_mat);

    // выполняем вращение
    cvWarpAffine(src, dst, rot_mat);

	// сохраним результат трансформации
    cvCopy(dst, src);

	// преобразуем в градации серого
	cvCvtColor(src, gray, CV_RGB2GRAY);

	// преобразуем в двоичное
    cvInRangeS(gray, cvScalar(40), cvScalar(150), bin); // atoi(argv[2])

	// находим контуры
    int contoursCont = cvFindContours( bin, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE ,cvPoint(0,0));
		
	// нарисуем контуры
    for(CvSeq* seq0 = contours; seq0 != 0; seq0 = seq0->h_next)
	{
		cvDrawContours(dst, seq0, CV_RGB(255,216,0), CV_RGB(0,0,250), 0, 1, 8); // рисуем контур

		CvPoint* p = (CvPoint*)cvGetSeqElem ( seq0, 0 );
		cout << "Верхняя координата угла : ";
		cout << "(" <<  p->x << " ; " <<  p->y << ")" << endl;

		p = (CvPoint*)cvGetSeqElem ( seq0, image->height );
		cout << "Нижняя координата угла : ";
		cout << "(" <<  p->x << " ; " <<  p->y << ")" << endl;
	}	 

//	cvShowImage("binary", bin);
//	cvShowImage("contours", dst);
	
	system("pause");

    // освобождаем ресурсы
    cvReleaseImage(&image);
    cvReleaseImage(&src);
    cvReleaseStructuringElement(&Kern);
    cvReleaseImage(&Temp);
    cvReleaseImage(&gray);
    cvReleaseImage(&bin);
    cvReleaseImage(&dst);
	cvReleaseMat(&rot_mat);

    // удаляем окна
    cvDestroyAllWindows();
    
	return 0;
}