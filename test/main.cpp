// test.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.


#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

void convertToGrayscale(const Mat &img, Mat &imgGray){
	
	for (int x=0; x<img.cols; x++){		//for-loops for going through picture per pixel
		for (int y=0; y<img.rows; y++){
			
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

	Mat temp(imgLeft.rows, imgLeft.cols, CV_8UC1);		//matrices for storing cost volume temporarily
	

	int cost = 0;

	int a = windowSize/2;

	for(int i=0; i<=maxDisp; i++){		// for-loop for disparity values

		for (int x=0; x<imgLeft.cols; x++){		//for-loops for going through picture per pixel
			for (int y=0; y<imgLeft.rows; y++){

				for (int a=windowSize/2; a>=(-windowSize/2); a--){		//for-loops for scanning window
					for (int  b=windowSize/2; b>=(-windowSize/2); b--){

						if((x>i)&&(y>windowSize/2)&&((y+windowSize/2)<imgLeft.rows)&&((x+windowSize/2)<imgLeft.cols)){			//handling boundary problems
							if(((x-a)>0)&&((x-a)<imgLeft.cols)&&((y-b)>0)&&((y-b)<imgLeft.rows)){
								if((x-a-i)<0){		//setting costVolume very high for out-of-image-windows
									cost+=1000; 
								}else{				//calculating costVolums
									cost+=abs(imgLeft.at<uchar>(y-b,x-a) - imgRight.at<uchar>(y-b,x-a-i));
							
								}
							}
						}
					}		
				}

				cost=cost/(windowSize*windowSize);		//normalizing cost volume
				if(cost>255){
					cost=0;
				}
				temp.at<uchar>(y,x) = cost;			//write cost volume in temporary matrix and reset it
				cost = 0;
				
			}
		}
		costVolumeLeft.push_back(temp);			//writing cost Volume matrix in vector and reset it

		Mat temp2(imgLeft.rows, imgLeft.cols, CV_8UC1);
		temp = temp2;
	}
}


void selectDisparity(Mat &dispLeft, Mat &dispRight, vector<Mat> &costVolumeLeft, 
	vector<Mat> &costVolumeRight, int scaleDispFactor){
	uchar displevel=255;
	int disperity = 0;
	for (int x=0; x<dispLeft.cols; ++x){		//for-loops for going through picture per pixel
		for (int y=0; y<dispLeft.rows; ++y){
			for(int i=0; i<costVolumeLeft.size(); i++){			//for-loops for different disparity values

				uchar costVol = costVolumeLeft.at(i).at<uchar>(y,x);

				if(costVol<displevel){				//check if current cost volume is lower then previously lowest
				  displevel = costVolumeLeft.at(i).at<uchar>(y,x);			//if so, set it as new lower boundary
				  disperity = i;
				}  
			}

			dispLeft.at<uchar>(y,x)=disperity*scaleDispFactor;			//set pixel in desparity map
			disperity = 0;
			displevel=255;
		}
	}
}


int main(){
	const Mat imgLeft = imread("tsukuba_left.png", CV_LOAD_IMAGE_UNCHANGED);		//reading the two original images
	const Mat imgRight = imread("tsukuba_right.png", CV_LOAD_IMAGE_UNCHANGED);
	
	vector<Mat> costVolumeLeft;			//vectors for storing the different cost volumes for each disparity
	vector<Mat> costVolumeRight;
	
	int windowSize = 7;			//scanning window size
	int maxDisp = 15;			//definition of the maximum disparity

	Mat dispLeft(imgLeft.rows, imgLeft.cols, CV_8UC1);
	Mat dispRight(imgRight.rows, imgRight.cols, CV_8UC1);

	int scaleDispFactor = 16;		//stretches the disparity levels for better visualization

	Mat imgGrayLeft(imgLeft.rows, imgLeft.cols, CV_8UC1);			//computing the grayscale images				
	Mat imgGrayRight(imgRight.rows, imgRight.cols, CV_8UC1);

	convertToGrayscale(imgLeft, imgGrayLeft);
	convertToGrayscale(imgRight, imgGrayRight);

	computeCostVolume(imgGrayLeft, imgGrayRight, costVolumeLeft, costVolumeRight, windowSize, maxDisp);

	selectDisparity(dispLeft, dispRight, costVolumeLeft, costVolumeRight, scaleDispFactor);

	imshow("dispLeft",dispLeft);
	waitKey(0);
	return 0;
}

