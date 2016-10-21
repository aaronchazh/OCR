#include "cv.h"

#define THRESHOLD_VALUE 0
#define NUM_CHARACTERS 36

void show(const cv::Mat &img, std::string windowName, int waitTime) {

	cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	cv::imshow(windowName, img);
	cv::waitKey(waitTime);
	cv::destroyWindow(windowName);
}

bool compareRect(const cv::Rect &r1, const cv::Rect &r2) {

    if (abs((r1.y + r1.size().height) - (r2.y + r2.size().height)) > 20) {

        return (r1.y + r1.size().height) < (r2.y + r2.size().height);
    }

    return r1.x < r2.x;
}

double correlation(cv::Mat image1, cv::Mat image2) {

    cv::Size scaleSize = image2.size();

    cv::Mat scaled_img;
    cv::resize(image1, scaled_img, scaleSize);

    cv::Mat scaled_gray_img1;
    cv::cvtColor(scaled_img, scaled_gray_img1, CV_BGR2GRAY);

    cv::Mat thresh_img1;
    threshold(scaled_gray_img1, thresh_img1, THRESHOLD_VALUE, 255, cv::THRESH_OTSU);

    cv::Mat scaled_gray_img2;
    cv::cvtColor(image2, scaled_gray_img2, CV_BGR2GRAY);

    cv::Mat thresh_img_temp2;
    threshold(scaled_gray_img2, thresh_img_temp2, THRESHOLD_VALUE, 255, cv::THRESH_OTSU);

    cv::Mat thresh_img2;
    cv::bitwise_not(thresh_img_temp2, thresh_img2);
    
    int n_pixels = thresh_img1.rows * thresh_img1.cols;

    cv::Scalar im1_Mean, im1_Std, im2_Mean, im2_Std;
    meanStdDev(thresh_img1, im1_Mean, im1_Std);
    meanStdDev(thresh_img2, im2_Mean, im2_Std);

    double covar = (thresh_img1 - im1_Mean).dot(thresh_img2 - im2_Mean) / n_pixels;
    double correl = covar / (im1_Std[0] * im2_Std[0]);

    return correl;
}

std::vector<cv::Mat> getBoundingBoxes(const cv::Mat &img, int x1, int x2, int x3, int x4) {

    cv::Mat scaled_img = img.clone();

    cv::Mat scaled_gray_img;
    cv::cvtColor(scaled_img, scaled_gray_img, CV_BGR2GRAY);

    cv::Mat grad_img;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(x1, x2));
    cv::morphologyEx(scaled_gray_img, grad_img, cv::MORPH_GRADIENT, kernel);

    cv::Mat thresh_img;
    threshold(grad_img, thresh_img, THRESHOLD_VALUE, 255, cv::THRESH_OTSU);

    /*cv::Mat connected;
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(x3, x4));
    cv::morphologyEx(thresh_img, connected, cv::MORPH_CLOSE, kernel);*/

    cv::Mat mask = cv::Mat::zeros(thresh_img.size(), CV_8UC1);
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(thresh_img, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    std::vector<cv::Rect> bRects;

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

        if (rect.size().width > 30 && rect.size().height > 30) {

            cv::rectangle(scaled_img, rect, cv::Scalar(255,0,0));
            bRects.push_back(rect);
        }

    }

    std::sort(bRects.begin(), bRects.end(), compareRect);
    
    std::vector<cv::Mat> bboxes(bRects.size());
    for (int i = 0; i < bRects.size(); i++) {
        cv::Rect rect_nobox(bRects[i].x, bRects[i].y, bRects[i].size().width, bRects[i].size().height);
        bboxes[i] = img(rect_nobox);
    }

    show(scaled_img);

    return bboxes;

}

std::string getCharacter(const cv::Mat img) {

    std::string characters[] = {"A", "B", "C", "D", "E",
                                "F", "G", "H", "I", "J",
                                "K", "L", "M", "N", "O",
                                "P", "Q", "R", "S", "T",
                                "U", "V", "W", "X", "Y", "Z",
                                "0", "1", "2", "3", "4",
                                "5", "6", "7", "8", "9"};

    std::string imgFold = "letters_numbers/";

    std::vector<double> correlations;

    for (int i = 0; i < NUM_CHARACTERS; i++) {

        std::ostringstream path;
        path << imgFold;
        path << characters[i];
        path << ".bmp";

        cv::Mat character = cv::imread(path.str());

        correlations.push_back(correlation(img, character));
    }

    double max = 0;
    int maxIdx;

    for (int i = 0; i < correlations.size(); i++) {

        if(correlations[i] > max) {

            max = correlations[i];
            maxIdx = i;
        }
    }

    return characters[maxIdx];

}