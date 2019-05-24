#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 3
#define RAIO 3

int main(int argc, char** argv){
  vector<int> yrange;
  vector<int> xrange;

  Mat image, frame, points;

  int width, height, gray;
  int x, y;
  Mat blur, border;
  int threshold = 5;
  int ratio = 5;
  int kernel_size = 5;

  image= imread("image.jpg",CV_LOAD_IMAGE_GRAYSCALE);

  srand(time(0));

  width  = image.cols;
  height = image.rows;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  points = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());

  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = image.at<uchar>(x,y);
      //criando circulos na imagem
      circle(points,
             cv::Point(y,x),
             RAIO,
             CV_RGB(gray,gray,gray),
             -1,
             CV_AA);
    }
  }
  imwrite("pontos.jpg", points);

  //utilizando canny para saber onde estão as bordas
  Canny(image, border, threshold, ratio*threshold, kernel_size);
  imwrite("cannyborders.png", border);

  for (int j = 0; j< height; j++){
    for(int k = 0; k < width; k++){
      if(border.at<uchar>(j,k)>0){//fazendo um teste para saber se o pixel é uma borda
              gray = image.at<uchar>(j,k);
              circle(points, Point(k,j),RAIO,CV_RGB(gray,gray,gray),-1,CV_AA);//criando um circulo na posição da borda
              }
    }
  }
  imwrite("pontos_detalhes.jpg", points);
  return 0;
}
