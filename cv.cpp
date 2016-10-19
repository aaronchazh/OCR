#include "cv.h"

#define THRESHOLD_VALUE 0

void show(cv::Mat img, std::string windowName, int waitTime) {

	cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	cv::imshow(windowName, img);
	cv::waitKey(waitTime);
	cv::destroyWindow(windowName);
}

std::vector<cv::Mat> getBoundingBoxes(cv::Mat img, int x1, int x2, int x3, int x4) {

    cv::Mat scaled_img = img;

    cv::Mat scaled_gray_img;
    cv::cvtColor(scaled_img, scaled_gray_img, CV_BGR2GRAY);

    cv::Mat grad_img;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(x1, x2));
    cv::morphologyEx(scaled_gray_img, grad_img, cv::MORPH_GRADIENT, kernel);

    cv::Mat thresh_img;
    threshold(grad_img, thresh_img, THRESHOLD_VALUE, 255, cv::THRESH_OTSU);

    cv::Mat connected;
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(x3, x4));
    cv::morphologyEx(thresh_img, connected, cv::MORPH_CLOSE, kernel);

    cv::Mat mask = cv::Mat::zeros(thresh_img.size(), CV_8UC1);
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(connected, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    std::vector<cv::Mat> bboxes;

    for (int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {

        cv::Rect rect = cv::boundingRect(contours[idx]);

        // code for using rotated rects instead of rects
        /*
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

            std::cout << rrect.size.x << std::endl;

            cv::Mat M, rotated, cropped;
            float angle = rrect.angle;
            cv::Size rrect_size = rrect.size;

            if (rrect.angle < -45.) {

                angle += 90.0;
                std::swap(rrect_size.width, rrect_size.height);
            }

            M = cv::getRotationMatrix2D(rrect.center, angle, 1.0);
            cv::warpAffine(scaled_img, rotated, M, scaled_img.size(), cv::INTER_CUBIC);
            cv::getRectSubPix(rotated, rrect_size, rrect.center, cropped);

            bboxes.push_back(cropped);
        }
        */

        if (rect.size().width > 1 && rect.size().height > 1) {

            cv::rectangle(scaled_img, rect, cv::Scalar(255,0,0));
            bboxes.push_back(scaled_img(rect));
        }

    }

    std::cout << bboxes.size() << std::endl;

    cv::imwrite("cont.jpg", scaled_img);
    return bboxes;

}