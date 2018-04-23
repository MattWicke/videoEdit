#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include "types.h"

struct Point2D
{
    double x, y;
};

double normalize(
        double m_in
        , double m_max
        );

Point2D getSphFromCart(
        double m_x
        , double m_y
        , double m_z
        , double src_width
        , double src_height
        );

cv::Mat genFront(const cv::Mat& m_src);
cv::Mat genLeft(const cv::Mat& m_src);
cv::Mat genTop(const cv::Mat& m_src);
cv::Mat genRight(const cv::Mat& m_src);
