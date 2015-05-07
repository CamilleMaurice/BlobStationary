#include "blobfuns.h"
#include "BasicBlob.h"
/**
 *	Blob classificaion between the five available classes in 'BasicBlob.h' (see CLASS typedef). All the input arguments must be 
 *  initialized when using this function.
 *
 * \param frame Input image
 * \param fgmask Foreground/Background segmentation mask (1-channel) 
 * \param pBlobList List to store the blobs found 
 *
 * \return Operation code (negative if not succesfull operation) 
 */
int classifyBlobs(IplImage* frame, IplImage *fgmask, BlobList *pBlobList)
{
	//check input conditions and return -1 if any is not satisfied
	if ( fgmask == NULL || frame == NULL || pBlobList == NULL )
	{
		return -1;
	}

	//required variables for classification
	//for aspect ratio
	float W,H,AR;
	float mean_car,mean_person,std_car,std_person;
	float Pp,Pc;
	int small_threshold = 30;
	//classify each blob of the list
	for(int i = 0; i < pBlobList->getBlobNum(); i++) 
	{
		//get the ith blob of the list
		BasicBlob* ith_blob = pBlobList->getBlob(i); 
		H = ith_blob->getHeight();
		W = ith_blob->getWidth();
		if (H<small_threshold  || W<small_threshold){
			ith_blob->setlabel((CLASS)1);
		}else{		
		
		
			//calculate the aspect ratio feature
			AR = W/H;
		
			//calculate the probability density of the normal distribution (for person)
			//mean_person = 0.3645;
			mean_person = 0.4067;
			//std_person = 0.0621;
			std_person = 0.1449;
			Pp = 1/(std_person*sqrt(2*M_PI))*exp(-(pow(AR-mean_person, 2))/(2*std_person*std_person));
		
			//Pp = normalPDF(AR, mean_person, std_person);
		

			//calculate the probability density of the normal distribution (for cars)
			mean_car = 1.8537;
			std_car = 0.6112;
			Pc = 1/(std_car*sqrt(2*M_PI))*exp(-(pow(AR-mean_car, 2))/(2*std_car*std_car));
		
			//Pc = normalPDF(AR, mean_car, std_car)
		

			//classify the blob
			//using simple statistical classifier	
			if(Pc <0.1 && Pp <0.1){	//The probability is too few 
				ith_blob->setlabel((CLASS)0);		
						
			}else{
				if (Pp > Pc)
				{
					ith_blob->setlabel((CLASS)1);
				}
				if (Pc >= Pp) {
					ith_blob->setlabel((CLASS)3);
				}
					
			}
		}
	}
	
	//destroy all resources
	//...
	
	//return OK code
	return 1;
}

//float normalPDF(float x, float mean, float std){
//	float result;
//	result = 1/(std*sqrt(2*M_PI))*exp(-(pow(x-mean, 2))/(2*std));
//	return result;	
//}


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
