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
            ifstream PFirma;
            
            ifstream Plano;
            ofstream archCifrado;
            archCifrado.open("Mensaje Cifrado.txt",ifstream::binary);
            string tPlano;
            cout<<"Ingrese el tamaño de <a> : ";
            cin>>tamA;

            ZZ N_p = stoZZ(get_file_contents("N_publico.txt"));
            ZZ g_p = stoZZ(get_file_contents("G_publico.txt"));
            ZZ q_p = stoZZ(get_file_contents("Q_publico.txt"));
            ZZ e_p = stoZZ(get_file_contents("E_publico.txt"));
            ZZ n_prop = stoZZ(get_file_contents("clave_N.txt"));
        
            Protocolo R(tamA,N_p,g_p,q_p,e_p, A.getD() , A.getN() );

            Plano.open("Mensaje.txt",ifstream::binary);
            getline(Plano,tPlano);
              
            string Firma = R.Imagen();
            string Rubrica = R.FirmarMensaje(Firma);


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
