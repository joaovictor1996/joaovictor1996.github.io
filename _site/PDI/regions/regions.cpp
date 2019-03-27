#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b val;
  int x1, x2, y1, y2; //coordenadas passadas pelo usuário

  image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE); 
  if(!image.data) //teste para saber se a imagem foi realmente carregada
    cout << "Nao abriu o arquivo!" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  cout << "Digite as coordenadas X1 e Y1, seguidos de enter\n";
  cin >> x1 >> y1;
  cout << "Digite as coordenadas X2 e Y2, seguidos de enter\n";
  cin >> x2 >> y2;

  for(int i=x1;i<x2;i++){
    for(int j=y1;j<y2;j++){
      image.at<uchar>(i,j)=255-image.at<uchar>(i,j); //substitui o valor do pixel pela subtração entre 255 e o valor dele
    }
  }

  imshow("janela", image);
  imwrite("regions.png", image);
  waitKey();
  return 0;
}
