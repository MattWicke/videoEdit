#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <unistd.h>
#include <vector>
#include <string>

class VideoWrapper
{
public:
    VideoWrapper(std::string m_fileName);
    void record(std::string m_fileName);
    int activeIndex;
    bool applyCrop;
    cv::Rect croproi;
    cv::VideoCapture vidCap;
    cv::Mat* getFramePtr(int index);
    int maxFrames;
private:
    std::vector<cv::Mat> frameVec;
    int ex;
    double fps;
    char ext[4];
    double frameDelayUS;
    cv::Size sz;
};
