#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "GrayGenerator.h"
#include "GrayEncoder.h"
#include "GrayDecoder.h"

#include "PGCapture.h"
#include "GrayCapture.h"

#include "TimeMultiplexer.h"
#include "EdgeDetector.h"


//using namespace std;
using namespace cv;

/* For OPENCV:
Make sure to run Visual Studio in x64 mode -> Active(Debug), Active(x64)
Path: C:\opencv\build\x64\vc14\bin\
Include DIR: C:\opencv\build\include
Linker: Additional Library C:\opencv\build\x64\vc14\lib
Linker: Additional Input opencv_world400d.lib
*/

/* FOR FLYCAPTURE 2
Download and install FlyCapture_2.13.3.61_x64.exe, select USB Camera in install gui
Path: C:\Program Files\Point Grey Research\FlyCapture2\bin64\
Include DIR: C:\Program Files\Point Grey Research\FlyCapture2\include
Linker: Additional Library C:\Program Files\Point Grey Research\FlyCapture2\lib64
Linker: Additional Input FlyCapture2d_v100.lib
*/

#include <iostream>


int main()
{
	const int beamerWidth = 1024;
	const int beamerHeight = 768;

	// both will be 10, 2^9=512 < 768 < 2^10=1024
	int widthGrayRows = ceil(log2(beamerHeight));
	int widthGrayCols = ceil(log2(beamerWidth));

	GrayGenerator* generatorRows = new GrayGenerator(widthGrayRows);
	GrayGenerator* generatorCols = new GrayGenerator(widthGrayCols);

	GrayEncoder* encoder = new GrayEncoder(generatorRows,generatorCols,beamerHeight,beamerWidth);
	GrayDecoder* decoder = new GrayDecoder(generatorRows, generatorCols);

	PGCapture* camera = new PGCapture();
	GrayCapture* capturedImages = new GrayCapture(encoder); //test data
	//GrayCapture* capturedImages = new GrayCapture(encoder,camera); // use this instead if camera is connected

	TimeMultiplexer* multiplexer = new TimeMultiplexer(capturedImages,decoder);

	/* TODO: */
	EdgeDetector* detector = new EdgeDetector(multiplexer); //test data
	//EdgeDetector* detector = new EdgeDetector(multiplexer, camera); // use this instead if camera is connected
}