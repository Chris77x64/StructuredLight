#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class GrayCapture {

	typedef unsigned char uchar;

public:

	/** constructor */
	GrayCapture(GrayEncoder* encoder, PGCapture* camera);
	GrayCapture(GrayEncoder* encoder); //for testing purposes

	/** destructor **/
	~GrayCapture();

	Mat* getCapturedColImages() {
		return capturedColImages;
	}
	Mat* getCapturedColInvertedImages(){
		return capturedColInvertedImages;
	}
	Mat* getCapturedRowImages(){
		return capturedRowImages;
	}
	Mat* getCapturedRowInvertedImages(){
		return capturedRowInvertedImages;
	}

	Mat* getCapturedWhiteImage(){
		return &capturedWhiteImage;
	}

	Mat* getCapturedBlackImage() {
		return &capturedBlackImage;
	}

private:

	int lengthColCode;
	int lengthRowCode;

	Mat* capturedColImages;
	Mat* capturedColInvertedImages;

	Mat* capturedRowImages;
	Mat* capturedRowInvertedImages;

	Mat capturedWhiteImage;
	Mat capturedBlackImage;

	GrayEncoder* encoder;
	PGCapture* camera;

	Mat captureImage(Mat imageToDisplay );
	void start();

};
