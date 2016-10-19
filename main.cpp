#include "cv.h"

#define SRC_IMG "2.jpg"

int main() {

	cv::Mat img = cv::imread(SRC_IMG);
	getBoundingRects(img);

	return 0;
}