#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <unistd.h>
#include <vector>
#include <string>
#include "VideoWrapper.h"
#include "types.h"

//************************************cmdParser***************************************
class CmdParser
{
public:
    std::vector<std::string> args;
    CmdParser(int m_argc, char* m_argv[]);
    std::string operator[](size_t ii);
};

CmdParser::CmdParser(int m_argc, char* m_argv[])
{
    for(int ii = 0; ii < m_argc; ii++)
    {
        args.push_back(std::string(m_argv[ii]));
    }
}

std::string CmdParser::operator[](size_t ii)
{
    return args[ii];
}
//************************************************************************************

struct CropParams
{
    bool drag;
    Side activeSide;
};

class StateMachine
{
public:
    State state;
    void cropPhase();
    CropParams cropParams;
    VideoWrapper* activeVideo;
    void loadVideo(std::string fileName);
    ~StateMachine();
    bool setActiveVideo(int index);
private:
    //CmdParser parser;
    std::vector<VideoWrapper*> vidWrappers;
};

StateMachine::~StateMachine()
{
    for(int ii = 0; ii < vidWrappers.size(); ii++)
        delete vidWrappers[ii];
}


void StateMachine::loadVideo(std::string fileName)
{
    VideoWrapper* newVideo = new VideoWrapper(fileName);
    vidWrappers.push_back(newVideo);
    activeVideo = newVideo;
}

//** used in the crop phase to figure out what side of the crop
//** rectangle the user clicked
Side getSideFromClick(VideoWrapper* vidPtr, int mouseX, int mouseY)
{
    Side side = NONE;
    int lineWidth = 10;
    int x1 = vidPtr->croproi.x;
    int x2 = vidPtr->croproi.x + vidPtr->croproi.width;
    int y1 = vidPtr->croproi.y;
    int y2 = vidPtr->croproi.y + vidPtr->croproi.height;

    if( abs(mouseX - x1) < lineWidth)
        side = LEFT;
    if( abs(mouseX - x2) < lineWidth)
        side = RIGHT;
    if( abs(mouseY - y1) < lineWidth)
        side = UP;
    if( abs(mouseY - y2) < lineWidth)
        side = DOWN;

    return side;
}

//** used in the crop phase to change the dimensions of the crop
//** rectangle when the user is dragging it
void changeCropRect(VideoWrapper* vidPtr 
                    , CropParams* cropParams
                    , int mouseX
                    , int mouseY)
{
    int x1 = vidPtr->croproi.x;
    int x2 = vidPtr->croproi.x + vidPtr->croproi.width;
    int y1 = vidPtr->croproi.y;
    int y2 = vidPtr->croproi.y + vidPtr->croproi.height;

    switch(cropParams->activeSide)
    {
        case LEFT:
        vidPtr->croproi.x = mouseX;
        vidPtr->croproi.width = abs(x2 - mouseX);
        break;

        case RIGHT:
        vidPtr->croproi.width = mouseX - x1;
        break;

        case UP:
        vidPtr->croproi.y = mouseY;
        vidPtr->croproi.height = abs(y2 - mouseY);
        break;

        case DOWN:
        vidPtr->croproi.height = mouseY - y1;
        break;
    }
}

//** used int the crop phase. Should only be called from the mouseCallback
void cropPhaseMouseFunction(int event, int mouseX, int mouseY, void* smPtr)
{
    StateMachine* sm = (StateMachine*)smPtr;
    switch(event)
    {
        case EVENT_LBUTTONDOWN:
            sm->cropParams.drag = true;
            sm->cropParams.activeSide = getSideFromClick(sm->activeVideo, mouseX, mouseY);
        break;

        case EVENT_LBUTTONUP:
            sm->cropParams.drag = false;
            sm->cropParams.activeSide = NONE;
        break;

        case EVENT_MOUSEMOVE:
        if(sm->cropParams.drag)
        {
            changeCropRect( sm->activeVideo 
                           , &sm->cropParams
                           , mouseX
                           , mouseY
                           );

        }
        break;
    }
}

//*****************************Callbacks*********************************
void mouseCallback(int event, int mouseX, int mouseY, int, void* smPtr)
{
    StateMachine* sm = (StateMachine*)smPtr;
    //if(sm->state == CROP)
    {
        cropPhaseMouseFunction(event, mouseX, mouseY, smPtr);
    }
}

void cropTrackCallback(int inVal, void* smPtr)
{
    StateMachine* sm = (StateMachine*)smPtr;
    sm->activeVideo->activeIndex = inVal;
        cv::Mat displayFrame;
        sm->activeVideo->getFrameActivePtr()->copyTo(displayFrame);
        cv::rectangle(displayFrame, sm->activeVideo->croproi, CV_RGB(255, 255, 0), 10);
        cv::imshow("crop", displayFrame);
}

//***********************************************************************

void StateMachine::cropPhase()
{
    cv::Mat* framePtr = activeVideo->getFramePtr(0);
    cv::namedWindow("crop", CV_WINDOW_NORMAL);
    cv::imshow("crop", *activeVideo->getFrameActivePtr());
    cv::createTrackbar(  "track"
                       , "crop"
                       , &(activeVideo->activeIndex)
                       , activeVideo->maxFrames -1
                       , cropTrackCallback
                       , this
                       );
    char key = 0;

    cv::setMouseCallback("crop", mouseCallback, this);


    while(key != ' ')
    {
        cv::Mat displayFrame;
        activeVideo->getFrameActivePtr()->copyTo(displayFrame);
        cv::rectangle(displayFrame, activeVideo->croproi, CV_RGB(255, 255, 0), 10);
        cv::imshow("crop", displayFrame);
        key = waitKey(10);
    }

    cv::waitKey(0);
}

int main(int argc, char* argv[])
{
    CmdParser parser(argc, argv);
    StateMachine sm;
    sm.loadVideo("splash.avi");
    sm.cropPhase();
}
