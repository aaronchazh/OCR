#include "cv.h"

#define THRESHOLD_VALUE 150
#define TOO_LARGE 300
#define TOO_SMALL 40

void show(cv::Mat img, std::string windowName, int waitTime) {

	cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	cv::imshow(windowName, img);
	cv::waitKey(waitTime);
	cv::destroyWindow(windowName);
}

std::vector<cv::Rect> getBoundingRects(cv::Mat img) {

	cv::Mat thresh_img;
	cv::threshold(img, thresh_img, THRESHOLD_VALUE, 255, cv::THRESH_BINARY_INV);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));

	cv::Mat dilated_img;
	cv::dilate(thresh_img, dilated_img, kernel);

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(dilated_img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	
	int numContours = contours.size();
	std::vector<cv::Rect> boundRect;
	
	for (int i = 0; i < numContours; i++) {

		cv::Rect rect = cv::boundingRect(cv::Mat(contours[i]));

		if (rect.width > TOO_LARGE && rect.height > TOO_LARGE)
			continue;
		if (rect.width < TOO_SMALL || rect.height < TOO_SMALL)
			continue;

		cv::rectangle(img, rect, cv::Scalar(255, 255, 255), 5);
		boundRect.push_back(rect);
	}

	show(img);
	
	return boundRect;
}