#include "cv.h"

#define SRC_IMG "sign.jpg"

int main() {

	cv::Mat img = cv::imread(SRC_IMG);

	if(!img.data) {

		std::cerr << "Error: Image not Found" << std::endl;
		return 1;
	}

	std::vector<cv::Mat> bboxes = getBoundingBoxes(img, 3, 3, 9, 1);

	for (int i = 0; i < bboxes.size(); i++)
		show(bboxes[i]);

	return 0;
}