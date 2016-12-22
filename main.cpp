#include "cv.h"
#include "tr_chars.hpp"

#define SRC_IMG "abc.jpg"

int main() {

	const cv::Mat img = cv::imread(SRC_IMG);

	if(!img.data) {

		std::cerr << "Error: Image not Found" << std::endl;
		return 1;
	}

	std::vector<cv::Mat> bboxes = getBoundingBoxes(img, 2, 2, 9, 1, true);

	for(int i = 0; i < bboxes.size(); i++) {

		std::vector<cv::Mat> innerbboxes = getBoundingBoxes(bboxes[i], 3, 3, 9, 1, false);
 
 		for(int j = 0; j < innerbboxes.size(); j++) {
 			imwrite("temp.png", innerbboxes[j]);
 			//cv::Mat test = cv::imread("temp.png");
 			//show(test);
 			std::cout << getCharacter_ML("temp.png") << " ";
 		}

 		if (innerbboxes.size() != 1) {

 			std::cout << std::endl;
 		}
	}

	std::cout << std::endl;

	return 0;
}
