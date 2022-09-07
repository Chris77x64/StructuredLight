#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "GrayGenerator.h"
#include "GrayEncoder.h"


using namespace std;
using namespace cv;

/*
Input: 
data [ .. .. .. .. ] is row vector of shape 1024 x 1
dataInverted [ .. .. .. .. ] is row vector of shape 1024 x 1
Output:
Mat result of shape 1024x768, s.t. each row is identical to data
Mat resultInverted of shape 1024x768, s.t. each row is identical to dataInverted
*/

void GrayEncoder::createImageCols(Mat data,Mat dataInverted,int index) {
	
	Mat result = data.clone();
	Mat resultInverted = dataInverted.clone();

	for (int i = 1; i < rowsBeamer; i++) {
		vconcat(result, data, result);
		vconcat(resultInverted, dataInverted, resultInverted);
	}


	result.convertTo(result, CV_8UC1, 255);
	resultInverted.convertTo(resultInverted, CV_8UC1, 255);

	this->colCodeImages[index] = result;
	this->colCodeInvertedImages[index] = resultInverted;

}

/*
Iterate through each coulmn of the graycode matrix.
Each column will result in an Image of column stripes
created using createImageCols() function
*/

void GrayEncoder::encodeCols() {


	for (int i = 0; i < colCode.cols; i++) {

		Rect regionOfInterest = Rect(i, 0, 1, colCode.rows);

		Mat roiCode = Mat(colCode, regionOfInterest);
		Mat roiCodeInverted = Mat(colCodeInverted, regionOfInterest);

		createImageCols(roiCode.t(),roiCodeInverted.t(),i);

	}
}


/*
Input:
data [ .. .. .. .. ]^T is col vector of shape 1x 1024 
dataInverted [ .. .. .. .. ]^T is col vector of shape 1 x 1024
Trick:
Since beamer has height 768 cut data and dataInverted to 1x 768
Output:
Mat result of shape 1024x768, s.t. each row is identical to data
Mat resultInverted of shape 1024x768, s.t. each row is identical to dataInverted
*/

void GrayEncoder::createImageRows(Mat data, Mat dataInverted,int index) {

	data = Mat(data, Rect(0, 0, 1, this->rowsBeamer));
	dataInverted = Mat(dataInverted, Rect(0, 0, 1, this->rowsBeamer));

	Mat result = data.clone();
	Mat resultInverted = dataInverted.clone();

	for (int i = 1; i < colsBeamer; i++) {
		hconcat(result, data, result);
		hconcat(resultInverted, dataInverted, resultInverted);
	}

	result.convertTo(result, CV_8UC1, 255);
	resultInverted.convertTo(resultInverted, CV_8UC1, 255);

	this->rowCodeImages[index] = result;
	this->rowCodeInvertedImages[index] = resultInverted;

}

/*
Iterate through each coulmn of the graycode matrix.
Each column will result in an Image of row stripes
created using createImageRows() function
*/

void GrayEncoder::encodeRows() {
	for (int i = 0; i < rowCode.cols; i++) {

		Rect regionOfInterest = Rect(i, 0, 1, rowCode.rows);

		Mat roiCode = Mat(rowCode, regionOfInterest);
		Mat roiCodeInverted = Mat(rowCodeInverted, regionOfInterest);

		createImageRows(roiCode, roiCodeInverted,i);

	}
}

void GrayEncoder::encodeAdditional() {

	this->blackImage = Mat::zeros(rowsBeamer, colsBeamer, CV_8UC1);
	this->whiteImage = Mat::ones(rowsBeamer, colsBeamer, CV_8UC1);

	blackImage.convertTo(blackImage, CV_8UC1, 255);
	whiteImage.convertTo(whiteImage, CV_8UC1, 255);

}



void GrayEncoder::displayColAndInverted() {

	for (int i = 0; i < colCode.cols; i++) {

		namedWindow("COLUMN CODE IMAGE " + to_string(i), WINDOW_AUTOSIZE);
		imshow("COLUMN CODE IMAGE " + to_string(i), colCodeImages[i]);
		waitKey(0);

		namedWindow("COLUMN CODE INVERTED IMAGE "+ to_string(i), WINDOW_AUTOSIZE);
		imshow("COLUMN CODE INVERTED IMAGE " + to_string(i), colCodeInvertedImages[i]);
		waitKey(0);
	}

}
void GrayEncoder::displayRowAndInverted() {

	for (int i = 0; i < rowCode.cols; i++) {

		namedWindow("ROW CODE IMAGE " + to_string(i), WINDOW_AUTOSIZE);
		imshow("ROW CODE IMAGE " + to_string(i), rowCodeImages[i]);
		waitKey(0);

		namedWindow("ROW CODE INVERTED IMAGE " + to_string(i), WINDOW_AUTOSIZE);
		imshow("ROW CODE INVERTED IMAGE " + to_string(i), rowCodeInvertedImages[i]);
		waitKey(0);
	}
}


/*
In order to save images make sure to create a folder within the current main directory called "Images"
*/

void GrayEncoder::saveColAndInverted() {
	for (int i = 0; i < colCode.cols; i++) {
		 imwrite("Images/COL"+ to_string(i) + ".jpg", colCodeImages[i]);
		 imwrite("Images/COLINV" + to_string(i) + ".jpg", colCodeInvertedImages[i]);
	}
	imwrite("Images/black.jpg", this->blackImage);
	imwrite("Images/white.jpg", this->whiteImage);
}

void GrayEncoder::saveRowAndInverted() {
	for (int i = 0; i < rowCode.cols; i++) {
		imwrite("Images/ROW" + to_string(i) + ".jpg", rowCodeImages[i]);
		imwrite("Images/ROWINV" + to_string(i) + ".jpg", rowCodeInvertedImages[i]);

	}
}

GrayEncoder::GrayEncoder(GrayGenerator* generatorRows, GrayGenerator* generatorCols, int rowsBeamer, int colsBeamer) {

	this->rowsBeamer = rowsBeamer;
	this->colsBeamer = colsBeamer;

	this->colCode = *generatorCols->getCode();
	this->colCodeInverted = *generatorCols->getInvertedCode();
	this->rowCode = *generatorRows->getCode();
	this->rowCodeInverted = *generatorRows->getInvertedCode();

	this->colCodeLength = generatorCols->getLength();
	this->rowCodeLength = generatorRows->getLength();

	this->colCodeImages = new Mat[colCodeLength];
	this->colCodeInvertedImages = new Mat[colCodeLength];
	this->rowCodeImages = new Mat[rowCodeLength];
	this->rowCodeInvertedImages = new Mat[rowCodeLength];

	this->encodeCols();
	this->encodeRows();
	this->encodeAdditional();
}

GrayEncoder::~GrayEncoder() {
	
}