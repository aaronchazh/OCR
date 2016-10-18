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

/*
#include "cv.h"

#define SRC_IMG "test.jpg"

int main() {

	cv::Mat img = cv::imread(SRC_IMG);

	std::vector<cv::RotatedRect> bRects = getBoundingRects(img);
	int numRects = bRects.size();

	std::vector<cv::Mat> boundingBoxes(numRects);

	for (int i = 0 ; i < numRects; i++) {

		cv::RotatedRect rect = bRects[i];
		cv::Mat M, rotated, cropped;
		float angle = rect.angle;
		cv::Size rect_size = rect.size;

		if (rect.angle < -45.) {

            angle += 90.0;
            std::swap(rect_size.width, rect_size.height);
        }

        M = cv::getRotationMatrix2D(rect.center, angle, 1.0);
        cv::warpAffine(img, rotated, M, img.size(), cv::INTER_CUBIC);
        getRectSubPix(rotated, rect_size, rect.center, cropped);
        boundingBoxes[i] = cropped;
        show(cropped);
	}

	return 0;
}
*/