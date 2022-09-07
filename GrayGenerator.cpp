#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "GrayGenerator.h"

using namespace std;
using namespace cv;

void GrayGenerator::generate() {
	
	uchar data[8] = { 0,0,0,1,1,1,1,0};
	Mat previousCode = Mat(4, 2, CV_8UC1, data);

	// generate n-bit gray code using n-1 bit gray code
	for (int i = 2; i < length; i++) {

		// reverse given code sequence, s.t. first row -> last row, 2nd row -> 2nd. last row etc...
		Mat reversedCode = Mat(previousCode.rows, previousCode.cols, CV_8UC1);
		flip(previousCode, reversedCode, 0);

		// append ones to inverted code and zeros to previous code
		Mat zeroVector = Mat::zeros(previousCode.rows,1,CV_8UC1);
		Mat onesVector = Mat::ones(previousCode.rows, 1, CV_8UC1);

		hconcat(zeroVector, previousCode, previousCode);
		hconcat(onesVector, reversedCode, reversedCode);

		// append previous and inverted code
		vconcat(previousCode, reversedCode, previousCode);

		// free memory
		reversedCode.release();
		zeroVector.release();
		onesVector.release();

	}
	this->code = previousCode;
}

/*
Invert previously calculated code mat, 0->1 , 1->0
*/
void GrayGenerator::invert() {
	threshold(code, codeInverted, 0, 1, THRESH_BINARY_INV);
}

GrayGenerator::GrayGenerator(int length) {

	this->length = length;
	generate();
	invert();
	
}

GrayGenerator::~GrayGenerator() {
	this->code.release();
	this->codeInverted.release();
}