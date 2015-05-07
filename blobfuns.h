#ifndef BLOBFUNS_H_INCLUDE
#define BLOBFUNS_H_INCLUDE

#include "BasicBlob.h"
#include "BlobList.h"

//blob extraction functions (included in 'blobextrac.cpp')
int extractBlobs(IplImage * frame,IplImage * fgmask, BlobList *pBlobList);

//blob classification functions (included in 'blobclassify.cpp')
int classifyBlobs(IplImage* frame, IplImage *fgmask, BlobList *pBlobList);
IplImage *paintBlobClasses(IplImage* frame, BlobList *pBlobList);

#endif
