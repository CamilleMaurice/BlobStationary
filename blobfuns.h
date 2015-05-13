#ifndef BLOBFUNS_H_INCLUDE
#define BLOBFUNS_H_INCLUDE
#include "BasicBlob.h"
#include "BlobList.h"
#include "opencv2/core/core.hpp"
using namespace cv;
//blob extraction functions (included in 'blobextrac.cpp')
int extractBlobs(IplImage * frame,Mat fgmask, BlobList *pBlobList);

//blob classification functions (included in 'blobclassify.cpp')
int classifyBlobs(IplImage* frame, IplImage *fgmask, BlobList *pBlobList);
IplImage *paintBlobClasses(IplImage* frame, BlobList *pBlobList);

//stationary foreground segmentation
int detectStationaryForeground(IplImage *frame, IplImage *fgmask, Mat fgmask_counter, Mat sfgmask);
#endif
