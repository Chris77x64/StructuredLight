#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class EdgeDetector {

	typedef unsigned char uchar;

public:

	/** constructor */
	EdgeDetector(TimeMultiplexer* multiplexer, PGCapture* camera);
	EdgeDetector(TimeMultiplexer* multiplexer); //for testing purposes

	/** destructor **/
	~EdgeDetector();


private:

	Mat capturedImage;
	Mat calculatedEdgeImage;
	Mat enhancedImage;

	TimeMultiplexer* multiplexer;
	PGCapture* camera;

	void warp(Mat* source, Mat* target, Mat* mapRows, Mat* mapCols);
	void captureImage();
	void detectEdges();
	void displayEnhancedImage();
}; 
