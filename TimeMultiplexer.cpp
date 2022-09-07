#include "GrayGenerator.h"
#include "GrayEncoder.h"
#include "GrayDecoder.h"
#include "PGCapture.h"
#include "GrayCapture.h"
#include "TimeMultiplexer.h"

using namespace cv;
using namespace std;

TimeMultiplexer::TimeMultiplexer(GrayCapture* capturedImages, GrayDecoder* decoder) {

	this->decoder = decoder;
	this->capturedImages = capturedImages;

	this->lengthColCode = decoder->getLengthColCode();
	this->lengthRowCode = decoder->getLengthRowCode();

	multiplexColCode();
	multiplexRowCode();

	calculateMapping();

	//visualizeColCorrespondencies();
	//visualizeRowCorrespondencies();
}

/*
Image, ImageInverse are grayscale images with intensity [0,255]
Process:
A bit mask is created in order to distinguish which pixel should be one or zero of the image
Output:
Bit Mask Mat with elementwise max(Image-ImageInverse,0)
*/

Mat TimeMultiplexer::extractBitMask(Mat image, Mat imageInverse) {

	Mat result = Mat(image.rows, image.cols, CV_8UC1);
	subtract(image, imageInverse,result);
	threshold(result, result, 0, 1, THRESH_BINARY);

	return result;
}


/*
Input: 
Image, ImageInverse are grayscale images with intensity [0,255]
whiteImage,blackImage are grayscale images with intensity [0,255]
Process:
A bit mask is created in order to distinguish which pixel should be one or zero of the image
differenceImage = elementwise max(Image-ImageInverse,0)
referenceImage = elementwise max(whiteImage-blackImage,0)
Output:
Bit Mask Mat with elementwise differenceImage >= referenceImage
*/

Mat TimeMultiplexer::extractBitMask(Mat image, Mat imageInverse, Mat whiteImage, Mat blackImage) {

	Mat differenceImage = extractBitMask(image, imageInverse);

	Mat referenceImage = Mat(image.rows, image.cols, CV_8UC1);
	subtract(whiteImage, blackImage, referenceImage);
	threshold(referenceImage, referenceImage, 0, 1, THRESH_BINARY);

	Mat result = (differenceImage >= referenceImage);

	differenceImage.release();
	referenceImage.release();

	result.convertTo(result, CV_8UC1, 1 / 255.0);
	return result;
}

/*
Input: 
10 captured col colde images from the camera
10 captured col colde inverse images from the camera
Algorithm:
For each pair of image,imageInverse create its own Bit Mask
Consequently 10 Bit Masks will be created.
Output:
10 dimensional Tensor having a bit mask in each channel
*/
void TimeMultiplexer::multiplexColCode() {

	Mat* resultCol = new Mat[lengthColCode];

	Mat* capturedCol = capturedImages->getCapturedColImages();
	Mat* capturedColInverse = capturedImages->getCapturedColInvertedImages();

	for (int i = 0; i < lengthColCode; i++) {

		Mat image = capturedCol[i];
		Mat imageInverse = capturedColInverse[i];

		resultCol[i] = extractBitMask(image, imageInverse);

		image.release();
		imageInverse.release();
	}

	merge(resultCol, lengthColCode, colCodeTensor);
}

/*
Input:
10 captured row colde images from the camera
10 captured row colde inverse images from the camera
Algorithm:
For each pair of image,imageInverse create its own Bit Mask
Consequently 10 Bit Masks will be created.
Output:
10 dimensional Tensor having a bit mask in each channel
*/
void TimeMultiplexer::multiplexRowCode() {

	Mat* resultRow = new Mat[lengthRowCode];

	Mat* capturedRow = capturedImages->getCapturedRowImages();
	Mat* capturedRowInverse = capturedImages->getCapturedRowInvertedImages();

	for (int i = 0; i < lengthRowCode; i++) {

		Mat image = capturedRow[i];
		Mat imageInverse = capturedRowInverse[i];

		resultRow[i] = extractBitMask(image, imageInverse);

		image.release();
		imageInverse.release();
	}

	merge(resultRow, lengthRowCode, rowCodeTensor);
}

/*
Input: 
colCode Tensor is a 10 dimensional Mat having all Col Masks in its channels
rowCode Tensor is a 10 dimensional Mat having all Row Masks in its channels
Algorithm:
Iterate over each camera pixel, this pixel has a codeword (not neccessary unique )
the respective codewords are stored in both input tensors
Obtain 1 Pixel Sub Mat from Inputs of shape 1x1x10 and flatten it to a 1x10 row vector
this is the input of the respective decoding function in order to obtain row/col index
Output:
colCorrespondencies is a Mat that contains the beamer column index for every camera pixel
rowCorrespondencies is a Mat that contains the beamer row index for every camera pixel
*/
void TimeMultiplexer::calculateMapping() {

	int cameraCols = capturedImages->getCapturedColImages()[0].cols;
	int cameraRows = capturedImages->getCapturedRowImages()[0].rows;

	this->colCorrespondencies = Mat(cameraRows, cameraCols, CV_8UC1);
	this->rowCorrespondencies = Mat(cameraRows, cameraCols, CV_8UC1);

	for (int x = 0; x < cameraRows; x++) {
		for (int y = 0; y < cameraCols; y++) {

			Rect regionOfInterest = Rect(y,x, 1, 1);

			Mat colCodeWord = Mat(colCodeTensor, regionOfInterest);
			Mat rowCodeWord = Mat(rowCodeTensor, regionOfInterest);

			colCodeWord = colCodeWord.reshape(1, 1);
			rowCodeWord = rowCodeWord.reshape(1, 1);

			int colIndex = decoder->calculateColIndex(colCodeWord);
			int rowIndex = decoder->calculateRowIndex(rowCodeWord);

			colCorrespondencies.at<uchar>(x,y) = colIndex;
			rowCorrespondencies.at<uchar>(x,y) = rowIndex;

			colCodeWord.release();
			rowCodeWord.release();
		}
	}
}

/*
Compute gradient image of col correspondencies and display it
usefull metric to decide if calibration ands capture was sucessfull
has to look like a straight line going from origin to top right
*/
void TimeMultiplexer::visualizeColCorrespondencies() {

	double minValue, maxValue;
	minMaxIdx(colCorrespondencies, &minValue, &maxValue);
	int margin = 20;

	Mat visualization = 255 * Mat::ones( (int) maxValue + margin, colCorrespondencies.cols * colCorrespondencies.rows+margin, CV_8UC1);

	Mat vectorizedCol = colCorrespondencies.clone();
	flip(vectorizedCol, vectorizedCol, 0);
	vectorizedCol = vectorizedCol.t();
	vectorizedCol = vectorizedCol.reshape(1, 1);

	for (int i = 0; i < vectorizedCol.cols; i++) {

		int value = vectorizedCol.at<uchar>(0,i);
		Point2d point = Point2d(i,value);
		Scalar color = Scalar(0,0,0);
		circle(visualization, point, 3, color);

	}

	flip(visualization, visualization, 0);
	resize(visualization, visualization, Size(700, 400), 0, 0, INTER_NEAREST);

	namedWindow("Gradient Image COL CORRESPONDENCIES", WINDOW_AUTOSIZE);
	imshow("Gradient Image COL CORRESPONDENCIES", visualization);
	waitKey(0);

	vectorizedCol.release();
	visualization.release();
	
}

/*
Compute gradient image of row correspondencies and display it
usefull metric to decide if calibration ands capture was sucessfull
has to look like a straight line going from origin to top right
*/
void TimeMultiplexer::visualizeRowCorrespondencies() {

	double minValue, maxValue;
	minMaxIdx(rowCorrespondencies, &minValue, &maxValue);
	int margin = 20;

	Mat visualization = 255 * Mat::ones(maxValue+margin, rowCorrespondencies.cols * rowCorrespondencies.rows + margin, CV_8UC1);

	Mat vectorizedRow = rowCorrespondencies.clone();
	vectorizedRow = vectorizedRow.reshape(1, 1);

	for (int i = 0; i < vectorizedRow.cols; i++) {

		int value = vectorizedRow.at<uchar>(0, i);
		Point2d point = Point2d(i, value);
		Scalar color = Scalar(0, 0, 0);
		circle(visualization, point, 3, color);

	}

	flip(visualization, visualization, 0);
	resize(visualization, visualization, Size(700, 400), 0, 0, INTER_NEAREST);

	namedWindow("Gradient Image ROW CORRESPONDENCIES", WINDOW_AUTOSIZE);
	imshow("Gradient Image ROW CORRESPONDENCIES", visualization);
	waitKey(0);

	vectorizedRow.release();
	visualization.release();
}

TimeMultiplexer::~TimeMultiplexer() {

}


