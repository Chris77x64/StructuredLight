#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "GrayGenerator.h"
#include "GrayDecoder.h"

using namespace cv;
using namespace std;

GrayDecoder::GrayDecoder(GrayGenerator* generatorRows, GrayGenerator* generatorCols) {

	this->colCode = *generatorCols->getCode();
	this->colCodeInverted = *generatorCols->getInvertedCode();
	this->rowCode = *generatorRows->getCode();
	this->rowCodeInverted = *generatorRows->getInvertedCode();

	this->lengthColCode = generatorCols->getLength();
	this->lengthRowCode = generatorRows->getLength();
}

/*
Iterate row by row through the target and compare inputCode
with current row, if equal return index
Output: -1 if no match is found, else corresponding row index
*/
int GrayDecoder::calculateIndex(Mat inputCode, Mat target) {
	
	for (int i = 0; i < target.rows; i++) {

		Rect regionOfInterest = Rect(0, i, target.cols, 1);
		Mat roi = Mat(target, regionOfInterest);
		
		bool equal = std::equal(inputCode.begin<uchar>(), inputCode.end <uchar>(), roi.begin<uchar>());

		roi.release();

		if (equal) {
			return i;
		}

	}
	return -1;
}


int GrayDecoder::calculateRowIndex(Mat inputCode) {
	return calculateIndex(inputCode, rowCode);
}

int GrayDecoder::calculateColIndex(Mat inputCode) {
	return calculateIndex(inputCode, colCode);
}


GrayDecoder::~GrayDecoder() {
}

