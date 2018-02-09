#include <iostream>
#include "FirmaDigital/FirmaDigital.cpp"
#include "Gamal/gamal.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;


class Protocolo : public Gamal
{
    private:
        ZZ N;
        ZZ p1;
        ZZ p2;
        ZZ fn;
        ZZ d;
        ZZ e;

        ZZ a;
        ZZ q;
        ZZ g;
        ZZ k;
        ZZ Ca;
        ZZ kiv;


        //ADICIONALES SOLO PARA LA FIRMA
        ZZ n_prop;
        ZZ dp;
		ZZ dq;
		ZZ P1;
		ZZ P2;
		ZZ q1;
		ZZ q2;

    public:
        string alfabeto = "0123456789 abcdefghijklmnopqrstuvwxyz.,ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        Protocolo();
        Protocolo(int,ZZ,ZZ,ZZ,ZZ,ZZ,ZZ);
        void generarClaves(int);
        ZZ getD();
        ZZ getN();
        string encriptar(string ,string);
        string desencriptar(string mns);
        
        string Imagen();
        void recuperarImagen(string);

        string FirmarMensaje(string);
        string VerFirma(string);
        long tamBloque();
        ZZ resto_chino_G(ZZ c);
        int modE(int,int);
              
};

