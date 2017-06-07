#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

enum Side{ NONE = 0, LEFT, RIGHT, UP, DOWN};
enum State{ START, CROP, PAUSE, RECORD, PLAY, PLAY_AND_CROP, QUIT};
//enum Mode{ NOFX, ERODE, ADD , STACK, AVERAGE, TRAILS, ROLL, CHANNELS}; 
enum RollMode{RM_DOWN, RM_UP, RM_LEFT, RM_RIGHT}; 

struct Pixel
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
};
