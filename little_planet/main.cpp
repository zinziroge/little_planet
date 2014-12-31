/*
http://www.subblue.com/blog/2010/6/17/little_planets
http://mathworld.wolfram.com/StereographicProjection.html
*/

#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>

#include "little_planet.hpp"

/*****************************************************************************/
// Visual Studio 2013
#ifdef _DEBUG
//Debugモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_core2410d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_imgproc2410d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_highgui2410d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_objdetect2410d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_contrib2410d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_features2d2410d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_flann2410d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_gpu2410d.lib")
//#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_haartraining_engined.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_legacy2410d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_ts2410d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_video2410d.lib")
#else
//Releaseモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_core2410.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_imgproc2410.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_highgui2410.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_objdetect2410.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_contrib2410.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_features2d2410.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_flann2410.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_gpu2410.lib")
//#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_haartraining_engined.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_legacy2410.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_ts2410.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_video2410.lib")
#endif

/*****************************************************************************/
IplImage *img1;
IplImage *img2;
int sw_img = 0;
double g_lat = -90;	// 緯度[deg]
double g_lng = 0;	// 経度[deg]
double g_zoom = 0.1;

/*****************************************************************************/
void on_mouse(int event, int x, int y, int flags, void *param = NULL);

/*****************************************************************************/
void on_mouse(int event, int x, int y, int flags, void *param) {
	static int line = 0;
	const int max_line = 15, w = 15, h = 30;

	// (4)マウスイベントを取得
	switch (event) {
	case CV_EVENT_MBUTTONDOWN:
		g_lng = (x - img1->width/2) / (double)(img1->width/2) * 180;
		printf("MDOWN:lat=%lf,lng=%lf,zoom=%lf\n", g_lat, g_lng, g_zoom);
		cv_LittlePlanet(img1, img2, g_lat / 180.*M_PI, g_lng / 180.*M_PI, g_zoom);
		cvShowImage("Little Planet", img2);
		break;
	case CV_EVENT_LBUTTONDOWN:
		g_lat = g_lat + 10;
		//g_zoom += 0.1;
		//g_lng = g_lng + 10;
		printf("LDOWN:lat=%lf,lng=%lf,zoom=%lf\n", g_lat, g_lng, g_zoom);
		cv_LittlePlanet(img1, img2, g_lat / 180.*M_PI, g_lng / 180.*M_PI, g_zoom);
		cvShowImage("Little Planet", img2);
		break;
	case CV_EVENT_RBUTTONDOWN:
		g_lat = g_lat - 10;
		//g_zoom -= 0.1;
		//g_lng = g_lng - 10;
		if (g_zoom < 0) g_zoom = 0.1;
		printf("RDOWN:lat=%lf,lng=%lf,zoom=%lf\n", g_lat, g_lng, g_zoom);
		cv_LittlePlanet(img1, img2, g_lat / 180.*M_PI, g_lng / 180.*M_PI, g_zoom);
		cvShowImage("Little Planet", img2);
		break;
	}
}

void addGridAndCenter(IplImage* img, const int nx, const int ny) {
	int x, y;
	double x_step = img->width / (double)nx;
	double y_step = img->height / (double)ny;

	for (x = 0; x < nx / 2; x++) {
		cvLine(img, cvPoint(x*x_step, 0), cvPoint(x*x_step, img->height - 1), cvScalar(255, 255, 255, 0), 1);
	}
	for (x = nx/2; x < nx; x++) {
		cvLine(img, cvPoint(x*x_step, 0), cvPoint(x*x_step, img->height - 1), cvScalar(255, 0, 0, 0), 1);
	}
	for (y = 0; y < ny/2; y++) {
		cvLine(img, cvPoint(0, y*y_step), cvPoint(img->width - 1, y*y_step), cvScalar(255, 255, 255, 0), 1);
	}
	for (y = ny/2; y < ny; y++) {
		cvLine(img, cvPoint(0, y*y_step), cvPoint(img->width - 1, y*y_step), cvScalar(0, 255, 0, 0), 1);
	}

	cvCircle(img, cvPoint(img->width / 2, img->height / 2), 2, cvScalar(0, 0, 255, 0), CV_FILLED);
}

int main(int argc, char** argv) {
	char c;

	img1 = cvLoadImage(argv[1], 1);
	img2 = cvCreateImage(cvSize(img1->width/2, img1->height), img1->depth, img1->nChannels);

	cvNamedWindow("Pano", CV_WINDOW_AUTOSIZE);
	addGridAndCenter(img1, 36, 18);
	cvShowImage("Pano", img1);

	cvNamedWindow("Little Planet", CV_WINDOW_AUTOSIZE);
	cvSetMouseCallback("Little Planet", on_mouse);
	cv_LittlePlanet(img1, img2, g_lat/180.*M_PI, g_lng/180.*M_PI, g_zoom);
	cvShowImage("Little Planet", img2);

	printf("Enter any key?\n");
	// (3)'Esc'キーが押された場合に終了する
	while (1) {
		c = cvWaitKey(0);
		if (c == '\x1b')
			return 1;
	}

	// 
	cvDestroyWindow("Little Planet");
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);

	return 0;
}