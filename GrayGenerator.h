#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class GrayGenerator {

	typedef unsigned char uchar;

public:

	/** constructor */
	GrayGenerator(int length);

	/** destructor **/
	~GrayGenerator();

	/** getters */
	Mat* getCode() {
		return &code;
	}

	Mat* getInvertedCode() {
		return &codeInverted;
	}

	int getLength() {
		return length;
	}

private:

	int length;

	Mat code;
	Mat codeInverted;

	void generate();
	void invert();

};



