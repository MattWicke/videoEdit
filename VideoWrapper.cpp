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
        std::cout << "loading " << ii << "/" << maxFrames << std::endl;
        cv::Mat tempFrame;
        vidCap.read(tempFrame);
        if(tempFrame.rows > 0 && tempFrame.cols >0)
            frameVec.push_back(tempFrame);
        else
            std::cout << "WARNING: bad frame read" << ii << std::endl;
    }
    std::cout << "  Done Loading Video" << std::endl;
}

void VideoWrapper::record(std::string m_fileName)
{
    cv::VideoWriter vidWrite(m_fileName, CV_FOURCC('M','P','4','2'), fps, sz);

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
        std::cout << "crop " << ii << std::endl;
        cv::Mat temp;
        temp = cv::Mat(frameVec[ii](croproi));
        frameVec[ii] = temp;
    }
}
