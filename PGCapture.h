/*
 * PGCapture.h
 *
 *  Created on: Nov 7, 2016
 *      Author: Jens Schindel
 */

#ifndef PGCAPTURE_H_
#define PGCAPTURE_H_

#include "FlyCapture2.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace FlyCapture2;

class PGCapture {
public:
	// Constructor
	PGCapture();
	// Destructor
	~PGCapture();

	// Get image from PG Camera in OpenCVs Mat format
	Mat getImage();

private:
	// Connect to camera. Used in constructor.
	bool initFlyCap();
	// Disconnect from camera. Used in destructor.
	bool disconnectFlyCap();

private:
	FlyCapture2::Error _error;
	Camera _camera;
	CameraInfo _camInfo;
};

#endif /* PGCAPTURE_H_ */

