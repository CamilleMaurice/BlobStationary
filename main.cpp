//file description
/**
 * \file main.cpp 
 * \author Juan Carlos San Miguel
 * \date 09/03/2013
 * \brief Scheme for lab sessions of VATS (I2TIC Master - EPS (UAM)
 * \version 1.0\n 
 *
 */
//standard and OpenCV functions
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
//include for blob-related functions
#include "blobfuns.h" 

#define INPUT_VIDEO	"PETS06_S1-T1-C_3_abandoned_object_cif_mpeg.mpg"

using namespace cv;
using namespace std;

int main()
{		
	// Required variables for the program
	CvCapture* capture=NULL;
	CvVideoWriter *videowriter;
	Mat fgM, bgM;
	IplImage *frame=NULL; //images for background subtraction	
	Mat outblobsM, outlabelsM;
	IplImage *outblobs=NULL; //output images for blob extraction and blob labels
	BlobList *blobList = new BlobList();
	Mat fgmask_counter ; 
	Mat sfgmask ;

		
	//BG subtractor initialization
	cv::BackgroundSubtractorMOG2 subtractor;
    //this works on the university environment:
    subtractor.nmixtures = 3;
	//this is for subsequent versions of OpenCV:
	//subtractor.set("nmixtures",3);
//	namedWindow("frameM",CV_WINDOW_AUTOSIZE);	
	double start=0,end=0,total=0;	
	int i = 0;
	CvFont font;
	
	//module initialization

	//read video file & first frame
	if(!(capture = cvCaptureFromFile(INPUT_VIDEO)))
	{
		
		printf("Problem opening video file\n");
		return -1;
	}
	 
	cvNamedWindow("frame", CV_WINDOW_AUTOSIZE); 	
	namedWindow("stationarymask");
	cvNamedWindow("sBlobs");
	frame = cvQueryFrame( capture );
	
	Mat frameM(frame);
   	Mat frameBW ;
	cvtColor(frameM, frameBW, CV_BGR2GRAY);

	fgmask_counter = Mat::zeros(frameBW.rows, frameBW.cols, CV_8UC1);
	sfgmask = Mat::zeros(frameBW.rows, frameBW.cols, CV_8UC1);
	Mat fg_binary;		
	
	do
	{
	
		i++;
		start =((double)cvGetTickCount()/(cvGetTickFrequency()*1000.) );
		
		//background subtraction (final foreground mask must be placed in 'fg' variable)		
		subtractor.operator()(frameM,fgM,0.000000000000001);
        subtractor.getBackgroundImage(bgM);

		//Get a binary foreground mask
		threshold( fgM, fg_binary, 200, 255, 3);

		//Erosion and dilation operations
        int erosion_size = 1;	
		Mat element1 = getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(3 * erosion_size + 1, 2 * erosion_size + 1),cv::Point(erosion_size, erosion_size) );
		Mat element2 = getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),cv::Point(erosion_size, erosion_size) );
		erode(fg_binary,fg_binary,element1);
		dilate(fg_binary,fg_binary,element2);

		//Compute Fg stationary  mask
		IplImage *fg = new IplImage(fg_binary);
	
		detectStationaryForeground(frame,fg,fgmask_counter,sfgmask);
		extractBlobs(frame,sfgmask,blobList);
		outblobs = paintBlobClasses(frame,blobList);
		
		cvShowImage("sBlobs",outblobs);

		imshow("stationarymask",sfgmask);
		cvShowImage("frame",frame);
	
		end = ((double)cvGetTickCount()/(cvGetTickFrequency()*1000.) );

		total=total + end-start;
		printf("Processing frame %d --> %.3g ms\n", i,end-start);

			
		cvWaitKey( 2 );

		
		//cvWriteFrame( videowriter, outlabels );		
		
		//release memory of temporal images
		cvReleaseImage( &outblobs );
		

	} while(frame=cvQueryFrame(capture));
	//destroy all resources
	delete blobList;

	return EXIT_SUCCESS;
}
