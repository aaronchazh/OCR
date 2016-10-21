#include "cv.h"

#define SRC_IMG "park.jpg"

int main() {

	const cv::Mat img = cv::imread(SRC_IMG);

	if(!img.data) {

		std::cerr << "Error: Image not Found" << std::endl;
		return 1;
	}

	std::vector<cv::Mat> bboxes = getBoundingBoxes(img, 2, 2, 9, 1);

	for(int i = 0; i < bboxes.size(); i++) {

		std::cout << getCharacter(bboxes[i]) << " ";
	}

	return 0;
}