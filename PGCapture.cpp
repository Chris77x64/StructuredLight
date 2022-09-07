/*
 * PGCapture.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: Jens Schindel
 */

#include "PGCapture.h"

PGCapture::PGCapture() {
	initFlyCap();
}

PGCapture::~PGCapture() {
	disconnectFlyCap();
}

// FlyCap Usabilities
bool PGCapture::initFlyCap() {
	// Connect the camera
	_error = _camera.Connect(0);
	if (_error != PGRERROR_OK)
	{
		std::cout << "Failed to connect to camera" << std::endl;
		return false;
	}

	// Get the camera info and print it out
	_error = _camera.GetCameraInfo(&_camInfo);
	if (_error != PGRERROR_OK)
	{
		std::cout << "Failed to get camera info from camera" << std::endl;
		return false;
	}
	std::cout << _camInfo.vendorName << " "
		<< _camInfo.modelName << " "
		<< _camInfo.serialNumber << std::endl;

	_error = _camera.StartCapture();
	if (_error == PGRERROR_ISOCH_BANDWIDTH_EXCEEDED)
	{
		std::cout << "Bandwidth exceeded" << std::endl;
		return false;
	}
	else if (_error != PGRERROR_OK)
	{
		std::cout << "Failed to start image capture" << std::endl;
		return false;
	}
	std::cout << "Step: flycap initialized" << std::endl;
	return true;
}

bool PGCapture::disconnectFlyCap() {
	_error = _camera.StopCapture();
	if (_error != PGRERROR_OK)
	{
		// This may fail when the camera was removed, so don't show
		// an error message
	}
	_camera.Disconnect();
	std::cout << "Step: flycap disconnected" << std::endl;
	return true;
}

Mat PGCapture::getImage() {
	// Get the image
	Image rawImage;
	FlyCapture2::Error error = _camera.RetrieveBuffer(&rawImage);
	if (error != PGRERROR_OK)
	{
		std::cout << "capture error" << std::endl;
	}

	// convert to rgb
	Image rgbImage;
	rawImage.Convert(FlyCapture2::PIXEL_FORMAT_BGR, &rgbImage);

	// convert to OpenCV Mat
	unsigned int rowBytes = (double)rgbImage.GetReceivedDataSize() / (double)rgbImage.GetRows();

	Mat img = Mat(rgbImage.GetRows(), rgbImage.GetCols(), CV_8UC3, rgbImage.GetData(), rowBytes).clone();

	//std::cout << "Step: flycap image taken" << std::endl;
	return img.clone();
}
