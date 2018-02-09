#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <queue>
#include <NTL/ZZ.h>
#include <string>
#include <time.h>
#include <sstream>
#include <fstream>
#include <regex>
#include <map>
#include <vector>
#include <math.h>
using namespace NTL;
using namespace cv;
using namespace std;
#include "funciones.h"



void recuperar(string C)
{
    Mat src(32,32,CV_8UC3);
    int k =0,v;
    for(int i=0;i<32;i++)
    {
        for(int j=0;j<32;j++)
        {
            int cont =0;
            v = 0;
            for(int a = k ;cont<9;a+=3)
            {
                string st(C,a,3);
                src.at<Vec3b>(i,j)[v] = stoi(st);
                v++;
                cont+=3;
            }
            k+=9;
        } 
    }
    cout<<"dibujando"<<endl;
    namedWindow("Display window" , CV_WINDOW_AUTOSIZE);
    imshow("Display window" , src);
    waitKey(0);
    destroyWindow("Display window");
    cout<<"Dibujo terminado"<<endl;
}


int main()
{
    Mat src = imread("img2.jpg",CV_LOAD_IMAGE_COLOR);
    if(src.empty())
    {
        cout<<"Error : La Imagen no  se cargo........."<<endl;
        return 0;
    }

    ofstream archPlano;
    archPlano.open("RGB.txt",std::ifstream::binary);
    string rgb;

    queue<Vec3b> v;

    for(int i=0;i<src.rows;i++)
    {
        for(int j=0;j<src.cols;j++)
        {
            v.push((src.at<Vec3b>(i,j)));
            rgb +=ToString( (ZZ)src.at<Vec3b>(i,j)[0],3 );
            rgb +=ToString( (ZZ)src.at<Vec3b>(i,j)[1],3 );
            rgb +=ToString( (ZZ)src.at<Vec3b>(i,j)[2],3 );
            //cout<<src.at<Vec3b>(i,j)<<endl;
            //rgb += ToString(conv<ZZ>(src(i,j)[0]));
        }
    }
    archPlano << rgb;
    archPlano.close();
/*
    namedWindow("Display window" , CV_WINDOW_AUTOSIZE);
    imshow("Display window" , src);
    waitKey(0);
    destroyWindow("Display window");*/


    //RECUPERACION DE LA IMAGEN
    ifstream archRGB;
    string ARGB;

    archRGB.open("Recuperado.txt",ifstream::binary);
    getline(archRGB,ARGB);
    recuperar(ARGB);

    queue<Vec3b> vv;
    //llenarVec(ARGB,vv);


    //IMAGEN RECUPERADA

    Mat imgenRe(src.rows,src.cols,CV_8UC3);

    for(int i=0;i<imgenRe.rows;i++)
    {
        for(int j=0;j<imgenRe.cols;j++)
        {
            imgenRe.at<Vec3b>(Point(j,i)) = v.front();
            v.pop();
        }
    }

/*
    namedWindow("Display window1" , CV_WINDOW_AUTOSIZE);
    imshow("Display window1" , imgenRe);
    waitKey(0);
    destroyWindow("Display window1");*/
    int a;
    
    cin>>a;
    if(a == 0)
    {
        return 0;
    }
     return 0;   
}
