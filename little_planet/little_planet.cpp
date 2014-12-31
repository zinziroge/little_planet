#define _USE_MATH_DEFINES

#include <math.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>

#include "little_planet.hpp"

// clat, clng [rad]
void cv_LittlePlanet(
	const IplImage* pano_img, const IplImage* lp_img, 
	const double clat, const double clng, const double zoom) {
	int x, y;	// at little planet image
	int cx, cy;
	int xx, yy;
	unsigned char r, g, b;
	double lat, lng; // ˆÜ“x[rad],Œo“x[rad] at pano image
	double rho, theta; // at little planet image
	double sphere_r;
	int u, v; // at pano image
	// debug
	double max_lat=-100, min_lat=100;
	double max_lng=-100, min_lng=100;

	sphere_r = zoom * lp_img->width;
	cx = lp_img->width / 2;
	cy = lp_img->height / 2;

	for (y = 0; y < lp_img->height; y++) {
		for (x = 0; x < lp_img->width; x++) {
#ifdef _DEBUG
//			printf("x=%d,y=%d\n", x, y);
#endif

			xx = x - cx;
			yy = y - cy;

			rho = sqrt(xx*xx + yy*yy);
			theta = 2*atan2(rho, (2.0 * sphere_r));
			lat = asin(cos(theta)*sin(clat) + yy*sin(theta)*cos(clat) / rho);
			lng = clng + atan2( xx*sin(theta), (rho*cos(clat)*cos(theta)-yy*sin(clat)*sin(theta)) );

#ifdef _DEBUG
			if (lat > max_lat) max_lat = lat;
			if (lat < min_lat) min_lat = lat;
			if (lng > max_lng) max_lng = lng;
			if (lng < min_lng) min_lng = lng;
#endif

			u = (int)(pano_img->width/2 - pano_img->width/2 * lng / M_PI);				// -M_PI <= lng <= M_PI
			v = (int)(pano_img->height/2 - pano_img->height/2 * lat / (M_PI / 2.0));	// -M_PI/2 <= lat <= M_PI/2

#ifdef _DEBUG
//			if( x==lp_img->width/4 && y%10==0 )
//				printf("u=%d,v=%d,lat=%lf,lng=%lf\n", 
//				u, v,
//				lat/M_PI*180, lng/M_PI*180);
#endif
			u = (u + pano_img->width) % pano_img->width;
			v = (v + pano_img->height) % pano_img->height;

			b = (unsigned char)(pano_img->imageData[pano_img->widthStep*v + pano_img->nChannels*u + 0]);
			g = (unsigned char)(pano_img->imageData[pano_img->widthStep*v + pano_img->nChannels*u + 1]);
			r = (unsigned char)(pano_img->imageData[pano_img->widthStep*v + pano_img->nChannels*u + 2]);

#ifdef _DEBUG
			//printf("b=%d,g=%d,r=%d\n", (int)b, (int)g, (int)r);
#endif
			lp_img->imageData[lp_img->widthStep*y + lp_img->nChannels*x + 0] = b;
			lp_img->imageData[lp_img->widthStep*y + lp_img->nChannels*x + 1] = g;
			lp_img->imageData[lp_img->widthStep*y + lp_img->nChannels*x + 2] = r;
			lp_img->imageData[lp_img->widthStep*y + lp_img->nChannels*x + 3] = 0;

		}
	}

#ifdef _DEBUG
//	printf("max_lat=%lf,min_lat=%lf,max_lng=%lf,min_lng=%lf\n",
//		max_lat / M_PI * 180, min_lat / M_PI * 180,
//		max_lng / M_PI * 180, min_lng / M_PI * 180
//		);
#endif

}