#include "blobfuns.h"
//#include "BasicBlob.h"
#include "opencv2/imgproc/imgproc_c.h"
#include <opencv2/opencv.hpp>
//#include <iostream>
#define MIN_WIDTH 15
#define MIN_HEIGHT 15

using namespace cv;

/**
 *	Blob extraction from 1-channel image (binary). The extraction is performed based
 *	on the analysis of the connected components. All the input arguments must be 
 *  initialized when using this function.
 *
 * \param frame Input image
 * \param fgmask Foreground/Background segmentation mask (1-channel) 
 * \param pBlobList List to store the blobs found 
 *
 * \return Operation code (negative if not succesfull operation) 
 */
int extractBlobs(IplImage *frameIpl, Mat fgmask, BlobList *pBlobList)
{	
	if(frameIpl == NULL){
		std::cerr<<"frame or FG mask not defined"<<std::endl<<"exiting..."<<std::endl;
		return -1;
	}
	
	//required variables for connected component analysis 
	Mat frame(frameIpl);
	//Mat fgmask(fgmaskIpl);
	Size s=frame.size();
	/*Pre-processing of mask to improve the blob extraction: 
	 * some eroding to get rid of most of the noise
	 * then some wider dilating to get merge regions that should be connected but aren't in the FG mask
	 */
	threshold(fgmask,fgmask,250,255,THRESH_BINARY);

	Mat fireMask(s.width+2,s.height+2,CV_8UC1);
	copyMakeBorder(fgmask,fireMask,1,1,1,1,BORDER_CONSTANT,0);
	Size s_mask=fireMask.size();

	//clear blob list (to fill with this function)
	pBlobList->clear();
	
	/* adaptation the mask to feed it to floodfill function:
	 * 0 meaning we should search for CC
	 * 255 meaning we shouldn't
	 */
	for (int x=0;x<s_mask.width;x++){
		for (int y=0;y<s_mask.height;y++){
			switch(fireMask.at<uchar>(y,x)){
				case 255:
					fireMask.at<uchar>(y,x)=0;//if a pix is detected as FG we want to search for CC in it
					break;
				case 0:
					fireMask.at<uchar>(y,x)=255;//not if it is detected as BG or shadow
					break;
				
			};
		}
	}
	for (int x=0;x<s.width;x++){
		for (int y=0;y<s.height;y++){
			//we assure this pixel is not already part of another CC
			if(fireMask.at<uchar>(y,x)==0){
				Rect blob_square = Rect();
				//difference allowed between pixel and neighbors
				int loDiff=0;
				int upDiff=0;
				//usage: connectivity|val for mask|fixed range|mask only<=>doesnt change input image
				int flags = (8 | (1<<8) | (0<<8) | (0<<8)); 
				//computing the flood fill given the current seed point
				floodFill(fireMask, cvPoint(x,y), 255, &blob_square, loDiff, upDiff, flags);
				
				/* We create a BasicBlob and add it to the list if
				 * its size is above the minimum defined in the header
				 */
				if(blob_square.width>= MIN_WIDTH && blob_square.height >= MIN_HEIGHT){
					BasicBlob * new_blob = new BasicBlob();
					new_blob->setX((blob_square.x));
					new_blob->setY((blob_square.y));
					new_blob->setWidth((blob_square.width));
					new_blob->setHeight((blob_square.height));
										
					pBlobList->addBlob(new_blob);
				}
			}
		}
	}
	return EXIT_SUCCESS;
}

/**
 *	Draw blobs (and its classes) with different color rectangles on the image 'frame'. All the input arguments must be 
 *  initialized when using this function.
 *
 * \param frame Input image 
 * \param pBlobList List to store the blobs found 
 *
 * \return Image containing the draw blobs. If no blobs have to be painted 
 *  or arguments are wrong, the function returns NULL. The memory of this image 
 * is created inside this function so it has to be released after its use by the 
 * function calling 'paintBlobImage'.
 */
IplImage *paintBlobClasses(IplImage* frame, BlobList *pBlobList)
{
	IplImage* blobImage;
	blobImage = cvCloneImage(frame);
	if ( frame == NULL || pBlobList == NULL )
	{
		return NULL;
	}	

	//required variables to paint
	float H,W,x,y;
	class classified;
	
	//paint each blob of the list
	for(int i = 0; i < pBlobList->getBlobNum(); i++) 	
	{	
		//get info about the ithblob
		BasicBlob* ith_blob = pBlobList->getBlob(i); 
		
		H = ith_blob->getHeight();
		W = ith_blob->getWidth();
		x = ith_blob->getX();
		y = ith_blob->getY();
		
		
		cvRectangle( blobImage, cvPoint(x,y), cvPoint(W+x, H+y), CV_RGB( 0, 255, 0 ), 2, 8, 0 );
		if(ith_blob->getlabel() == (CLASS) 3){
			cvRectangle( blobImage, cvPoint(x,y), cvPoint(W+x, H+y), CV_RGB( 100, 0, 0 ), 2, 8, 0 );
			
		}
		if(ith_blob->getlabel() == (CLASS) 1){
			cvRectangle( blobImage, cvPoint(x,y), cvPoint(W+x, H+y), CV_RGB( 0, 0, 100 ), 2, 8, 0 );
		}

	}

	//destroy all resources (if required)

	//return the image to show
	return blobImage;
}

