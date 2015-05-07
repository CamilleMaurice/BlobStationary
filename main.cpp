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
	IplImage *outblobs=NULL, *outlabels=NULL; //output images for blob extraction and blob labels
	BlobList *blobList = new BlobList();
	
	//BG subtractor initialization
	cv::BackgroundSubtractorMOG2 subtractor;
    //this works on the university environment:
    subtractor.nmixtures = 3;
	//this is for subsequent versions of OpenCV:
	//subtractor.set("nmixtures",3);
	
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
	
	frame = cvQueryFrame( capture );
		
		
	//create output windows	
	//namedWindow("frameM");
	//namedWindow("fireMask");
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
	//create output writer
	videowriter = cvCreateVideoWriter("result.mpg", CV_FOURCC('P','I','M','1'), 25, cvGetSize(frame), 1 );	
	cvInitFont( &font, CV_FONT_HERSHEY_DUPLEX, 0.8, 0.8, 0, 2, 8 );
	Mat frameM(frame);
	
	subtractor.operator()(frameM,fgM);
    subtractor.getBackgroundImage(bgM);
			
	do
	{
		i++;
		start =((double)cvGetTickCount()/(cvGetTickFrequency()*1000.) );
		//background subtraction (final foreground mask must be placed in 'fg' variable)		
		subtractor.operator()(frameM,fgM);
        subtractor.getBackgroundImage(bgM);
		
		IplImage* fg = new IplImage(fgM);
		
		//blob extraction
		extractBlobs(frame, fg, blobList); 
		//blob classification
		classifyBlobs(frame, fg, blobList); 
		outlabels = paintBlobClasses(frame, blobList);
		
		//showing results
		imshow("frame", frameM);
		cvShowImage("mainWin",outlabels);
		
		end = ((double)cvGetTickCount()/(cvGetTickFrequency()*1000.) );
		total=total + end-start;
		printf("Processing frame %d --> %.3g ms\n", i,end-start);
			
		cvWaitKey( 2 );

		//write frame result to video
		cvWriteFrame( videowriter, outlabels );		
		
		//release memory of temporal images
		cvReleaseImage( &outblobs );
		cvReleaseImage( &outlabels );

	} while(frame=cvQueryFrame(capture));
	//destroy all resources
	delete blobList;

	return EXIT_SUCCESS;
}
