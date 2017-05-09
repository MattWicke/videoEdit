#include "filters.h"
#include "types.h"

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int max_elem = 2;
int max_kernel_size = 21;
Mat erosion_kern;


ChannelSplitter::ChannelSplitter(int n_red_depth, int n_green_depth, int n_blue_depth):
    red_depth(n_red_depth), 
    green_depth(n_green_depth), 
    blue_depth(n_blue_depth), 
    red_count(1), 
    green_count(1), 
    blue_count(1) ,
    first_set(false)
{
}

void ChannelSplitter::setColorDepth(int n_red_depth, int n_green_depth, int n_blue_depth)
{
    red_depth = n_red_depth;
    green_depth = n_green_depth;
    blue_depth = n_blue_depth;
}

void ChannelSplitter::process(Mat input, Mat& output)
{
    Mat channel_array_target[3];
    Mat channel_array_out[3];
    static Mat channel_array_first[3];
    split(input, channel_array_target);

    if(!first_set)
    {
        //channel_array_target[0].copyTo(channel_array_first[0]);
        channel_array_target[1].copyTo(channel_array_first[1]);
        channel_array_target[2].copyTo(channel_array_first[2]);
        first_set = true;
    }
    //if(red_count > red_depth)
    //{
        red_list.push(channel_array_target[0]);
        channel_array_out[0] = (red_list.front());
    //}
    if(green_count > green_depth)
    {
        green_list.push(channel_array_target[1]);
        channel_array_out[1] = (green_list.front());
    }
    else
    {
        Mat temp_mat = Mat::zeros(input.rows, input.cols, CV_8UC1);
        //green_list.push(temp_mat);
        green_list.push(channel_array_first[1]);
        channel_array_out[1] = (green_list.front());
    }

    if(blue_count > blue_depth)
    {
        blue_list.push(channel_array_target[2]);
        channel_array_out[2] = (blue_list.front());
    }
    else
    {
        Mat temp_mat = Mat::zeros(input.rows, input.cols, CV_8UC1);
        //blue_list.push(temp_mat);
        blue_list.push(channel_array_first[2]);
        channel_array_out[2] = (blue_list.front());
    }

    if(red_list.size() > red_depth)
        red_list.pop();
    if(green_list.size() > green_depth)
        green_list.pop();
    if(blue_list.size() > blue_depth)
        blue_list.pop();
   
   red_count++;
   green_count++;
   blue_count++;
   merge(channel_array_out, 3, output);
}

void sum(Mat _a, Mat _b, Mat& _dst, double scale)
{
   for( int m = 0; m < _dst.rows; m++)
   {
      for(int n = 0; n < _dst.cols; n++)
      {
         Pixel tempElem;
         tempElem.b = 0;
         tempElem.g = 0;
         tempElem.r = 0;
         if(m < _a.rows && n < _a.cols)
         {
            tempElem.b += _a.at<Pixel>(m,n).b;
            tempElem.g += _a.at<Pixel>(m,n).g;
            tempElem.r += _a.at<Pixel>(m,n).r;
         }
         if(m < _b.rows && n < _b.cols)
         {
            tempElem.b +=(int) (_b.at<Pixel>(m,n).b * scale);
            tempElem.g +=(int) (_b.at<Pixel>(m,n).g * scale);
            tempElem.r +=(int) (_b.at<Pixel>(m,n).r * scale);
         }
         _dst.at<Pixel>(m,n) = tempElem;
      }
   }
}

void trails(Mat _a, Mat _b, Mat& _dst, double scale)
{
   for( int m = 0; m < _dst.rows; m++)
   {
      for(int n = 0; n < _dst.cols; n++)
      {
         Pixel tempElem;
         tempElem.b = 0;
         tempElem.g = 0;
         tempElem.r = 0;
         if((m < _a.rows && n < _a.cols)&&(m < _b.rows && n < _b.cols))
         {
//            tempElem.b = _a.at<Pixel>(m,n).b + abs(_b.at<Pixel>(m,n).b - _a.at<Pixel>(m,n).b)/3;
//            tempElem.g = _a.at<Pixel>(m,n).g + abs(_b.at<Pixel>(m,n).g - _a.at<Pixel>(m,n).g)/3;
//            tempElem.r = _a.at<Pixel>(m,n).r + abs(_b.at<Pixel>(m,n).r - _a.at<Pixel>(m,n).r)/3;
            tempElem.b = _a.at<Pixel>(m,n).b + abs(_b.at<Pixel>(m,n).b - _a.at<Pixel>(m,n).b)*_b.at<Pixel>(m,n).b/255;
            tempElem.g = _a.at<Pixel>(m,n).g + abs(_b.at<Pixel>(m,n).g - _a.at<Pixel>(m,n).g)*_b.at<Pixel>(m,n).g/255;
            tempElem.r = _a.at<Pixel>(m,n).r + abs(_b.at<Pixel>(m,n).r - _a.at<Pixel>(m,n).r)*_b.at<Pixel>(m,n).r/255;
         }
         _dst.at<Pixel>(m,n) = tempElem;
      }
   }
}


void roll(Mat _a, Mat& _dst, int _frameRate, int _frameIndex, RollMode _rollMode)
{
//   cout << "barrier " << barrier << endl;
//   line(_dst, Point(0, barrier), Point(_dst.cols, barrier), CV_RGB(0,0,255), 5);

   if( _rollMode == RM_DOWN)
   {
      int barrier = _a.rows * (_frameIndex % _frameRate)/_frameRate;
      _a(Rect(0, 0, _a.cols, _a.rows - barrier)).copyTo(_dst(Rect(0, barrier, _a.cols, _a.rows - barrier)));
      if(barrier >0)
         _a(Rect(0, _a.rows - barrier, _a.cols, barrier )).copyTo(_dst(Rect(0, 0, _a.cols, barrier )));
   }
   if( _rollMode == RM_UP)
   {
      int barrier = _a.rows * (_frameIndex % _frameRate)/_frameRate;
      barrier = _a.rows - barrier;
      line(_dst, Point(0, barrier), Point(_dst.cols, barrier), CV_RGB(0,0,255), 5);
      cout << "barrier " << barrier << endl;
      _a(Rect(0, _a.rows - barrier, _a.cols, barrier)).copyTo(_dst(Rect(0, 0, _a.cols, barrier)));
      if(barrier < _a.rows -1)
         _a(Rect(0, 0, _a.cols, _a.rows - barrier )).copyTo(_dst(Rect(0, barrier, _a.cols, _a.rows - barrier )));
   }
   if( _rollMode == RM_LEFT)
   {
      int barrier = _a.cols * (_frameIndex % _frameRate)/_frameRate;
      _a(Rect(0, 0, _a.cols - barrier, _a.rows )).copyTo(_dst(Rect(barrier, 0, _a.cols - barrier, _a.rows)));
      if(barrier >0)
         _a(Rect(_a.cols - barrier, 0, barrier , _a.rows )).copyTo(_dst(Rect(0, 0, barrier, _a.rows )));
   }
   if( _rollMode == RM_RIGHT)
   {
      int barrier = _a.cols * (_frameIndex % _frameRate)/_frameRate;
      barrier = _a.cols - barrier;
      _a(Rect(0, 0, _a.cols - barrier, _a.rows )).copyTo(_dst(Rect(barrier, 0, _a.cols - barrier, _a.rows)));
      if(barrier >0)
         _a(Rect(_a.cols - barrier, 0, barrier , _a.rows )).copyTo(_dst(Rect(0, 0, barrier, _a.rows )));
   }
}

void Erosion( int, void* )
{
   int erosion_type;
   if( erosion_elem == 0)
   {
      erosion_type = MORPH_RECT;
   }
   else if( erosion_elem == 1 )
   {
      erosion_type = MORPH_CROSS;
   }
   else if( erosion_elem == 2 )
   {
      erosion_type = MORPH_ELLIPSE;
   }

   erosion_kern = getStructuringElement( erosion_type,
                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                        Point( erosion_size, erosion_size) );

   //***** Apply the erosion operation
//   erode( src,  erosion_dst, element );
//   imshow( "Erosion Demo", erosion_dst );
}

//void Dilation( int, void* )
//{
//   int dilation_type;
//   if( dilation_elem == 0 )
//   {
//      dilation_type = MORPH_RECT;
//   }
//   else if( dilation_elem == 1 )
//   {
//      dilation_type = MORPH_CROSS;
//   }
//   else if( dilation_elem == 2 )
//   {
//      dilation_type = MORPH_ELLIPSE;
//   }
//
//   Mat element = getStructuringElement( dilation_type,
//                                        Size( 2*dilation_size+1, 2*dilation_size+1),
//                                        Point( dilation_size, dilation_size ));
//
//   //Apply the dilation operation
//   dilate( src, dilation_dst, element );
//   //imshow( "Dilation Demo", dilation_dst );
//}
