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

// Okno dlia otobrazheniia kartinki
//    cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
//    cvNamedWindow("binary",CV_WINDOW_AUTOSIZE);
//    cvNamedWindow("contours",CV_WINDOW_AUTOSIZE);


// show kartinky
// cvShowImage("original",image);

    // Delaem preobrazovania kartinki:
    // sozdaem iadro
    IplConvKernel* Kern = cvCreateStructuringElementEx(radius*2+1, radius*2+1, radius, radius, CV_SHAPE_ELLIPSE);
    // kartinka dlia promezhyto4nogo xranenia rezyl'tatov cvCreateImage
    IplImage* Temp = 0;
    Temp = cvCreateImage(cvSize(image->width, image->height) , IPL_DEPTH_8U, 1);
        
    cvMorphologyEx(image, src, Temp, Kern, CV_MOP_OPEN, iterations);
    cvMorphologyEx(image, src, Temp, Kern, CV_MOP_GRADIENT, iterations);
	
    // sozdaem odnokanal'nie kartinki
    gray = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
    bin = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
    // kloniryem
    dst = cvCloneImage(src);

    // preobrazyem v gradacii serogo
    cvCvtColor(src, gray, CV_RGB2GRAY);

    // preobrazyem v dvoi4noe
    cvInRangeS(gray, cvScalar(40), cvScalar(150), bin);

    // naxodim kontyri
    int contoursCont = cvFindContours( bin, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE ,cvPoint(0,0));
		
    // risyem kontyri i opredelzem ti4ki
    for(CvSeq* seq0 = contours; seq0 != 0; seq0 = seq0->h_next)
	{
		cvDrawContours(dst, seq0, CV_RGB(255,216,0), CV_RGB(0,0,250), 0, 1, 8);
		
		CvPoint* p = (CvPoint*)cvGetSeqElem ( seq0, 0 );
		cout << "Verxniaa granica ygla : ";
		cout << "(" <<  p->x << " ; " <<  p->y << ")" << endl;

		p = (CvPoint*)cvGetSeqElem ( seq0, image->height );
		cout << "Nizhniaa granica ygla : ";
		cout << "(" <<  p->x << " ; " <<  p->y << ")" << endl;
	}	 

//  cvShowImage("binary", bin);
//  cvShowImage("contours", dst);
	
    system("pause");

    // osvoboghdaem resyrsi
    cvReleaseImage(&image);
    cvReleaseImage(&src);
    cvReleaseStructuringElement(&Kern);
    cvReleaseImage(&Temp);
    cvReleaseImage(&gray);
    cvReleaseImage(&bin);
    cvReleaseImage(&dst);

    // ydaliaem okna
    cvDestroyAllWindows();
    
	return 0;
}
