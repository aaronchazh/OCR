#include "cv.h"

#define SRC_IMG "new.png"

int main() {

	cv::Mat img = cv::imread(SRC_IMG);
	getBoundingRects(img);

	return 0;
}