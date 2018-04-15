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
#include "cmdParser.h"
#include "controls.h"



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

//*****************************Callbacks*********************************
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
                       , (void*)activeVideo
                       );
    char key = 0;
    CropParams crop_params;
    crop_params.croproi = activeVideo->croproi;

    cv::setMouseCallback("crop", mouseCallback, (void*)&crop_params);


    while(key != ' ')
    {
        if(key == 'c')
        {
            crop_params.croproi = centerCropRoi(activeVideo->croproi);
        }
        cv::Mat displayFrame;
        activeVideo->getFrameActivePtr()->copyTo(displayFrame);
        cv::rectangle(displayFrame, crop_params.croproi, CV_RGB(255, 255, 0), 10);
        cv::imshow("crop", displayFrame);
        key = waitKey(10);
    }
    cv::destroyWindow("crop");
    activeVideo->crop(crop_params.croproi);
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
    char retKey = 0;
    int picCount = 1;
    activeVideo->getFramePtr(0)->copyTo(store);
    activeVideo->getFramePtr(0)->copyTo(dst);

    if(parser.loops > 0)
        activeVideo->loopVideo(parser.loops);

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
                       , .1
                       );
              //  dst.copyTo(store);
              //  {
              //  cv::Mat temp;
              //  cv::bilateralFilter(dst, temp, 3, 10, 10);
              //  dst = temp;
              //  }
                dst.copyTo(store);
            break;

            case STACK:
                sum( *activeVideo->getFramePtr(ii)
                    , store 
                    , dst
                    , .5
                    );
                dst.copyTo(store);
                //{
                //cv::Mat temp;
                //cv::bilateralFilter(dst, temp, 5, 20, 20);
                //dst = temp;
                //}
            break;

            case ROLL:
                roll( *activeVideo->getFramePtr(ii)
                     , dst
                     , activeVideo->frameRate * 4
                     , ii
                     , parser.rollMode
                    );
            break;

            case KALE:
                kaleido2( *activeVideo->getFramePtr(ii)
                        , dst
                        );
            break;

            case NONE:
            //    activeVideo->getFramePtr(ii)->copyTo(dst);
            break;
        }
        dst.copyTo(*activeVideo->getFramePtr(ii));
        cv::imshow("play", *activeVideo->getFramePtr(ii));
        retKey = cv::waitKey(10);
        if(retKey = ' ')
        {
            std::string fileName;
            fileName = "snap" + picCount;
            fileName = fileName + ".jpg";
            //imwrite(fileName, *activeVideo->getFramePtr(ii));
            imwrite(fileName, dst);
        }
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
