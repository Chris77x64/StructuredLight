
/*
Mat blu;

Mat blue1 = Mat::ones(10, 10, CV_8UC1);
Mat blue2 = Mat::zeros(10, 10, CV_8UC1);
Mat blue3 = Mat::zeros(10, 10, CV_8UC1);

Mat bluearray[3]{
	blue1,blue2,blue3
};

merge(bluearray, 3, blu);
blu.convertTo(blu, CV_8UC3, 255);

Mat black;

Mat b1 = Mat::zeros(10, 10, CV_8UC1);
Mat b2 = Mat::zeros(10, 10, CV_8UC1);
Mat b3 = Mat::zeros(10, 10, CV_8UC1);

Mat barray[3]{
	b1,b2,b3
};

merge(barray, 3, black);
black.convertTo(black, CV_8UC3, 255);

Mat pixelGrayscale, pixelInvertedGrayscale;

pixel = Mat(blu, Rect(0, 0, 1, 1));
pixelInverted = Mat(black, Rect(0, 0, 1, 1));

cvtColor(pixel, pixelGrayscale, COLOR_BGR2GRAY);
cvtColor(pixelInverted, pixelInvertedGrayscale, COLOR_BGR2GRAY);

int intensityPixel = pixelGrayscale.at<uchar>(0, 0);
int intensityPixelInverted = pixelInvertedGrayscale.at<uchar>(0, 0);

cout << intensityPixel << " | " << intensityPixelInverted << endl;

namedWindow("image", WINDOW_AUTOSIZE);
imshow("image", black);
waitKey(0);

*/

/*
Test Data: single row of length 10
uchar data[10] = { 0,0,0,0,0,0,0,1,1,1 };
Mat test = Mat(1, 10, CV_8UC1, data);
*/

/*
Mat d1 = Mat::ones(5, 5, CV_8UC1);
Mat d2 = Mat::zeros(5, 5, CV_8UC1);

d1.setTo(5);
d2.setTo(2);
d1.at<uchar>(0, 0) = 0;

Mat r1 = Mat::ones(5, 5, CV_8UC1);
Mat r2 = Mat::zeros(5, 5, CV_8UC1);
r1.setTo(5);
r2.setTo(3);

Mat test = extractBitMask(d1, d2, r1, r2);
cout << test;
*/