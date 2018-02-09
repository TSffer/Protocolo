#include <iostream>
#include <NTL/ZZ.h>
#include <string>
#include <time.h>
#include <sstream>
#include <fstream>
#include <regex>
#include <map>
#include <math.h>
#include "FirmaDigital.cpp"
using namespace NTL;
using namespace std;



int main()
{
    string r1 , r2 ,r3;
    FirmaDigital A;
    string mensaje , firma;
    mensaje = "hola como estas";
    firma = "luis";
    A.generarClaves(1024);
    ZZ	np = stoZZ (get_file_contents("clave_n.txt"));
	ZZ	ep = stoZZ (get_file_contents("clave_e.txt"));
	ZZ	d = stoZZ (get_file_contents("clave_d.txt"));
	ZZ	n = stoZZ (get_file_contents("clave_n.txt"));

    FirmaDigital B(np,ep,d,n);

    r1 = B.Cifrar(mensaje,firma);
   // cout<<r1<<endl;

    r2 = A.decifrar(r1);


    return 0;
}