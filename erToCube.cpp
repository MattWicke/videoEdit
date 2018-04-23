#include "erToCube.h"
#include <iostream>

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

cv::Mat genFront(const cv::Mat& m_src)
{
    cv::Mat dst(m_src.rows/2, m_src.rows/2, m_src.type());
    //** Front tile
    for(int ii = 0; ii < dst.rows; ii++)
    {
        for(int jj = 0; jj < dst.cols; jj++)
        {
            cv::Vec3b temp_pix;
            
            double y, z;
            const double x = 1.0;
            Point2D src_pnt;
            
            y = normalize(
                    jj
                    , dst.rows
                    );

            z = normalize(
                    ii
                    , dst.cols
                    );

            src_pnt = getSphFromCart(
                    x
                    ,y
                    ,z
                    ,m_src.cols
                    ,m_src.rows
                    );

            temp_pix = m_src.at<cv::Vec3b>(src_pnt.y ,src_pnt.x);
            dst.at<cv::Vec3b>(ii,jj) = temp_pix;
        }
    }
    return dst;
}

cv::Mat genLeft(const cv::Mat& m_src)
{
    cv::Mat dst(m_src.rows/2, m_src.rows/2, m_src.type());
    //** Front tile
    for(int ii = 0; ii < dst.rows; ii++)
    {
        for(int jj = 0; jj < dst.cols; jj++)
        {
            cv::Vec3b temp_pix;
            
            double x, z;
            const double y = 1.0;
            Point2D src_pnt;
            
            x = normalize(
                    jj
                    , dst.rows
                    );

            z = normalize(
                    ii
                    , dst.cols
                    );

            src_pnt = getSphFromCart(
                    x
                    ,y
                    ,z
                    ,m_src.cols
                    ,m_src.rows
                    );

            temp_pix = m_src.at<cv::Vec3b>(src_pnt.y ,src_pnt.x);
            dst.at<cv::Vec3b>(ii,dst.cols - jj) = temp_pix;
        }
    }
    return dst;
}

cv::Mat genTop(const cv::Mat& m_src)
{

  cv::Mat dst(m_src.rows/2, m_src.rows/2, m_src.type());
  //** Front tile
  for(int ii = 0; ii < dst.rows; ii++)
  {
      for(int jj = 0; jj < dst.cols; jj++)
      {
 //           cv::Vec3b temp_pix;
 //           double x, z;
 //           const double y = 1.0;
 //           Point2D src_pnt;
 //           
 //           x = normalize(
 //                   jj
 //                   , dst.rows
 //                   );

 //           z = normalize(
 //                   ii
 //                   , dst.cols
 //                   );

 //           src_pnt = getSphFromCart(
 //                   x
 //                   ,y
 //                   ,z
 //                   ,m_src.cols
 //                   ,m_src.rows
 //                   );

 //           temp_pix = m_src.at<cv::Vec3b>(src_pnt.y ,src_pnt.x);
 //           dst.at<cv::Vec3b>(ii,dst.cols - jj) = temp_pix;
 //           

            cv::Vec3b temp_pix;
            
            double y, x;
            const double z = 1.0;
            Point2D src_pnt;
            
            y = normalize(
                    jj
                    , dst.rows
                    );
 
            x = normalize(
                    ii
                    , dst.cols
                    );
 
            src_pnt = getSphFromCart(
                    x
                    ,y
                    ,z
                    ,m_src.cols
                    ,m_src.rows
                    );
 
            temp_pix = m_src.at<cv::Vec3b>(src_pnt.y ,src_pnt.x);
            dst.at<cv::Vec3b>(dst.rows - ii -1,jj) = temp_pix;
//            dst.at<cv::Vec3b>(ii,dst.cols - jj) = temp_pix;
        }
    }
    return dst;
}

cv::Mat genRight(const cv::Mat& m_src)
{
    cv::Mat dst(m_src.rows/2, m_src.rows/2, m_src.type());
    //** Front tile
    for(int ii = 0; ii < dst.rows; ii++)
    {
        for(int jj = 0; jj < dst.cols; jj++)
        {
            cv::Vec3b temp_pix;
            
            double x, z;
            const double y = -1.0;
            Point2D src_pnt;
            
            x = normalize(
                    jj
                    , dst.rows
                    );

            z = normalize(
                    ii
                    , dst.cols
                    );

            src_pnt = getSphFromCart(
                    x
                    ,y
                    ,z
                    ,m_src.cols
                    ,m_src.rows
                    );

            temp_pix = m_src.at<cv::Vec3b>(src_pnt.y ,src_pnt.x);
            dst.at<cv::Vec3b>(ii, jj) = temp_pix;
        }
    }
    return dst;
}
