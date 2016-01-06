#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "opencv2/highgui/highgui_c.h"

using namespace cv;
using namespace std;
Mat input;

int main(int argc, char** argv)
{
	if(argc!=2)
	{
         printf("Extra inputs!!! ERROR") ;
         return 0;
	}
input= imread(argv[1],0);
int i,j,k,zero_count,one_count,flag;
int thresh=320;
Mat thresholded;
Mat RLSA= Mat(input.size(),input.type(),Scalar(255));
///Thresholding
threshold(input,thresholded,150,255,THRESH_BINARY);
one_count=0;
///RLSA
for(i=0;i<thresholded.rows;i++)
{
  one_count=0;
  for(j=0;j<thresholded.cols;j++)
  {
    if(thresholded.at<uchar>(i,j)==0)
     {
      RLSA.at<uchar>(i,j)=0;
     }
     else
     {
        if((thresholded.at<uchar>(i,j+1)==0)&&(one_count<thresh))
         {
          for(k=j-one_count;k<j+1;k++)
          {
            RLSA.at<uchar>(i,k)=0;
          }
          one_count=0;

         }
         else if((one_count>=thresh)&&(thresholded.at<uchar>(i,j+1)==0))
          {
            one_count=0;
           //RLSA.at<uchar>(i,k)=255;
          }
         else 
         {

           one_count=one_count+1;
           //RLSA.at<uchar>(i,k)=255;
         }


     }


  }
}	
  ///RLSA ended
zero_count=0;
int ind=0;
for(i=0;i<RLSA.rows;i++)
{ind=0;
zero_count=0;
for(j=5;j<RLSA.cols;j++)
  {
    if(zero_count>5)
       {ind=1;
        break;}
    else if(RLSA.at<uchar>(i,j)==0)
     zero_count++;
  }
if(ind==1)
 {for(k=5;k<RLSA.cols-5;k++)
   {//if((dst.at<uchar>(i-1,k)==0)||(dst.at<uchar>(i+1,k)==0)) ///on uncommenting you will not get rectangle
       RLSA.at<uchar>(i,k)=0;}
 }
}

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
Mat drawing= Mat(input.size(),input.type(),Scalar(0));
findContours(RLSA, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0) );
Scalar color = Scalar(255);
for(i=0;i<contours.size();i++)
    {   drawContours( drawing, contours,i, color, CV_FILLED, 8, hierarchy, 0, Point() );
       normalize(drawing,drawing,0,1,NORM_MINMAX,-1,Mat());
drawing=drawing.mul(input);
namedWindow("Output",CV_WINDOW_NORMAL);
imshow("Output",drawing);
waitKey(1000);
drawing=Mat(input.size(),input.type(),Scalar(0));}
return 0;
}
