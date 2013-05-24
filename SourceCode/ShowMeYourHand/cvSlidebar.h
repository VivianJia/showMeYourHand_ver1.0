#ifndef CVSLIDEBAR_H
#define CVSLIDEBAR_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

//callback function for opencv creatTrackbar 
void onSlided(int, void*);
	
#endif