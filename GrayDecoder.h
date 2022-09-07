#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class GrayDecoder {

	typedef unsigned char uchar;

public:

	/** constructor */
	GrayDecoder(GrayGenerator* generatorRows, GrayGenerator* generatorCols);

	/** destructor **/
	~GrayDecoder();

	/** getter **/
	int getLengthColCode() {
		return lengthColCode;
	}
	int getLengthRowCode() {
		return lengthRowCode;
	}

	int calculateIndex(Mat rowCode, Mat target);
	int calculateRowIndex(Mat rowCode);
	int calculateColIndex(Mat colCode);


private:

	int lengthColCode;
	int lengthRowCode;

	Mat colCode;
	Mat colCodeInverted;
	Mat rowCode;
	Mat rowCodeInverted;

};
