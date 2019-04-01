#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image1, image2, cinza1, cinza2;
  int width, height;
  VideoCapture cap;
  Mat hist1, hist2;
  int nbins = 64;
  float range[] = {0, 256}, tolerancia = 2, comparacao=0;
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
  Mat histImage(histh, histw, CV_8U, Scalar(0));

  while(1){
	cap >> image1;//captura a primeira imagem
	cvtColor(image1,cinza1,CV_BGR2GRAY);

	calcHist(&cinza1,1,0,Mat(),hist1,1,&nbins,&histrange,uniform,acummulate);//calcula o histograma da primeira imagem
	normalize(hist1, hist1, 0, histImage.rows, NORM_MINMAX, -1, Mat());
  	histImage.setTo(Scalar(0));

	cap >> image2;//captura a segunda imagem
	cvtColor(image2,cinza2,CV_BGR2GRAY);
	
	calcHist(&cinza2,1,0,Mat(),hist2,1,&nbins,&histrange,uniform,acummulate);//calcula o histograma da segunda imagem
	normalize(hist1, hist1, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	comparacao=compareHist(hist2,hist1,CV_COMP_BHATTACHARYYA);//compara os dois histogramas 

	if(comparacao*100>tolerancia){//compara para saber se existe movimento
		cout << "Movimento!!" << endl;
	}
	else{
		cout << comparacao << endl;
	}

	for(int i=0; i<nbins; i++){
      line(histImage,
           Point(i, histh),
           Point(i, histh-cvRound(hist1.at<float>(i))),
           Scalar(255), 1, 8, 0);
    }

	histImage.copyTo(cinza1(Rect(0, 0,nbins, histh)));
	imshow("Camera", cinza1);
	waitKey(1);
  }
  return 0;
}