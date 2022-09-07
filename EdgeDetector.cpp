#include "GrayGenerator.h"
#include "GrayEncoder.h"
#include "GrayDecoder.h"
#include "PGCapture.h"
#include "GrayCapture.h"
#include "TimeMultiplexer.h"
#include "EdgeDetector.h"

using namespace cv;
using namespace std;

EdgeDetector::EdgeDetector(TimeMultiplexer* multiplexer, PGCapture* camera) {
	this->multiplexer = multiplexer;
	this->camera = camera;
	captureImage();
	detectEdges();
	displayEnhancedImage();
}

/*
Perform affine transform warp with target(f(x,y),h(x,y)) = source(x,y)
apply this in iterative for all camera pixels, there always exists corresponding beamer pixel
f(x,y) are previously calculated row correspondencies ( Map: camera pixel -> Beamer Row )
h(x,y) are previously calculated col correspondencies ( Map: camera pixel -> Beamer Col )
source: image to display on beamer in camera space
target: result image on beamer in beamer space
*/
void EdgeDetector::warp(Mat* source, Mat* target, Mat* mapRows, Mat* mapCols) {

	for (int r = 0; r < source->rows; r++) {
		for (int c = 0; c < source->cols; c++) {

			int targetRow = mapRows->at<uchar>(r, c);
			int targetCol = mapCols->at<uchar>(r, c);

			target->at<uchar>(targetRow,targetCol) = source->at<uchar>(r, c);
		}
	}

}

/*
Apply laplacian with the captured camera image as input
translate the resulting image from camera to projector space (warp)
*/
void EdgeDetector::detectEdges() {

	Laplacian(capturedImage, calculatedEdgeImage, 3, 3, BORDER_DEFAULT);
	convertScaleAbs(calculatedEdgeImage, calculatedEdgeImage);
	calculatedEdgeImage.convertTo(calculatedEdgeImage, CV_8UC3);

	this->enhancedImage = 255 * Mat::ones(768, 1024, CV_8UC1);
	Mat* rowCorrespondencies = multiplexer->getRowCorrespondencies();
	Mat* colCorrespondencies = multiplexer->getColCorrespondencies();

	warp(&calculatedEdgeImage, &enhancedImage, rowCorrespondencies, colCorrespondencies);
}

/*
Capture an image of the object without the beamer running
*/

void EdgeDetector::captureImage() {

	Mat visualization = imread("visualization.jpg");
	namedWindow("Make sure projector is off!", WINDOW_AUTOSIZE);
	imshow("Make sure projector is off!", visualization);
	waitKey(0);
	this->capturedImage = this->camera->getImage();
}

/*
Display calculated enhanced image using the beamer
*/
void EdgeDetector::displayEnhancedImage() {

	Mat visualization = imread("visualization2.jpg");
	namedWindow("Make sure projector is on!", WINDOW_AUTOSIZE);
	imshow("Make sure projector is on!", visualization);
	waitKey(0);
	namedWindow("image", WINDOW_NORMAL);
	setWindowProperty("image", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
	imshow("image", enhancedImage);
	waitKey(0);
}

/*
Testing purposes
*/
EdgeDetector::EdgeDetector(TimeMultiplexer* multiplexer) {
	this->multiplexer = multiplexer;
	Mat visualization = imread("visualization.jpg");
	this->capturedImage = visualization;//this->camera->getImage();
	Laplacian(capturedImage, calculatedEdgeImage, 3, 3, BORDER_DEFAULT);
	convertScaleAbs(calculatedEdgeImage, calculatedEdgeImage);
	calculatedEdgeImage.convertTo(calculatedEdgeImage, CV_8UC3);

	resize(calculatedEdgeImage, calculatedEdgeImage, Size(this->multiplexer->getRowCorrespondencies()->cols, this->multiplexer->getRowCorrespondencies()->rows));

	this->enhancedImage = 255 * Mat::ones(768, 1024, CV_8UC1);
	Mat* rowCorrespondencies = multiplexer->getRowCorrespondencies();
	Mat* colCorrespondencies = multiplexer->getColCorrespondencies();

	warp(&calculatedEdgeImage, &enhancedImage, rowCorrespondencies, colCorrespondencies);


	namedWindow("image", WINDOW_NORMAL);
	setWindowProperty("image", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
	imshow("image", enhancedImage);
	waitKey(0);
}

/** destructor **/
EdgeDetector::~EdgeDetector() {

}