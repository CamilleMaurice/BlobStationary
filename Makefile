LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video
INCLUDES = /usr/include/opencv-2.3.1/

all: main.o BlobList.o BasicBlob.o blobextract.o blobstationary.o
	g++ -o vats main.o BlobList.o BasicBlob.o blobextract.o blobstationary.o $(LIBS) -lm

main.o: main.cpp
	g++ -Wno-write-strings -c main.cpp blobfuns.h -I$(INCLUDES) -O
	
BlobList.o: BlobList.cpp
	g++ -Wno-write-strings -c BlobList.cpp -I$(INCLUDES) -O
	
BasicBlob.o: BasicBlob.cpp
	g++ -Wno-write-strings -c BasicBlob.cpp -I$(INCLUDES) -O
	
blobextract.o: blobextract.cpp
	g++ -Wno-write-strings -c blobextract.cpp -I$(INCLUDES) -O
		
blobstationary.o: blobstationary.cpp
	g++ -Wno-write-strings -c blobstationary.cpp -I$(INCLUDES) -O
clean: 
	rm *.o; rm result.mpg; rm vats
