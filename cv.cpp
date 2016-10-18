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

/*
#include "cv.h"

#define THRESHOLD_VALUE 0

void show(cv::Mat img, std::string windowName, int waitTime) {

	cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	cv::imshow(windowName, img);
	cv::waitKey(waitTime);
	cv::destroyWindow(windowName);
}

std::vector<cv::RotatedRect> getBoundingRects(cv::Mat img) {

    cv::Mat scaled_img;
    cv::pyrDown(img, scaled_img);
    cv::pyrDown(scaled_img, scaled_img);

    cv::Mat scaled_gray_img;
    cv::cvtColor(scaled_img, scaled_gray_img, CV_BGR2GRAY);

    cv::Mat grad_img;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(scaled_gray_img, grad_img, cv::MORPH_GRADIENT, kernel);

    cv::Mat thresh_img;
    threshold(grad_img, thresh_img, THRESHOLD_VALUE, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::Mat connected;
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 1));
    cv::morphologyEx(thresh_img, connected, cv::MORPH_CLOSE, kernel);

    cv::Mat mask = cv::Mat::zeros(thresh_img.size(), CV_8UC1);
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(connected, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    std::vector<cv::RotatedRect> test;

    for (int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {

        cv::Rect rect = cv::boundingRect(contours[idx]);
        cv::Mat maskROI(mask, rect);
        maskROI = cv::Scalar(0, 0, 0);

        cv::drawContours(mask, contours, idx, cv::Scalar(255, 255, 255), CV_FILLED);

        cv::RotatedRect rrect = cv::minAreaRect(contours[idx]);
        double r = (double) countNonZero(maskROI) / (rrect.size.width * rrect.size.height);

        cv::Scalar color;
        int thickness = 1;

        if (r > 0.25 && (rrect.size.height > 8 && rrect.size.width > 8)) {

            thickness = 2;
            color = cv::Scalar(0, 255, 0);
        }

        else {

            thickness = 1;
            color = cv::Scalar(0, 0, 255);
        }

        cv::Point2f pts[4];
        rrect.points(pts);
        test.push_back(rrect);

        for (int i = 0; i < 4; i++) {
            cv::line(scaled_img, cv::Point((int)pts[i].x, (int)pts[i].y), cv::Point((int)pts[(i+1)%4].x, (int)pts[(i+1)%4].y), color, thickness);
        }
    }

    cv::imwrite("cont.jpg", scaled_img);
    
    return test;
}
*/