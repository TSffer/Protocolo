#include <iostream>
#include <NTL/ZZ.h>
#include <string>
#include <time.h>
#include <sstream>
#include <fstream>
#include <regex>
#include <map>
#include <vector>
#include <math.h>
#include <CImg.h>
using namespace NTL;
using namespace std;
#include "funciones.h"
using namespace cimg_library;

int main()
{
    ofstream archPlano;
     archPlano.open("Mensaje Cifrado.txt",std::ifstream::binary);
    string rgb;
    CImg<unsigned char> src("img.png");
    int width = src.width();
    int height = src.height();
    cout << width << "x" << height << endl;
    for (int r = 0; r < height; r++)
        for (int c = 0; c < width; c++)
        {
            rgb += (ToString( conv<ZZ>((int)src(c,r,0,0)) ,3));
            rgb += (ToString( conv<ZZ>((int)src(c,r,0,1)) ,3));
            rgb += (ToString( conv<ZZ>((int)src(c,r,0,2)) ,3));
            /*cout << "(" << r << "," << c << ") ="
                 << " R  " << (int)src(c,r,0,0)
                 << " G  " << (int)src(c,r,0,1)
                 << " B  " << (int)src(c,r,0,2) << endl;
            */
        }
        cout<<"tam"<<rgb.size()<<endl;
        archPlano << rgb;
        archPlano.close();
    const unsigned char red[] = {255, 0, 0};
    CImg<unsigned char> src1(131,167);

    int width1 = src1.width();
    int height1 = src1.height();
    cout << width1 << "x" << height1 << endl;
    for (int r = 0; r < height1; r++)
        for (int c = 0; c < width1; c++)
        {
            int rr = 155;
            int g = 45;
            int b = 100;
             //unsigned char color[] = { 44, 15 ,156};
           src1(c,r,rr,g,b);

        }

        src1.display();


   // src.display();
    return 0;
}