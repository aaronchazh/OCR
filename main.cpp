#include "cv.h"

#define SRC_IMG "test.jpg"

int main() {

	cv::Mat img = cv::imread(SRC_IMG, CV_LOAD_IMAGE_GRAYSCALE);

	std::vector<cv::Rect> bRects = getBoundingRects(img);
	int numRects = bRects.size();

	std::vector<cv::Mat> boundingBoxes(numRects);

	for (int i = 0 ; i < numRects; i++) {

		boundingBoxes[i] = img(bRects[i]);
		show(boundingBoxes[i]);
	}

	return 0;
}