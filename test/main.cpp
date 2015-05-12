// test.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.


#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;


void convertToGrayscale(const Mat &img, Mat &imgGray){
	
	for (int x=0; x<img.cols; ++x){		//for-loops for going through picture per pixel
		for (int y=0; y<img.rows; ++y){
			
			Vec3b intensity = img.at<Vec3b>(y, x);		//calculating luminance value for each pixel
			uchar blue = intensity.val[0];				//and pass it to the grayscale image
			uchar green = intensity.val[1];
			uchar red = intensity.val[2];	
			uchar grey = 0.21*red+0.72*green+0.07*blue;
			
			imgGray.at<uchar>(y, x) = grey;			
		}
	}	
}

int main()
{
	const Mat img = imread("tsukuba_left.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat imgGray(img.rows, img.cols, CV_8UC1);				//single channel Mat

	convertToGrayscale(img, imgGray);
		
	imshow("grau", imgGray);
	waitKey(0);
	return 0;
}

