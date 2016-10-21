#ifndef _CV_
#define _CV_

#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

void show(const cv::Mat &img, std::string windowName = "image", int waitTime = 0);
std::vector<cv::Mat> getBoundingBoxes(const cv::Mat &img, int x1, int x2, int x3, int x4);
bool compareRect(const cv::Rect &r1, const cv::Rect &r2);
double correlation(cv::Mat image_1, cv::Mat image_2);
std::string getCharacter(const cv::Mat img);

#endif