#include "cv.h"

#define THRESHOLD_VALUE 0

void show(cv::Mat img, std::string windowName, int waitTime) {

	cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	cv::imshow(windowName, img);
	cv::waitKey(waitTime);
	cv::destroyWindow(windowName);
}

void getBoundingRects(cv::Mat img) {

    cv::Mat orig_img = img;

    cv::Mat scaled_img = img;

    cv::Mat scaled_gray_img;
    cv::cvtColor(scaled_img, scaled_gray_img, CV_BGR2GRAY);

    cv::Mat grad_img;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(scaled_gray_img, grad_img, cv::MORPH_GRADIENT, kernel);

    cv::Mat thresh_img;
    threshold(grad_img, thresh_img, THRESHOLD_VALUE, 255, cv::THRESH_OTSU);

    /*cv::Mat connected;
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 1));
    cv::morphologyEx(thresh_img, connected, cv::MORPH_CLOSE, kernel);*/

    cv::Mat mask = cv::Mat::zeros(thresh_img.size(), CV_8UC1);
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(thresh_img, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    for (int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {

        cv::Rect rect = cv::boundingRect(contours[idx]);
        cv::Mat maskROI(mask, rect);
        maskROI = cv::Scalar(0, 0, 0);

        cv::RotatedRect rrect = cv::minAreaRect(contours[idx]);
        double r = (double) countNonZero(maskROI) / (rrect.size.width * rrect.size.height);

        if (rrect.size.width > 1 && rrect.size.height > 1) {

            cv::Point2f pts[4];
            rrect.points(pts);

            for (int i = 0; i < 4; i++) {
                cv::line(scaled_img, cv::Point((int)pts[i].x, (int)pts[i].y), cv::Point((int)pts[(i+1)%4].x, (int)pts[(i+1)%4].y), cv::Scalar(255,0,0));
            }

            //cv::Mat cropped = scaled_img(rrect.boundingRect());
        }

    }

    cv::imwrite("cont.jpg", scaled_img);

}