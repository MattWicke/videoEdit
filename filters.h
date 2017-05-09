#pragma once
#include <iostream>
#include <unistd.h>
#include <vector>
#include <queue>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "types.h"
using namespace cv;
using namespace std;

//**TODO kill this shit
int extern erosion_elem;
int extern erosion_size;
int extern dilation_elem;
int extern dilation_size;
int extern max_elem;
int extern max_kernel_size;
Mat extern erosion_kern;

class ChannelSplitter
{
public:
   queue<Mat> red_list;
   queue<Mat> green_list;
   queue<Mat> blue_list;
   int red_depth;
   int green_depth;
   int blue_depth;

   int red_count;
   int green_count;
   int blue_count;
   bool first_set;

   ChannelSplitter(int n_red_depth, int n_green_dept, int n_blue_depth);
   void setColorDepth(int n_red_depth, int n_green_dept, int n_blue_depth);
   void process(Mat input, Mat& output);
};

void Erosion( int, void* );
void sum(Mat _a, Mat _b, Mat& _dst, double scale=1.0);
void trails(Mat _a, Mat _b, Mat& _dst, double scale=1.0);
void roll(Mat _a, Mat& _dst, int _frameRate, int _frameIndex, RollMode _rollMode);

//void Dilation( int, void* )
