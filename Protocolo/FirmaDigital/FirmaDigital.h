#include <iostream>
#include <NTL/ZZ.h>
#include <string>
#include <time.h>
#include <sstream>
#include <fstream>
#include <regex>
#include <map>
#include <math.h>
using namespace NTL;
using namespace std;


class FirmaDigital
{
    private:
        ZZ p;
        ZZ q;
        ZZ n;
        ZZ fn;
        ZZ d;
        ZZ e;

        ZZ e_publica;
        ZZ n_publica;

        ZZ dp;
        ZZ dq;
        ZZ P1;
        ZZ P2;
        ZZ q1;
        ZZ q2;
    
    public:
		string alfabeto ="0123456789 abcdefghijklmnopqrstuvwxyz.,ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        long long sizeAlfabeto = alfabeto.size();
        FirmaDigital();
        FirmaDigital(ZZ,ZZ,ZZ,ZZ);
        void generarClaves(int);
        

        string CifrarFirma(string);
        string Firmar(string);
        string Cifrar(string,string);

        string decifrar(string);
        string Rubrica(string);
        string decifrarRubrica(string);
        ZZ resto_chino_G(ZZ);
};