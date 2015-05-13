#include "blobfuns.h"
#include "opencv2/imgproc/imgproc_c.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#define FPS 25 //framerate of the input video
#define MIN_SECS 10.0 //minimum number of seconds to consider a foreground pixel as stationary

#define C_COST 0 //increment cost for stationary detection
#define D_COST 5 //penalization cost for stationary detection

using namespace std;
using namespace cv;

/**
 *	Function to detect stationary foreground based on accumulator techniques. All the input arguments must be 
 *  initialized prior using this function.
 *
 * \param frame Input image
 * \param fgmask Foreground/Background segmentation mask (1-channel, uint8)  
 * \param fgmask_counter counter for stationary pixels (1-channel, float) 
  obtained in the analysis of the previous frame (to be updated in this function)
 * \param sfgmask Foregroung segmentation mask (1-channel, uint8)  with the stationary pixels
 *
 * \return Operation code (negative if not succesfull operation) 
 */
int detectStationaryForeground(IplImage* frame, IplImage *fgmask, Mat fgmask_counter, Mat sfgmask, Mat C_counter,Mat D_counter)
{
	//check input validity and return -1 if any is not valid
	//...
	
	//num frames to static
	int numframes2static = (int)(FPS * MIN_SECS);
    Mat frameM(frame);
    Mat fgmaskM(fgmask);
   	Mat frameBW ;
	cvtColor(frameM, frameBW, CV_BGR2GRAY);
    Size s_frame = frameBW.size();

    Mat sfgmaskM(s_frame.height,s_frame.width,CV_8UC1);
	sfgmaskM = Mat::zeros(frameBW.rows, frameBW.cols, CV_8UC1);
	fgmask_counter = Mat::zeros(frameBW.rows, frameBW.cols, CV_8UC1);
	
	//Go though the image to increment or decrement the appropriate counter
	for(int i = 0; i < s_frame.width ; i++){
		for(int j = 0 ; j < s_frame.height ; j ++){			
			if(fgmaskM.at<uchar>(j,i) == 255 ){
				C_counter.at<uchar>(j,i) = C_counter.at<uchar>(j,i) + C_COST; 
				if(fgmask_counter.at<uchar>(j,i) + C_counter.at<uchar>(j,i)*(255/numframes2static) > 255){
					fgmask_counter.at<uchar>(j,i) = 255;
					cout << "STATIC OBJECT DETECTED"<< endl ;
					//ON NE PASSE JAMAIS ICIm j'ai limpression que quelque chose se reset. (le sac est blanc puis noir...puis blanc)
				}else{
				fgmask_counter.at<uchar>(j,i) = fgmask_counter.at<uchar>(j,i) +C_counter.at<uchar>(j,i)*(255/numframes2static);
				}				
				
			}else{
				C_counter.at<uchar>(j,i) = 0;
				//~ D_counter.at<uchar>(j,i) = D_counter.at<uchar>(j,i) + D_COST; 
				//~ if(fgmask_counter.at<uchar>(j,i) - D_counter.at<uchar>(j,i)*(255/numframes2static) < 0 ){
					//~ fgmask_counter.at<uchar>(j,i) = 0;
				//~ }else{
					//~ fgmask_counter.at<uchar>(j,i) = fgmask_counter.at<uchar>(j,i) -D_counter.at<uchar>(j,i)*(255/numframes2static);
				//~ }
			//~ }
			   
		}
	}
}

	
	//operate with fgmask_counter to update sfgmask
	//...
	
	//~ for(int i = 0; i<s_frame.width ; i++){
		//~ for(int j = 0 ; j<s_frame.height ; j ++){
			//~ 
			//~ if(fgmask_counter.at<uchar>(j,i) >= 240){
				//~ sfgmaskM.at<uchar>(j,i) = 255;
			//~ }else{
				//~ sfgmaskM.at<uchar>(j,i) = 0;
			//~ }
		//~ }
	//~ }
	
	namedWindow("sfg");
	imshow("sfg",C_counter);
	
	
	sfgmask = sfgmaskM.clone();
	


	return 1;
}
