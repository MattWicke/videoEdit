#include "VideoWrapper.h"

VideoWrapper::VideoWrapper(std::string m_fileName) :
    activeIndex(0)
{
    vidCap.open(m_fileName);

    //** set up basic video properties
    sz.width = vidCap.get(CV_CAP_PROP_FRAME_WIDTH);
    sz.height = vidCap.get(CV_CAP_PROP_FRAME_HEIGHT);
    ex = static_cast<int>(vidCap.get(CV_CAP_PROP_FOURCC));
    fps = vidCap.get(CV_CAP_PROP_FPS);
    maxFrames = vidCap.get(CV_CAP_PROP_FRAME_COUNT);
    ext[0] = (char)(ex & 0XFF); 
    ext[1] = (char)((ex & 0XFF00) >> 8);
    ext[2] = (char)((ex & 0XFF0000) >> 16);
    ext[3] = (char)((ex & 0XFF000000) >> 24);
    double frameDelayUS = (1.0 / fps) * 1000000.0; 
    croproi.x = 0;
    croproi.y = 0;
    croproi.width = sz.width;
    croproi.height = sz.height;

    //** report properties
    std::cout << "New VideoWrapper created:" << std::endl;
    std::cout << "  fourcc " << ex << std::endl;
    std::cout << "  codec type " << ext << std::endl;
    std::cout << "  fps " << fps << std::endl;
    std::cout << "  frame delay " << frameDelayUS << std::endl;
    
    //** load all video into memory
    for(int ii = 0; ii < maxFrames; ii++)
    {
        cv::Mat tempFrame;
        vidCap.read(tempFrame);
        frameVec.push_back(tempFrame);
    }
}

void VideoWrapper::record(std::string m_fileName)
{
    cv::VideoWriter vidWrite(m_fileName, ex, fps, sz);

    std::cout << "Beginning video write" << std::endl;
    for(int ii = 0; ii < frameVec.size(); ii++)
    {
        vidWrite.write(frameVec[ii]);
        usleep(frameDelayUS);
    }
    std::cout << "Video write complete" << std::endl;
}

cv::Mat* VideoWrapper::getFramePtr(int index)
{
    return &frameVec[index];
}

cv::Mat* VideoWrapper::getFrameActivePtr()
{
    return &frameVec[activeIndex];
}

void VideoWrapper::crop()
{
    for(int ii = 0; ii < frameVec.size(); ii++)
    {
        cv::Mat temp(frameVec[ii](croproi));
        frameVec[ii] = temp;
    }
}
