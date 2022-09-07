#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class GrayEncoder {

	typedef unsigned char uchar;

public:

	/** constructor */
	GrayEncoder(GrayGenerator* generatorRows, GrayGenerator* generatorCols,int rowsBeamer,int colsBeamer);

	/** destructor **/
	~GrayEncoder();

	/** getter **/
	Mat* getColCodeImages(void) {
		return this->colCodeImages;
	}
	Mat* getColCodeInvertedImages(void) {
		return this->colCodeInvertedImages;
	}
	Mat* getRowCodeImages(void) {
		return this->rowCodeImages;
	}
	Mat* getRowCodeInvertedImages(void) {
		return this->rowCodeInvertedImages;
	}
	Mat* getWhiteImage(void) {
		return &this->whiteImage;
	}
	Mat* getBlackImage(void) {
		return &this->blackImage;
	}

	int getColCodeLength(void) {
		return colCodeLength;
	}

	int getRowCodeLength(void) {
		return rowCodeLength;
	}


private:

	int rowsBeamer;
	int colsBeamer;

	int colCodeLength;
	int rowCodeLength;

	Mat colCode;
	Mat colCodeInverted;
	Mat rowCode;
	Mat rowCodeInverted;

	Mat *colCodeImages;
	Mat *colCodeInvertedImages;
	Mat *rowCodeImages;
	Mat *rowCodeInvertedImages;

	Mat whiteImage;
	Mat blackImage;

	void createImageCols(Mat data, Mat dataInverted,int index);
	void createImageRows(Mat data, Mat dataInverted, int index);

	void encodeCols();
	void encodeRows();
	void encodeAdditional();

	void displayColAndInverted();
	void displayRowAndInverted();

	void saveColAndInverted();
	void saveRowAndInverted();

};