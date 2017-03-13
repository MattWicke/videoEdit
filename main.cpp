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
    VideoWrapper* = activeVideo;
private:
    CmdParser parser;
    std::vector<VideoWrapper> vidWrappers;
};

void StateMachine::cropPhase()
{

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
void changeCropRect(videoWrapper* vidPtr 
                    , CropParams* cropParams
                    , int mouseX
                    , int mouseY)
{
}

//** used int the crop phase. Should only be called from the mouseCallback
void cropPhaseMouseFunction(int event, int mouseX, int mouseY, void* smPtr)
{
    StateMachine* sm = (StateMachine*)smPtr;
    CropParams* cropParams = &(sm->cropParams);
    switch(event)
    {
        case EVENT_LBUTTONDOWN:
            cropParams->drag = true;
            cropParams->side = getSideFromClick(sm->activeVideo, mouseX, mouseY);
        break;

        case EVENT_LBUTTONUP:
            cropParams->drag = false;
            cropParams->side = none
        break;

        case EVENT_MOUSEMOVE:
        if(cropParams->drag)
        {
        }
        break;
    }
}

//*****************************Callbacks*********************************
void mouseCallback(int event, int mouseX, int mouseY, int, void* smPtr)
{
    StateMachine* sm = (StateMachine*)smPtr;
    if(sm->state == CROP)
    {
        cropPhaseMouseFunction(event, mouseX, mouseY, smPtr);
    }
}
//***********************************************************************

int main(int argc, char* argv[])
{
    CmdParser parser(argc, argv);
    VideoWrapper vw(parser[1]);
    vw.record(parser[2]);
}
