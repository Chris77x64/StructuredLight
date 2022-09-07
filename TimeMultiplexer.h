#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class TimeMultiplexer {

	typedef unsigned char uchar;

public:

	/** constructor */
	TimeMultiplexer(GrayCapture* capturedImages, GrayDecoder* decoder );

	/** destructor **/
	~TimeMultiplexer();

	Mat* getRowCorrespondencies(){
		return &rowCorrespondencies;
	}

	Mat* getColCorrespondencies() {
		return &colCorrespondencies;
	}

private:

	int lengthColCode;
	int lengthRowCode;

	Mat rowCodeTensor;
	Mat colCodeTensor;

	Mat rowCorrespondencies;
	Mat colCorrespondencies;

	GrayDecoder* decoder;
	GrayCapture* capturedImages;

	Mat extractBitMask(Mat image, Mat imageInverse);
	Mat extractBitMask(Mat image, Mat imageinverse, Mat whiteImage, Mat blackImage);

	void multiplexColCode();
	void multiplexRowCode();
	void calculateMapping();

	void visualizeColCorrespondencies();
	void visualizeRowCorrespondencies();
};