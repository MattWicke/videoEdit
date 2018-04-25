#include "erToCube.h"
#include <iostream>

const double SCALE = 2.0;
double normalize(
        double m_in
        , double m_max
        )
{
    double temp;
    if(m_in < m_max/2.0)
    {
        temp = 1.0 - (2 * m_in / m_max);
    }
    else
    {
        temp = 1- (2 *m_in / m_max);
    }

    return temp;
}

Point2D getSphFromCart(
        double m_x
        , double m_y
        , double m_z
        , double src_width
        , double src_height
        )
{
    Point2D temp;
    double theta = atan2(m_y, m_x);
    double rho = sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
    double phi = acos(m_z / rho);

    if(theta <= M_PI)
        temp.x = src_width * (M_PI - theta) / (2 * M_PI);
    else
        temp.x = src_width * (3 * M_PI - theta) / (2 * M_PI);
    
    temp.y = src_height * (phi * 2) / (2 * M_PI);
    return temp;
}

Point2D genFrontIndices(
        int m_ii
        ,int m_jj
        ,double m_width
        ,double m_height
        )
{
    double y, z;
    const double x = 1.0;
    Point2D src_pnt;
    
    y = normalize(
            m_jj
            , m_height
            );

    z = normalize(
            m_ii
            , m_height
            );

    src_pnt = getSphFromCart(
            x
            ,y
            ,z
            ,m_width
            ,m_height
            );
    return src_pnt;
}


Point2D genLeftIndices(
        int m_ii
        ,int m_jj
        ,double m_width
        ,double m_height
        )
{
    double x, z;
    const double y = 1.0;
    Point2D src_pnt;
    
    x = normalize(
            m_jj
            , m_height
            );

    z = normalize(
            m_ii
            , m_width
            );

    src_pnt = getSphFromCart(
            x
            ,y
            ,z
            ,m_width
            ,m_height
            );

    return src_pnt;
}

Point2D genTopIndices(
        int m_ii
        ,int m_jj
        ,double m_width
        ,double m_height
        )
{
    double y, x;
    const double z = 1.0;
    Point2D src_pnt;
    
    y = normalize(
            m_jj
            , m_height
            );

    x = normalize(
            m_ii
            , m_width
            );

    src_pnt = getSphFromCart(
            x
            ,y
            ,z
            ,m_width
            ,m_height
            );

    return src_pnt;
}

Point2D genRightIndices(
        int m_ii
        ,int m_jj
        ,double m_width
        ,double m_height
        )
{
    double x, z;
    const double y = -1.0;
    Point2D src_pnt;
    
    x = normalize(
            m_jj
            , m_height
            );

    z = normalize(
            m_ii
            , m_width
            );

    src_pnt = getSphFromCart(
            x
            ,y
            ,z
            ,m_width
            ,m_height
            );

    return src_pnt;
}

cv::Mat genCubeFace(
        const cv::Mat& m_src
        , CubeFace m_face)
{
    cv::Mat dst(m_src.rows, m_src.cols, m_src.type());
    cv::Mat map_x(m_src.rows, m_src.cols, CV_32FC1);
    cv::Mat map_y(m_src.rows, m_src.cols, CV_32FC1);
    //** Front tile
    for(int ii = 0; ii < m_src.rows/SCALE; ii++)
    {
        for(int jj = 0; jj < m_src.rows/SCALE; jj++)
        {
            Point2D src_pnt;
            switch(m_face)
            {
                case CF_FRONT:
                    src_pnt = genFrontIndices(
                            ii * SCALE
                            ,jj* SCALE
                            ,m_src.cols
                            ,m_src.rows
                            );
                break;
                case CF_LEFT:
                    src_pnt = genLeftIndices(
                            ii* SCALE
                            ,jj* SCALE
                            ,m_src.cols
                            ,m_src.rows
                            );
                break;
                case CF_RIGHT:
                    src_pnt = genRightIndices(
                            ii* SCALE
                            ,jj* SCALE
                            ,m_src.cols
                            ,m_src.rows
                            );
                break;
                case CF_TOP:
                    src_pnt = genTopIndices(
                            ii * SCALE
                            ,jj * SCALE
                            ,m_src.cols
                            ,m_src.rows
                            );
                break;
            }

            map_x.at<float>(ii, jj) = src_pnt.x;
            map_y.at<float>(ii, jj) = src_pnt.y;
        }
    }
    cv::remap(
            m_src
            ,dst
            ,map_x
            ,map_y
            ,CV_INTER_LANCZOS4
            ,IPL_BORDER_CONSTANT
            ,cv::Scalar(0,0,0)
            );
    //cv::Mat temp = dst(cv::Rect(0,0, m_src.rows/SCALE, m_src.rows/SCALE));
    dst = dst(cv::Rect(0,0, m_src.rows/SCALE, m_src.rows/SCALE));
    //dst = temp;
    //temp.release();
    map_x.release();
    map_y.release();
    return dst;
}
