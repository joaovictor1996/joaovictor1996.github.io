#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, const char** argv){
	Mat image;
    Mat imagetrashold;
    Mat imagegray;
    Mat imagefingers;
    VideoCapture cam(0);

    char fingers_text[10];
    int count =0;
    
    cam.open(0);
  
	if(!cam.isOpened()){
    	cout << "unavailable cam" << endl;
    	return -1;
	}
    
    namedWindow("Thresholded_image",CV_WINDOW_AUTOSIZE);
    namedWindow("Fingers",CV_WINDOW_AUTOSIZE);

    while(1){
        bool b=cam.read(image);
        if(!b){
            cout<<"ERROR : cannot read"<<endl;
            return -1;
        }
        Rect fingers(0,0,450,450);//Selects the area
        imagefingers=image(fingers);
        cvtColor(imagefingers,imagegray,CV_RGB2GRAY);//Converts to a gray image

        GaussianBlur(imagegray,imagegray,Size(19,19),0.0,0);//Reduce noise
        threshold(imagegray,imagetrashold,0,255,THRESH_BINARY_INV+THRESH_OTSU);//Converts to a binary image

        vector<vector<Point> >contours;
        vector<Vec4i>hierarchy;
        findContours(imagetrashold,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,Point());//Finds contours in a image
        if(contours.size()>0){
        		size_t indexOfBiggestContour = -1;
	            size_t sizeOfBiggestContour = 0;

	            for (size_t i = 0; i < contours.size(); i++){
		        	if(contours[i].size() > sizeOfBiggestContour){
			    	sizeOfBiggestContour = contours[i].size();
			    	indexOfBiggestContour = i;
		        	}
                }
                vector<vector<int> >hull(contours.size());
                vector<vector<Point> >hullPoint(contours.size());
                vector<vector<Vec4i> >defects(contours.size());
                vector<vector<Point> >defectPoint(contours.size());
                for(size_t i=0;i<contours.size();i++){
                    if(contourArea(contours[i])>5000){
                        convexHull(contours[i],hull[i],true);//Finds the convex hull of a point set
                        convexityDefects(contours[i],hull[i],defects[i]);//Finds the convexity defects of a contour.
                        if(indexOfBiggestContour==i){
                            for(size_t k=0;k<hull[i].size();k++){
                                int ind=hull[i][k];
                                hullPoint[i].push_back(contours[i][ind]);
                            }
                            count =0;

                            for(size_t k=0;k<defects[i].size();k++){
                                if(defects[i][k][3]>3300){//Test to know the distance from the farthest point
                                    int p_end=defects[i][k][1];//end point
                                    int p_far=defects[i][k][2];//Farthest point,
                                    defectPoint[i].push_back(contours[i][p_far]);
                                    circle(imagefingers,contours[i][p_end],3,Scalar(0,255,0),2);//Draw circles at the top of the fingers
                                    count++;
                                }
                            }

                            if(count==1)//Shows the number of fingers
                                strcpy(fingers_text,"1 Finger");
                            else if(count==2)
                                strcpy(fingers_text,"2 Fingers");
                            else if(count==3)
                                strcpy(fingers_text,"3 Fingers");
                            else if(count==4)
                                strcpy(fingers_text,"4 Fingers");
                            else if(count==5)
                                strcpy(fingers_text,"5 Fingers");
                            else
                                strcpy(fingers_text,"0 Fingers");

                            putText(image,fingers_text,Point(0,70),CV_FONT_HERSHEY_SIMPLEX,3,Scalar(50,50,50),2,8,false);//Shows the message
                            drawContours(imagefingers, hullPoint, i, Scalar(0,0,255),2, 8, vector<Vec4i>(),0, Point() );//Draws a rectangle from the hull
                        }
                    }

                }
            imshow("Threshold image",imagetrashold);
            imshow("Fingers",imagefingers);
            waitKey(1);
        }
    }
     return 0;
}