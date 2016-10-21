#include "cv.h"

#define SRC_IMG "2.jpg"

int main() {

	const cv::Mat img = cv::imread(SRC_IMG);

	if(!img.data) {

		std::cerr << "Error: Image not Found" << std::endl;
		return 1;
	}

	std::vector<cv::Mat> bboxes = getBoundingBoxes(img, 2, 2, 9, 1);

	const cv::Mat temp = cv::imread("letters_numbers/D.bmp");

	if(!temp.data) {

		std::cerr << "Error: Image not Found" << std::endl;
		return 1;
	}

	double c = correlation(bboxes[3], temp);
	std::cout << c << std::endl;
	getCharacter(temp);

	return 0;
}