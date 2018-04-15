#include "controls.h"


//** used in the crop phase to figure out what side of the crop
//** rectangle the user clicked
Side getSideFromClick(
          const cv::Rect& croproi
        , const int mouseX
        , const int mouseY
        )
{
    Side side = NONE;
    int lineWidth = 10;
    int x1 = croproi.x;
    int x2 = croproi.x + croproi.width;
    int y1 = croproi.y;
    int y2 = croproi.y + croproi.height;

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
void changeCropRect(
         CropParams* crop_params
        , int mouseX
        , int mouseY
        )
{
    int x1 = crop_params->croproi.x;
    int x2 = crop_params->croproi.x + crop_params->croproi.width;
    int y1 = crop_params->croproi.y;
    int y2 = crop_params->croproi.y + crop_params->croproi.height;

    switch(crop_params->activeSide)
    {
        case LEFT:
        crop_params->croproi.x = mouseX;
        crop_params->croproi.width = abs(x2 - mouseX);
        break;

        case RIGHT:
        crop_params->croproi.width = mouseX - x1;
        break;

        case UP:
        crop_params->croproi.y = mouseY;
        crop_params->croproi.height = abs(y2 - mouseY);
        break;

        case DOWN:
        crop_params->croproi.height = mouseY - y1;
        break;
    }
}

//** used int the crop phase. Should only be called from the mouseCallback
void cropPhaseMouseFunction(
        int event
        , int mouseX
        , int mouseY
        , CropParams* crop_params
        )
{
    //StateMachine* sm = (StateMachine*)smPtr;
    switch(event)
    {
        case EVENT_LBUTTONDOWN:
            crop_params->drag = true;
            crop_params->activeSide = getSideFromClick(
                    crop_params->croproi
                    , mouseX
                    , mouseY
                    );
        break;

        case EVENT_LBUTTONUP:
            crop_params->drag = false;
            crop_params->activeSide = NONE;
        break;

        case EVENT_MOUSEMOVE:
        if(crop_params->drag)
        {
            changeCropRect( 
                    crop_params
                    , mouseX
                    , mouseY
                    );

        }
        break;
    }
}

cv::Rect centerCropRoi(
        cv::Rect croproi
        )
{
    cv::Rect out;
    if(croproi.width > croproi.height)
    {
        out.width = croproi.height;
        out.height = croproi.height;
        out.x = (croproi.width - croproi.height) / 2;
        out.y = 0;
    }
    else if(croproi.width > croproi.height)
    {
    }
    else
    {
    }
    return out;
}

//*****************************Callbacks*********************************
void mouseCallback(
        int event
        , int mouseX
        , int mouseY
        , int flags
        , void* cp
        )
{
    CropParams* crop_params = (CropParams*)cp;
    //if(sm->state == CROP)
    {
        cropPhaseMouseFunction(event, mouseX, mouseY, crop_params);
    }
}

void cropTrackCallback(int inVal, void* vw)
{
    VideoWrapper* video_wrapper_ptr = (VideoWrapper*)vw;
    cv::Mat displayFrame;

    video_wrapper_ptr->activeIndex = inVal;
    video_wrapper_ptr->getFrameActivePtr()->copyTo(displayFrame);

    cv::rectangle(displayFrame, video_wrapper_ptr->croproi, CV_RGB(255, 255, 0), 10);
    cv::imshow("crop", displayFrame);
}


//***********************************************************************
