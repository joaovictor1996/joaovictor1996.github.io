#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, equalizado;
  int width, height;
  VideoCapture cap;
  Mat histC, equa;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  cap.open(0);
  
  if(!cap.isOpened()){
    cout << "cameras indisponiveis" << endl;
    return -1;
  }
  
  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;
  
  int histw = nbins, histh = nbins/2;
  Mat histImg(histh, histw, CV_8U, Scalar(0));
  Mat histEqu(histh, histw, CV_8U, Scalar(0));

  //namedWindow("Histograma",WINDOW_AUTOSIZE);

  while(1){
    cap >> image;
	cvtColor(image,image,CV_BGR2GRAY);

	equalizeHist(image, equalizado);//equalizando a imagem

	calcHist(&image,1,0,Mat(),histC,1,&nbins,&histrange,uniform,acummulate);//criando o histograma da imagem origianl
	normalize(histC, histC, 0, histImg.rows, NORM_MINMAX, -1, Mat());
  	histImg.setTo(Scalar(0));

	calcHist(&equalizado,1,0,Mat(),equa,1,&nbins,&histrange,uniform,acummulate);//criando o histograma da imagem equalizada
	normalize(equa, equa, 0, histImg.rows, NORM_MINMAX, -1, Mat());
  	histEqu.setTo(Scalar(0));

	for(int i=0; i<nbins; i++){
      line(histImg,
           Point(i, histh),
           Point(i, histh-cvRound(histC.at<float>(i))),
           Scalar(255), 1, 8, 0);
      line(histEqu,
           Point(i, histh),
           Point(i, histh-cvRound(equa.at<float>(i))),
           Scalar(255), 1, 8, 0);
    }

	histImg.copyTo(image(Rect(0, 0,nbins, histh)));//copiando o histograma para a imagem original
	imshow("Original", image);

	histEqu.copyTo(equalizado(Rect(0, 0,nbins, histh)));//copiando o histograma para a imagem equalizada
    imshow("Equalizado", equalizado);
    waitKey(1);
  }
  return 0;
}
