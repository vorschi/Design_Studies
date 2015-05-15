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

void computeCostVolume(const Mat &imgLeft, const Mat &imgRight, vector<Mat> &costVolumeLeft, 
	vector<Mat> &costVolumeRight, int windowSize, int maxDisp){

	Mat temp(imgLeft.rows, imgLeft.cols, CV_8UC1);
	for (int x=windowSize; x<imgLeft.cols-windowSize; ++x){		//for-loops for going through picture per pixel
		for (int y=windowSize; y<imgLeft.rows-windowSize; ++y){
		
		
		
		}
	}


}


void selectDisparity(Mat &dispLeft, Mat &dispRight, vector<Mat> &costVolumeLeft, 
	vector<Mat> &costvolumeRight, int scaleDispFactor){


}


int main(){
	const Mat imgLeft = imread("tsukuba_left.png", CV_LOAD_IMAGE_UNCHANGED);
	const Mat imgRight = imread("tsukuba_right.png", CV_LOAD_IMAGE_UNCHANGED);
	
	vector<Mat> costVolumeLeft;
	vector<Mat> costVolumeRight;
	
	int windowSize = 5;
	int maxDisp = 15;

	Mat dispLeft;
	Mat dispRight;

	int scaleDispFactor = 16;

	Mat imgGrayLeft(imgLeft.rows, imgLeft.cols, CV_8UC1);				//single channel Mat
	Mat imgGrayRight(imgRight.rows, imgRight.cols, CV_8UC1);

	convertToGrayscale(imgLeft, imgGrayLeft);
	convertToGrayscale(imgRight, imgGrayRight);

	computeCostVolume(imgGrayLeft, imgGrayRight, costVolumeLeft, costVolumeRight, windowSize, maxDisp);

	selectDisparity(dispLeft, dispRight, costVolumeLeft, costVolumeRight, scaleDispFactor);

	

	//imshow("grau", imgGray);
	waitKey(0);
	return 0;
}

