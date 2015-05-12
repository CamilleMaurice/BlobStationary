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

#define INPUT_VIDEO	"mh.mpg"

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
	IplImage *outblobs=NULL, *outlabels=NULL ; //output images for blob extraction and blob labels
	BlobList *blobList = new BlobList();
	Mat fgmask_counter ; //= NULL;//ew IplImage();
	Mat sfgmask ;//new IplImage;
	
	//cvSet(fgmask_counter, cvScalar(0));
	
	//BG subtractor initialization
	cv::BackgroundSubtractorMOG2 subtractor;
    //this works on the university environment:
    //subtractor.nmixtures = 3;
    //subtractor.history = 1;
	//this is for subsequent versions of OpenCV:
	subtractor.set("nmixtures",3);
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
	 
		cvNamedWindow("pouf", CV_WINDOW_AUTOSIZE); 
	frame = cvQueryFrame( capture );
	
	Mat frameM(frame);

	//affect
	fgmask_counter=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);//0;//check 


	//cvShowImage("pouf",sfgmask);
	//cvShowImage("test",fgmask_counter);
		

	//create output windows	
	
	namedWindow("sfg");
	//cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
	//create output writer
	//videowriter = cvCreateVideoWriter("result.mpg", CV_FOURCC('P','I','M','1'), 25, cvGetSize(frame), 1 );	
	//cvInitFont( &font, CV_FONT_HERSHEY_DUPLEX, 0.8, 0.8, 0, 2, 8 );


	subtractor.operator()(frameM,fgM);
    subtractor.getBackgroundImage(bgM);
			
	do
	{
	
		//cvShowImage("pouf",frame);
		i++;
		start =((double)cvGetTickCount()/(cvGetTickFrequency()*1000.) );
		//background subtraction (final foreground mask must be placed in 'fg' variable)		
		subtractor.operator()(frameM,fgM,0.000000000000001);
        subtractor.getBackgroundImage(bgM);
		
        int erosion_size = 1;	
		Mat element1 = getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),cv::Point(erosion_size, erosion_size) );
		Mat element2 = getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),cv::Point(erosion_size, erosion_size) );
		erode(fgM,fgM,element1);
		dilate(fgM,fgM,element2);
		
		
		//Compute Fg stationary  mask
		IplImage *fg = new IplImage(fgM);

		detectStationaryForeground(frame,fg,fgmask_counter,sfgmask);
	
		//imshow("sfg",sfgmask);
		//cvShowImage("pouf",fgmask_counter);
		//cvShowImage("pouf",sfgmask);
		
		end = ((double)cvGetTickCount()/(cvGetTickFrequency()*1000.) );
		total=total + end-start;
		printf("Processing frame %d --> %.3g ms\n", i,end-start);
			
		cvWaitKey( 2 );

		//
		//cvWriteFrame( videowriter, outlabels );		
		
		//release memory of temporal images
		cvReleaseImage( &outblobs );
		cvReleaseImage( &outlabels );

	} while(frame=cvQueryFrame(capture));
	//destroy all resources
	delete blobList;

	return EXIT_SUCCESS;
}
