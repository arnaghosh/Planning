#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int px=0,py=0;
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == cv::EVENT_LBUTTONDOWN )
     {
          px = x;
          py = y;
     }
}


void addObstacles(cv::Mat& img, vector<cv::Point>& obsCenter){
  for(int i=0;i<img.rows;i++){
    for(int j=0;j<img.cols;j++){
      if (i==0 || j==0 || i==img.rows-1 || j==img.cols-1){
        img.at<uchar>(i,j)=255;
      }
    }
  }
  cv::circle(img, cv::Point(img.cols/4,img.rows/4),20,255,-1);
  cv::circle(img, cv::Point(img.cols/2,img.rows/2),20,255,-1);
  cv::circle(img, cv::Point(img.cols/2,img.rows/4),20,255,-1);
  obsCenter.push_back(cv::Point(img.cols/4,img.rows/4));
  obsCenter.push_back(cv::Point(img.cols/2,img.rows/2));
  obsCenter.push_back(cv::Point(img.cols/2,img.rows/4));
}

void addTarget(cv::Mat& img, cv::Point target,vector<cv::Point> obsCenter){
  for(int i=0;i<img.rows;i++){
    for(int j=0;j<img.cols;j++){
      double r= 1.0*(int)img.at<uchar>(i,j);
      if(r==255)continue;
      double z = sqrt((target.x-j)*(target.x-j) + (target.y-i)*(target.y-i));
      if(z==0){
        img.at<uchar>(i,j)=0;
        continue;
      }
      r-= 2*(255.0/z);//*obsCenter.size();
      if(r>255)r=255;
      if(r<0)r=0;
      img.at<uchar>(i,j)=(int)r;
    }
  }
}

void addCostMap(cv::Mat& img, vector<cv::Point> obsCenter){
  for(int i=0;i<img.rows;i++){
    for(int j=0;j<img.cols;j++){
      int x = img.rows - i;
      int y = img.cols - j;
      if (i==0 || j==0 || x==0 || y==0)continue;
      double r = 0;
      int min = i;
      if(j<min)min=j;
      if(x<min)min=x;
      if(y<min)min=y;
      r = (255.0/min);
      int at_center = 0;
      for(int k=0;k<obsCenter.size();k++){
        double z = sqrt((obsCenter[k].x-j)*(obsCenter[k].x-j) + (obsCenter[k].y-i)*(obsCenter[k].y-i));
        if(z==0){
          at_center =1;
          break;
        }
        r+=(255.0/z);
      }
      r = 2*r;
      if(at_center)continue;
      if(r>255)r=255;
      if(r<0)r=0;
      if(r>(int)img.at<uchar>(i,j)){
        img.at<uchar>(i,j)=(int)r;
      }
    }
  }
}

int main(){
  cv::Mat im(500,500, CV_8UC1, cv::Scalar(0));
  vector<cv::Point> obstacleCenter;
  addObstacles(im, obstacleCenter);
  addCostMap(im, obstacleCenter);
  cv::imshow("map",im);
  cv::waitKey(0);
  cv::setMouseCallback("map",CallBackFunc,NULL);
  int saved = 0;
  while(1){
    int kb = cv::waitKey(15);
    if(kb==27)break;
    if(px ==0 && py==0)continue;
    cv::Mat im2 = im.clone();
    //cout<<px<<" "<<py<<endl;
    addTarget(im2,cv::Point(px,py),obstacleCenter);
    cv::circle(im2,cv::Point(px,py),2,255,-1);
    cv::imshow("map",im2);
    if (kb==115){
      saved++;
      cv::imwrite("costMap"+to_string(saved)+".jpg",im2);
    }
  }
  return 0;
}
