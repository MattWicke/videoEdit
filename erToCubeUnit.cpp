#include "erToCube.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include "types.h"

int main(int argc, char* argv[])
{

    cv::Mat src = imread(argv[1]);
    cv::Mat front = genCubeFace(
                src
                , CF_FRONT
                );
    cv::Mat left = genCubeFace(
                src
                , CF_LEFT
                );
    cv::Mat right = genCubeFace(
                src
                , CF_RIGHT
                );
    cv::Mat top = genCubeFace(
                src
                , CF_TOP
                );
    cv::namedWindow("source", CV_WINDOW_NORMAL);
    cv::namedWindow("front", CV_WINDOW_NORMAL);
    cv::namedWindow("left", CV_WINDOW_NORMAL);
    cv::namedWindow("right", CV_WINDOW_NORMAL);
    cv::namedWindow("top", CV_WINDOW_NORMAL);
    cv::imshow("source", src);
    cv::imshow("front", front);
    cv::imshow("left", left);
    cv::imshow("right", right);
    cv::imshow("top", top);
    cv::waitKey();

    return 0;
}
