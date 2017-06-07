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
#include "filters.h"
#include "pixelSort/pixelSort.h"

//************************************cmdParser***************************************
enum EffectMode {NO, RGB_DELAY, PIXEL_SORT, STACK, TRAILS, ROLL};
enum PSortMode {HORZ, VERT};
class CmdParser
{
public:
    std::vector<std::string> args;
    CmdParser(int m_argc, char* m_argv[]);
    std::string operator[](size_t ii);
    EffectMode eMode;
    PSortMode psMode;
    void printHelp();

private:
    void assignParams();
    void assignMode(std::string inParam);
    void sortDirFromUser();
};

CmdParser::CmdParser(int m_argc, char* m_argv[])
{
    for(int ii = 0; ii < m_argc; ii++)
    {
        args.push_back(std::string(m_argv[ii]));
    }
    assignParams();
}

void CmdParser::assignParams()
{
    int ii = 0;
    while(ii < args.size())
    {
        std::cout << ii << std::endl;
        if(args[ii] == "-m")
        {
            ii++;
            assignMode(args[ii]);
        }
        ii++;
    }
}

void CmdParser::assignMode(std::string inParam)
{
    if(inParam == "rgb")
    {
        eMode = RGB_DELAY;
        std::cout << "Mode set to RGB Delay" << std::endl;
    }

    if(inParam == "srt")
    {
        eMode = PIXEL_SORT;
        sortDirFromUser();
        std::cout << "Mode set to Pixel Sort" << std::endl;
    }

    if(inParam == "sta")
    {
        eMode = STACK;
        std::cout << "Mode set to Stack" << std::endl;
    }

    if(inParam == "tra")
    {
        eMode = TRAILS;
        std::cout << "Mode set to Trails" << std::endl;
    }

    if(inParam == "rol")
    {
        eMode = ROLL;
        std::cout << "Mode set to Roll" << std::endl;
    }

    if(inParam == "non")
    {
        eMode = NO;
        std::cout << "Mode set to none" << std::endl;
    }
}

void CmdParser::printHelp()
{
    std::cout << "vidEdit [fileName.mp4] -m [filter] " << std::endl;
    std::cout << " [rgb] RGB delay filter" << std::endl;
    std::cout << " [srt] pixel sorting filter" << std::endl;
    std::cout << " [sta] stack frames filter" << std::endl;
    std::cout << " [tra] trails filter" << std::endl;
}

void CmdParser::sortDirFromUser()
{
    char input = 0;

    while((input  != 'h')&&(input  != 'v'))
    {
        std::cout << "Enter a direction" << std::endl;
        std::cout << " horizontal h" << std::endl;
        std::cout << " vertical v" << std::endl;
        std::cin >> input;
    }

    switch(input)
    {
        case 'h':
        psMode = HORZ;
        break;

        case 'v':
        psMode = VERT;
        break;
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
    //**variables
    State state;
    CmdParser &parser;
    CropParams cropParams;
    VideoWrapper* activeVideo;
    ChannelSplitter* channelSplitter;
    CannyParams cparams;

    //**functions
    StateMachine(CmdParser &inparser);
    ~StateMachine();
    void cropPhase();
    void loadVideo(std::string fileName);
    bool setActiveVideo(int index);
    void processVideo();
    void pSortTuningPhase();

private:
    std::vector<VideoWrapper*> vidWrappers;
};

StateMachine::StateMachine(CmdParser &inparser):
    parser(inparser)
{
    channelSplitter = new ChannelSplitter(0,5,10);
}

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

void pSTrackCallback(int inVal, void* smPtr)
{
    StateMachine* sm = (StateMachine*)smPtr;
    sm->activeVideo->activeIndex = inVal;
    sm->cparams.edges = cannyEdgeDetect(sm->cparams.source,
                                        sm->cparams.lowThreshold);
    cv::imshow("edges", sm->cparams.edges);
}

void pSCallback(int, void* smPtr)
{
    StateMachine* sm = (StateMachine*)smPtr;
    sm->cparams.edges = cannyEdgeDetect(sm->cparams.source,
                                        sm->cparams.lowThreshold);
    cv::imshow("edges", sm->cparams.edges);
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
    cv::destroyWindow("crop");
    activeVideo->crop();
}

void StateMachine::pSortTuningPhase()
{
    cv::Mat* framePtr = activeVideo->getFramePtr(0);
    cv::namedWindow("edges", CV_WINDOW_NORMAL);
    cv::imshow("edges", *activeVideo->getFrameActivePtr());
    cv::createTrackbar(  "pose"
                       , "edges"
                       , &(activeVideo->activeIndex)
                       , activeVideo->maxFrames -1
                       , pSTrackCallback
                       , this
                       );

    cv::createTrackbar(  "canny"
                       , "edges"
                       , &cparams.lowThreshold
                       , 100
                       , pSCallback
                       , this
                       );

    char key = 0;
    while(key != ' ')
    {
        activeVideo->getFrameActivePtr()->copyTo(cparams.source);
        cparams.edges = cannyEdgeDetect(cparams.source, cparams.lowThreshold);
        
        //cv::imshow("edges", cparams.edges);
        key = waitKey(10);
    }
    cv::destroyWindow("edges");
}

void StateMachine::processVideo()
{
    cv::namedWindow("play", CV_WINDOW_NORMAL);
    cv::Mat dst;
    cv::Mat store;
    activeVideo->getFramePtr(0)->copyTo(store);
    activeVideo->getFramePtr(0)->copyTo(dst);

    for(int ii = activeVideo->activeIndex; ii < activeVideo->loadedFrames; ii++)
    {
        
        switch(parser.eMode)
        {
            case RGB_DELAY:
                channelSplitter->process(*activeVideo->getFramePtr(ii), dst);
            break;

            case PIXEL_SORT:
                switch(parser.psMode)
                    {
                        case VERT:
                            dst = cannySortColumn(*activeVideo->getFramePtr(ii),
                                                    cparams.lowThreshold);
                        break;

                        case HORZ:
                            dst = cannySortRow(*activeVideo->getFramePtr(ii),
                                                 cparams.lowThreshold);
                        break;
                    }
            break;

            case TRAILS: 
                trails(store
                       , *activeVideo->getFramePtr(ii)
                       , dst
                       );
                dst.copyTo(store);
            break;

            case STACK:
                sum( *activeVideo->getFramePtr(ii)
                    , store 
                    , dst
                    );
                dst.copyTo(store);
                store = store/2;
            break;

            case NONE:
                activeVideo->getFramePtr(ii)->copyTo(dst);
            break;
        }
        dst.copyTo(*activeVideo->getFramePtr(ii));
        cv::imshow("play", *activeVideo->getFramePtr(ii));
        cv::waitKey(1);
    }
    activeVideo->record("output.avi");
    std::cout << "video recorded" << std::endl;
}


int main(int argc, char* argv[])
{
    CmdParser parser(argc, argv);

    if(argc < 2)
    {
        parser.printHelp();
        return 0;
    }
    StateMachine sm(parser);
    sm.loadVideo(parser[1]);
    sm.cropPhase();
    if(sm.parser.eMode == PIXEL_SORT)
        sm.pSortTuningPhase();
    sm.processVideo();
}
