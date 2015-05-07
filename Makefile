LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video
INCLUDES = /usr/include/opencv-2.3.1/

all: vatsLab2

vatsLab2: main.o BlobList.o BasicBlob.o blobextract.o blobclassify.o
	g++ -o vats main.o BlobList.o BasicBlob.o blobextract.o blobclassify.o $(LIBS) -lm

main.o: main.cpp
	g++ -Wno-write-strings -c main.cpp -I$(INCLUDES) -O
	
BlobList.o: BlobList.cpp
	g++ -Wno-write-strings -c BlobList.cpp -I$(INCLUDES) -O
	
BasicBlob.o: BasicBlob.cpp
	g++ -Wno-write-strings -c BasicBlob.cpp -I$(INCLUDES) -O
	
blobextract.o: blobextract.cpp
	g++ -Wno-write-strings -c blobextract.cpp -I$(INCLUDES) -O
	
blobclassify.o: blobclassify.cpp
	g++ -Wno-write-strings -c blobclassify.cpp -I$(INCLUDES) -O
	
clean: 
	rm *.o; rm result.mpg; rm vats
