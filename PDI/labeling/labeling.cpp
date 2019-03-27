#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char** ){
  Mat image, mask;
  int width, height;
  int nobjects, com_buracos;
  
  CvPoint p;
  image= imread("bolhas2.png",CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data) //teste para saber se a imagem foi realmente carregada
    cout << "Nao abriu o arquivo!" << endl;

  width=image.size().width;
  height=image.size().height;
  p.x=0;
  p.y=0;

  nobjects=0;
  com_buracos = 0;
  for(int i=0; i<height; i++){ //percorre as bordas verticais para eliminar as bolhas presentes nela
      if(image.at<uchar>(i,0) == 255){
		p.x=0;
		p.y=i;
		floodFill(image,p,0); //pinta a bolha com a cor do fundo
	  }
      if(image.at<uchar>(i,255) == 255){
		p.x=255;
		p.y=i;
		floodFill(image,p,0); //pinta a bolha com a cor do fundo
	  }
  }
  for(int y=0; y<width; y++){ //percorre as bordas horizontais para eliminar as bolhas presentes nela
      if(image.at<uchar>(0,y) == 255){
		p.x=y;
		p.y=0;
		floodFill(image,p,0); //pinta a bolha com a cor do fundo
	  }
      if(image.at<uchar>(255,y) == 255){
		p.x=y;
		p.y=255;
		floodFill(image,p,0); //pinta a bolha com a cor do fundo
	  }
  }
  for(int i=0; i<height; i++){ //percorre toda a imagem procurando pelas bolhas
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
		nobjects++; //incrementa a quantidade de bolhas
		p.x=j;
		p.y=i;
		floodFill(image,p,nobjects); //pinta a bolha com o valor do incremento
	  }
	}
  }
  
  p.x = 0;
  p.y = 0;
  floodFill(image,p,100); //pinta o fundo com o valor 100

  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 0 && image.at<uchar>(i-1,j) != 255){ //percorre a imagem procurando por pixels que possuem o valor igual a 0 e o pixel anterior diferente de 255
		com_buracos++;
		p.x=j;
		p.y=i;
		floodFill(image,p,100); //pinta o buraco com a cor do fundo
		p.x=j;
		p.y=i-1;
		floodFill(image,p,255); //pinta a bolha de branco
        }
	if(image.at<uchar>(i,j) == 0 && image.at<uchar>(i-1,j) == 255){
		p.x=j;
		p.y=i;
		floodFill(image,p,100); //pinta o buraco com a cor do fundo
        }
      }
  }

  cout << "Total de bolhas encontradas: " << nobjects << endl;
  cout << "Total de bolhas com buracos encontradas: " << com_buracos << endl;
  cout << "Total de bolhas sem bugacos encontradas: " << nobjects-com_buracos << endl;
  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
