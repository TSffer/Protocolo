#include <iostream>
#include <NTL/ZZ.h>
#include <string>
#include <time.h>
#include <sstream>
#include <fstream>
#include <map>
#include <math.h>
using namespace NTL;
using namespace std;
#include "funciones.h"
#include "xorshift.h"
#include "miller_rabin.h"
#include "bbs.h"
#include "pollar.h"
#include "gamal.cpp"

int main()
{
    SeedXorShift();
    SeedBBS();
  
    Gamal A;
    int bits;
    cout<<"Ingrese el tamaño de bits de la clave: ";
    cin>>bits;
    A.generarClaves(bits);

    int op;

    while(1)
    {
        int opcion;
        cout<<" [1] Encriptar \n [2] Desencriptar \n [0] Cerrar \n"<<endl;
        cout<<"Elija una opcion: ";
        cin>>opcion;

        if(opcion == 1)
        {
            ifstream Plano;
            string tPlano;
            ZZ e_1 = stoZZ(get_file_contents("E_1.txt"));
            ZZ e_2 = stoZZ(get_file_contents("E_2.txt"));
            ZZ p_a = stoZZ(get_file_contents("P_a.txt"));
            Plano.open("Mensaje.txt",ifstream::binary);
            getline(Plano,tPlano);
            Gamal R(e_1,e_2,p_a);
            R.encriptar(tPlano);
            Plano.close();
        }
        else if(opcion == 2)
        {
            ifstream tCifrado;
            string mensajeC;
            tCifrado.open("Mensaje Cifrado.txt",ifstream::binary);
            getline(tCifrado,mensajeC);
            
            A.desencriptar(mensajeC);

            tCifrado.close();
        }
        else if(opcion == 0)
        {
            break;
        }
        else
        cout<<"Vuelva a ingresar otra opcion"<<endl;
    }
    return 0;
}
