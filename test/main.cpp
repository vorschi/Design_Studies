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
		//=imgLeft.clone(); //TODO? -> CV_32FC1
	//(imgLeft.rows, imgLeft.cols, CV_32FC1);

	uchar cost = 0;
	uchar costLeft=0;
	uchar costRight=0;

	int a = windowSize/2;
	cout << a;
	for(int i=0; i<=maxDisp; i++){	
		for (int x=0; x<imgLeft.cols; ++x){		//for-loops for going through picture per pixel
			for (int y=0; y<imgLeft.rows; ++y){
				for (int a=windowSize/2; a>=(-windowSize/2); --a){
					for (int  b=windowSize/2; b>=(-windowSize/2); --b){
						if(((x-a)>0)&&((x-a)<imgLeft.cols)&&((y-b)>0)&&((y-b)<imgLeft.rows)){
							if((x-a-i)<0){
								//cost+=255; //TODO
							}else{
								cost+=abs(imgLeft.at<uchar>(y-b,x-a) - imgRight.at<uchar>(y-b,x-a-i));
							//costLeft = imgLeft.at<uchar>(y-b, x-a);
							//int tester = x-a-maxDisp;
							//cout << tester;
							//if(x> 10)
							//costRight = imgRight.at<uchar>(x-a-maxDisp, y-b);
							//cost+=abs(costLeft-costRight);
							//cout << a;
						//cout << b;
							}
						}
					}		
				}
				//if(x==383 && y==287)
				//	int c=0;
				temp.at<uchar>(y,x) = cost;
				cost = 0;
				
			}
		}
		costVolumeLeft.push_back(temp);

		Mat temp2(imgLeft.rows, imgLeft.cols, CV_8UC1);
		temp = temp2;
		//temp = Mat::ones(imgLeft.rows, imgLeft.cols, CV_8UC1);
	}
}


void selectDisparity(Mat &dispLeft, Mat &dispRight, vector<Mat> &costVolumeLeft, 
	vector<Mat> &costVolumeRight, int scaleDispFactor){
	uchar displevel=255;
	int disperity = 0;
	for (int x=0; x<dispLeft.cols; ++x){		//for-loops for going through picture per pixel
		for (int y=0; y<dispLeft.rows; ++y){
			for(int i=0; i<costVolumeLeft.size(); i++){
				uchar costVol = costVolumeLeft.at(i).at<uchar>(y,x);

				if(costVol<displevel){
				  displevel = costVolumeLeft.at(i).at<uchar>(y,x);
				  disperity = i;
				}  
			}

			dispLeft.at<uchar>(y,x)=disperity*scaleDispFactor;
			disperity = 0;
			displevel=255;
		}
	}
}


int main(){
	const Mat imgLeft = imread("tsukuba_left.png", CV_LOAD_IMAGE_UNCHANGED);
	const Mat imgRight = imread("tsukuba_right.png", CV_LOAD_IMAGE_UNCHANGED);
	
	vector<Mat> costVolumeLeft;
	vector<Mat> costVolumeRight;
	
	int windowSize = 3;
	int maxDisp = 15;

	Mat dispLeft(imgLeft.rows, imgLeft.cols, CV_8UC1);
	Mat dispRight(imgRight.rows, imgRight.cols, CV_8UC1);

	int scaleDispFactor = 16;

	Mat imgGrayLeft(imgLeft.rows, imgLeft.cols, CV_8UC1);				//single channel Mat
	Mat imgGrayRight(imgRight.rows, imgRight.cols, CV_8UC1);

	convertToGrayscale(imgLeft, imgGrayLeft);
	convertToGrayscale(imgRight, imgGrayRight);

	computeCostVolume(imgGrayLeft, imgGrayRight, costVolumeLeft, costVolumeRight, windowSize, maxDisp);

	selectDisparity(dispLeft, dispRight, costVolumeLeft, costVolumeRight, scaleDispFactor);

	

	//imshow("grau", imgGrayLeft);
	imshow("costvolume 0", costVolumeLeft.at(0));
	imshow("costvolume 15", costVolumeLeft.at(15));
	imshow("dispLeft",dispLeft);
	waitKey(0);
	return 0;
}

