#include "GrayGenerator.h"
#include "GrayEncoder.h"
#include "PGCapture.h"
#include "GrayCapture.h"

using namespace cv;
using namespace std;


GrayCapture::GrayCapture(GrayEncoder* encoder, PGCapture* camera) {

	this->encoder = encoder;
	this->camera = camera;

	// 10 in both cases
	this->lengthColCode = encoder->getColCodeLength();
	this->lengthRowCode = encoder->getRowCodeLength();

	//initialize space for camptured images
	this->capturedColImages = new Mat[lengthColCode];
	this->capturedColInvertedImages = new Mat[lengthColCode];
	this->capturedRowImages = new Mat[lengthRowCode];
	this->capturedRowInvertedImages = new Mat[lengthRowCode];

	start();
}

/*
Displays an input image using the beamer (fullscreen).
This image will be displayed for 1s=1000ms
Return: captured photo of the object during that time in grayscale ( 1 Mat instead of 3)
*/
Mat GrayCapture::captureImage(Mat imageToDisplay) {
	namedWindow("image", WINDOW_NORMAL);
	setWindowProperty("image", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
	imshow("image", imageToDisplay);
	Mat currentImage = camera->getImage();
	waitKey(2000);
	Mat result = Mat(currentImage.rows, currentImage.cols, CV_8UC1);
	cvtColor(currentImage, result, COLOR_BGR2GRAY);
	currentImage.release();
	return result;
}

/*
Code and Inverted images will be pairwise displayed for both row and col code
using a delay of 1s, keep in mind to adjust recording rate of camera
For each displayed gray code image the camera takes a photo
*/
void GrayCapture::start() {

	Mat* colCodeImages = encoder->getColCodeImages();
	Mat* colCodeInvertedImages = encoder->getColCodeInvertedImages();
	Mat* rowCodeImages = encoder->getRowCodeImages();
	Mat* rowCodeInvertedImages = encoder->getRowCodeInvertedImages();

	/* Display Col Code and Col Code Inverted in pairwise order and capture it using the camera*/
	for (int i = 0; i < lengthColCode; i++) {

		capturedColImages[i] = captureImage(colCodeImages[i]);
		capturedColInvertedImages[i] = captureImage(colCodeInvertedImages[i]);

	}

	/* Display Row Code and Row Code Inverted in pairwise order and capture it using the camera*/
	for (int i = 0; i < lengthRowCode; i++) {

		capturedRowImages[i] = captureImage(rowCodeImages[i]);
		capturedRowInvertedImages[i] = captureImage(rowCodeInvertedImages[i]);

	}

	/* Display all white image and black image in specified order and capture it using the camera*/
	capturedWhiteImage = captureImage(*encoder->getWhiteImage());
	capturedBlackImage = captureImage(*encoder->getBlackImage());

	

}

/*
Test data used when no camera is connected
*/
GrayCapture::GrayCapture(GrayEncoder* encoder) {

	this->encoder = encoder;
	// 10 in both cases
	this->lengthColCode = encoder->getColCodeLength();
	this->lengthRowCode = encoder->getRowCodeLength();

	//initialize space for captured images
	this->capturedColImages = new Mat[lengthColCode];
	this->capturedColInvertedImages = new Mat[lengthColCode];
	this->capturedRowImages = new Mat[lengthRowCode];
	this->capturedRowInvertedImages = new Mat[lengthRowCode];

	Rect regionOfInterest = Rect(50, 50, 100, 100);

	for (int i = 0; i < encoder->getColCodeLength(); i++) {

		capturedColImages[i] = Mat(encoder->getColCodeImages()[i], regionOfInterest);
		capturedColInvertedImages[i] = Mat(encoder->getColCodeInvertedImages()[i], regionOfInterest);
		capturedRowImages[i] = Mat(encoder->getRowCodeImages()[i], regionOfInterest);
		capturedRowInvertedImages[i] = Mat(encoder->getRowCodeInvertedImages()[i], regionOfInterest);
	}

}

/** destructor **/
GrayCapture::~GrayCapture() {

}