#include "blobfuns.h"

#define FPS 25 //framerate of the input video
#define MIN_SECS 10.0 //minimum number of seconds to consider a foreground pixel as stationary

#define C_COST 1 //increment cost for stationary detection
#define D_COST 5 //penalization cost for stationary detection

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
int detectStationaryForeground(IplImage* frame, IplImage *fgmask, IplImage* fgmask_counter, IplImage *sfgmask)
{
	//check input validity and return -1 if any is not valid
	//...
	
	//num frames to static
	int numframes2static = (int)(FPS * MIN_SECS);
    Mat frameM(frame);
    Mat fgmaskM(fgmask);
    Mat fgmask_counterM(fgmask_counter);
    Mat sfgmaskM(sfgmask);
    
    Size s_frame = frameM.size();
    
	//operate with fgmask to update fgmask_counter
	//...
	//go through the image
	//check if is FG
	//   check if in the previous frame it was FG 
	//(we must have the same FG in 2 consecutive frames
	for(int i = 0; i<s_frame.width ; i++){
		for(int j = 0 ; i<s_frame.height ; j ++){
			if(fgmask.at<uchar>(j,i) == 255){
				fgmask_counter.at<uchar>(j,i) += 1;
			}
		}
	}
				
			
	//operate with fgmask_counter to update sfgmask
	//...
	for(int i = 0; i<s_frame.width ; i++){
		for(int j = 0 ; i<s_frame.height ; j ++){
			if(fgmask_counter.at<uchar>(j,i) >2){
				sfgmask.at<uchar>(j,i) = 255;
			}
		}
	}
	
	return 1;
}