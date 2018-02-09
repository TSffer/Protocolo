#include <iostream>
#include "Protocolo.h"
#include "Protocolo.cpp"
using namespace std;

int main()
{
    SeedXorShift();
    SeedBBS();

    Protocolo A;
    int bits;
    cout<<"Ingrese el tamaño de bits de la clave: ";
    cin>>bits;
    A.generarClaves(bits);

    int op;
    int tamA;

    while(true)
    {
        int opcion;
        cout<<" [1] Encriptar \n [2] Desencriptar \n [0] Cerrar \n"<<endl;
        cout<<"Elija una opcion: ";
        cin>>opcion;
 
        if(opcion == 1)
        {   
            ifstream Plano;
            ifstream FPlano;

            ofstream archCifrado;
            archCifrado.open("Mensaje Cifrado.txt",ifstream::binary);
            string tPlano;
            string FirPlano;
            cout<<"Ingrese el tamaño de <a> : ";
            cin>>tamA;

            ZZ N_p = stoZZ(get_file_contents("clave_N.txt"));
            ZZ g_p = stoZZ(get_file_contents("clave_g.txt"));
            ZZ q_p = stoZZ(get_file_contents("clave_q.txt"));
            ZZ e_p = stoZZ(get_file_contents("clave_e.txt"));
            Protocolo R(tamA,N_p,g_p,q_p,e_p, A.getD() , A.getN() );

            Plano.open("Mensaje.txt",ifstream::binary);
            FPlano.open("FMensaje.txt",ifstream::binary);
            getline(Plano,tPlano);
            getline(FPlano,FirPlano);
            //string Firma = R.Imagen();
            string Rubrica = R.FirmarMensaje(FirPlano);


            string men = R.encriptar(tPlano , Rubrica);
            
            archCifrado <<men;
            
            Plano.close();
            archCifrado.close();

            cout<<"****************REALIZADO**************"<<endl;
        }

        if(opcion == 2)
        {
            ifstream tCifrado;
            string mensajeC;
            tCifrado.open("Mensaje Cifrado.txt",ifstream::binary);
            getline(tCifrado,mensajeC);
            cout<<"****************PROCESANDO**************"<<endl;
            string RubricaMensaje = A.desencriptar(mensajeC);

            tCifrado.close();
            cout<<"****************REALIZADO**************"<<endl;
        }
        if(opcion == 0)
        {
            cout<<"****************REALIZADO**************"<<endl;
            break;
        }

    }
    return 0;
}
