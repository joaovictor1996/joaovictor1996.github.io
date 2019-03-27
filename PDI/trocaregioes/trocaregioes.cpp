#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image,copia;
  Vec3b val;
  int largura, altura;

  image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data) //teste para saber se a imagem foi realmente carregada
    cout << "Nao abriu o arquivo!" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  largura=image.size().width; //recebe o valor da largura
  altura=image.size().height; //recebe o valor da altura
  cout<<largura<<"\n"<<altura<<"\n";

  image.copyTo(copia); //copia a imagem original para uma cópia

  for(int i=0;i<largura/2;i++){ //transfere para a cópia os valores de cada quadrante da imagem original
    for(int j=0;j<altura/2;j++){
      copia.at<uchar>((largura/2)+i,(altura/2)+j)=image.at<uchar>(i,j);
      copia.at<uchar>(i,j)=image.at<uchar>((largura/2)+i,(altura/2)+j);
      copia.at<uchar>(i,(altura/2)+j)=image.at<uchar>((largura/2)+i,j);
      copia.at<uchar>((largura/2)+i,j)=image.at<uchar>(i,(altura/2)+j);
    }
  }

  imshow("janela", copia);
  imwrite("trocaregioes.png", copia);
  waitKey();
  return 0;
}
