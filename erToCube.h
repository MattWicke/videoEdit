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

inline double normalize(
        double m_in
        , double m_max
        );

inline Point2D getSphFromCart(
        double m_x
        , double m_y
        , double m_z
        , double src_width
        , double src_height
        );

inline Point2D genFrontIndices(
        int m_ii
        ,int m_jj
        ,double m_width
        ,double m_height
        );

inline Point2D genLeftIndices(
        int m_ii
        ,int m_jj
        ,double m_width
        ,double m_height
        );

inline Point2D genTopIndices(
        int m_ii
        ,int m_jj
        ,double m_width
        ,double m_height
        );

inline Point2D genRightIndices(
        int m_ii
        ,int m_jj
        ,double m_width
        ,double m_height
        );

cv::Mat genCubeFace(
        const cv::Mat& m_src
        , CubeFace m_face
        );
