#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "types.h"
#include "VideoWrapper.h"

struct CropParams
{
    bool drag;
    Side activeSide;
    cv::Rect croproi;
};

//** used in the crop phase to figure out what side of the crop
//** rectangle the user clicked
Side getSideFromClick(
          const cv::Rect& croproi
        , const int mouseX
        , const int mouseY
        );

//** used in the crop phase to change the dimensions of the crop
//** rectangle when the user is dragging it
void changeCropRect(
         CropParams* cropParams
        , int mouseX
        , int mouseY
        );

//** used int the crop phase. Should only be called from the mouseCallback
void cropPhaseMouseFunction(
        int event
        , int mouseX
        , int mouseY
        , CropParams &cropParams
        , cv::Rect& croproi
        );

cv::Rect centerCropRoi(
        cv::Rect croproi
        );

//*****************************Callbacks*********************************

void mouseCallback(
        int event
        , int mouseX
        , int mouseY
        , int flags
        , void* cp //** cast to a CropParams
        );

void cropTrackCallback(
        int inVal
        , void* vw //** cast to a VideoWrapper

        );

void pSTrackCallback(
        int inVal
        , void* smPtr
        );

void pSCallback(
        int
        , void* smPtr
        );
//***********************************************************************
