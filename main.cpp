#include "cv.h"

#define SRC_IMG "new.png"

int main() {

	cv::Mat img = cv::imread(SRC_IMG);
	std::vector<cv::Mat> bboxes = getBoundingBoxes(img);

	return 0;
}