#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm> 
#include <vector>
#include <stdio.h>
#include <NTL/ZZ.h>
#include <sstream>
#include <string>
#include <math.h>
#include <fstream>
#include <regex>
#include <ctype.h>

using namespace std;
using namespace NTL;
using namespace sf;
#include "funciones.h"
#include "Gamal/xorshift.h"
#include "Gamal/miller_rabin.h"

//*******************FUNCIONES MATEMATICAS******************
int MOD(ZZ D,int d)
{
    ZZ q = (D/d);
    ZZ r = (D-(q*d));
    if(r<(0))
    {
        q--;
        r += d;
    }
    int res = conv<int>(r);
    return res;
}

ZZ aleatorio()
{
    time_t t;
    srand((unsigned)time(&t));
    int res = rand();
    return to_ZZ(res);
}

ZZ modulo_for_ZZ(ZZ a,ZZ n)
{
    if(abs(a)==abs(n))
        return to_ZZ("0");
    if(a<0 && (a+n)!=0)
    {
        ZZ xd;
        xd = a+n*(((-1*a)/n)+1);
        if(xd == n)
        {
            return to_ZZ("0");
        }
        return xd;
    }
    return (a-n*(a/n));
}

ZZ exponenciar_rapidamente(ZZ a, ZZ n)
{
    ZZ x = a;
    ZZ exp;
    exp = 1;
    while(n > 0)
    {
        if(modulo_for_ZZ(n,to_ZZ(2))==1)
        {
            exp = (exp*x);
        }
        x = (x*x);
        n = n>>1;
    }
    return (exp);
}

ZZ expo_modular_rapida_II(ZZ a,ZZ k,ZZ n)
{
    ZZ b;
    b = conv<ZZ>(1);
    if(k==0)
        return b;
    ZZ A;
    A = a;
    if(bit(k,0)==1)
        b=a;
    int tamano;
    tamano = NumBits(k);
    for(int i=1;i<tamano;i++)
    {
        A = modulo_for_ZZ(A*A,n);
        if(bit(k,i)==1)
        {
            b = modulo_for_ZZ(A*b,n);
        }
    }
    return b;
}

//*************************FUNCIONES*************************

vector<bool> ConvertirString_a_Vector(string a)
{
    vector<bool> v;
    string aa;
    for(int i=0;i<a.size();i++)
    {
        aa += a[i];
        int val = stoi(aa);
        v.push_back(val);
        aa = "";
    }
    return v;
}

string ConvertirVector_a_String(vector<bool> v)
{
    string a;
    for(int i=0;i<v.size();i++)
    {
        a += to_string(v[i]);
    }
    return a;
}

void Imprime(vector<bool> v)
{
    for(int i=0;i<v.size();i++)
    {
        cout<<v[i];
    }
    cout<<"\n"<<endl;
}

string Convertir_a_Binario(ZZ num , vector<bool> &binario)
{
    vector<bool> v;
    while(num>0)
    {
        v.push_back(MOD(num,2));
        num = num/2;
    }
    v.push_back(MOD(num,2));
    string NUM;
    vector<bool>::const_reverse_iterator it = v.rbegin();
    for(;it != v.rend();it++)
    {
        binario.push_back(*it);
        NUM += to_string(*it);
    }
    return NUM;
}

string Convertir_a_String(vector<bool> v)
{
    string Conv="";
    for(int i=0;i<v.size();i++)
    {
        Conv += to_string(v[i]);
    }
    return Conv;
}

ZZ Convertir_a_Decimal(vector<bool> bits)
{
    ZZ suma; 
    int j=0;
    for(int i=bits.size()-1;i>-1;i--)
    {
        if(bits[i] == 1)
            suma += exponenciar_rapidamente(to_ZZ(2),to_ZZ(j));
            j++;
    }
    return suma;
}

//************FUNCIONES PARA GENERAR ALEATORIOS**************

bool test_primalidad_MIll_Rabin(ZZ n,int loops)
{
    ZZ a;
    a = n - to_ZZ("1");
    vector<ZZ> values;
    int i=0;
    while(a != 0)
    {
        i++;
        a /= to_ZZ("2");
        if(a>>1<<1 != a)
        {
            values.push_back(to_ZZ(i));
            values.push_back(a);
        }
    }
    values.push_back(to_ZZ(i));
    values.push_back(a);
    ZZ s = values[0];
    ZZ r = values[1];
    ZZ j;
    for(j=0;j<loops;j++)
    {
        ZZ rndom = modulo_for_ZZ(aleatorio(),n-2)+2;
        ZZ y = expo_modular_rapida_II(rndom,r,n);
        if(y != 1 && y!= n-to_ZZ("1"))
        {
            ZZ j = to_ZZ("1");
            while(j<s-to_ZZ("1") && y!= n-to_ZZ("1"))
            {
                y = expo_modular_rapida_II(y,to_ZZ("2"),n);
                if(y == 1)
                {
                    return 0;
                }
                j++;
            }
            if(y != n-to_ZZ("1"))
                return 0;
        }
    }
    return 1;
}

//////////////////////////////////////////////////////////////////
//*****************OPTENER SEMILLA DEL HARDWARE
//////////////////////////////////////////////////////////////////
vector<bool> copiar_model(vector<bool> &i_contenedor)
{
    i_contenedor.clear();
    vector<bool> copi;
    copi.resize(3);
    copi[0] = 1;
    copi[1] = 0;
    copi[2] = 1;
    for(int i=0;i<copi.size();i++)
        i_contenedor.push_back(copi[i]);
    return i_contenedor;
}


int* seed()
{   
    int *sem=new int[1000];
    int *ini=sem;
    int a=50;
    int x=0, y=0;
    RenderWindow window(VideoMode(400, 400), "SFML works!");
    while (window.isOpen())
    {
        Event event;
        CircleShape shape(a);
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(x,y);
        while (window.pollEvent(event))
        {   
            if (event.type == sf::Event::Closed){
                window.close();
            }
            if (event.type == sf::Event::MouseMoved){
                (*sem)=event.mouseMove.x;sem++;
                (*sem)=event.mouseMove.y;sem++;
            }
            if (event.type == sf::Event::MouseButtonPressed){
                if(event.key.code == sf::Mouse::Left){
                    return ini;
                }
            }
        }
        window.clear();
        window.display();
    }
}


vector<bool> llenar(vector<bool> &i_contenedor,int tam_bits)
{
    vector<bool> bb;
    int *aa = seed();
    int k=0;
    string valores;

    while(k<300)
    {
        valores += to_string(*(aa+k));
        k++;
    }
    ZZ val = stoZZ(valores);
    Convertir_a_Binario(val,bb);

    srand(time(NULL));
    for(int i=0;i<i_contenedor.size();i++)
    {
         i_contenedor[i] = bb[i];
    }
       // i_contenedor[i] = rand()%2+0;

    int i_count_zero = count(i_contenedor.begin(),i_contenedor.end(),0);
    if(i_count_zero == i_contenedor.size())
        copiar_model(i_contenedor);

    int tamano_anterior = i_contenedor.size();
    i_contenedor.resize(tam_bits);
    for(int i=tamano_anterior+1,j=0;i<i_contenedor.size();i++,j++)
        i_contenedor[i] = (i_contenedor[j]^i_contenedor[j+1]);
    return i_contenedor;     
}
//////////////////////////////////////////////////////////////////

bool taps(vector<bool> comodin , int num_taps)
{
    bool total;
    for(int i=0;i<num_taps;i++)
    {
        total^=comodin[rand()%128+0];
    }
    return total;
}

/////////////////////////////PASO 1/////////////////////////////

string funcionRotar(string cadena,int tam_bits)
{
    int num_taps = 1;
    int num_particiones = 2;
    int num_vueltas = 4;
    vector<bool> i_contenedor = ConvertirString_a_Vector(cadena);

    vector<bool> comodin(i_contenedor.begin(),i_contenedor.end());
    for(int i=0;i<num_vueltas;i++)
    {
        int indice_1 = 0;
        int indice_2 = tam_bits/num_particiones;
        indice_2 = indice_2*2-1;
        int largo_bloque = tam_bits/num_particiones;

        for(int p=0;p<num_particiones/2;p++)
        {
            for(int m=0;m<largo_bloque;m++)
            {
                comodin[indice_1] = comodin[indice_1+1];
                comodin[indice_2] = comodin[indice_2-1];
                indice_1++;
                indice_2--;
            }
            comodin[indice_1] = taps(comodin,num_taps);
            indice_1 += largo_bloque;
            comodin[indice_2] = taps(comodin,num_taps);
            indice_2 += largo_bloque*3;
        }
    }
    string Sem = "";
    Sem += Convertir_a_String(comodin);
    return Sem;
}

string funcion(int tam_bits, int largo_semilla,int num_vueltas,int num_particiones,int num_taps)
{
    vector<bool> i_contenedor;
    string Sem;
    i_contenedor.resize(largo_semilla);
    llenar(i_contenedor,tam_bits);     
    Sem =funcionRotar( Convertir_a_String(i_contenedor),tam_bits);     
    return Sem;
}

/////////////////////////////PASO 2/////////////////////////////

void PC1(vector<bool> &v)
{
    int Table[56] = {57 , 49 , 41 , 33 , 25 , 17 , 9 ,
                      1 , 58 , 50 , 42 , 34 , 26 , 18 ,
	                 10 ,  2 , 59 , 51 , 43 , 35 , 27 ,
                     19 , 11 ,  3 , 60 , 52 , 44 , 36 , 
	                 63 , 55 , 47 , 39 , 31 , 23 , 15 , 
                      7 , 62 , 54 , 46 , 38 , 30 , 22 ,
	                 14 ,  6 , 61 , 53 , 45 , 37 , 29 ,
                     21 , 13 ,  5 , 28 , 20 , 12 ,  4};
 
    for(int k =0;k<56;k++)
    {
       int temp = v[0];
       int valor = Table[k];
       v[0] = v[valor];
       for(int k=valor;k>1;k--)
       {
           v[k] = v[k-1];
       }
       v[1] = temp;
    }  
}

void PC2(vector<bool> &v)
{
    int Table[48] = {14 , 17 , 11 , 24 ,  1 ,  5 ,
                      3 , 28 , 15 ,  6 , 21 , 10 ,
                     23 , 19 , 12 ,  4 , 26 ,  8 ,
                     16 , 7 ,  27 , 20 , 13 ,  2 , 
                     41 , 52 , 31 , 37 , 47 , 55 ,
                     30 , 40 , 51 , 45 , 33 , 48 ,
                     44 , 49 , 39 , 56 , 34 , 53 , 
                     46 , 42 , 50 , 36 , 29 , 32 };
   
    for(int k =0;k<48;k++)
    {
       int temp = v[0];
       int valor = Table[k];
       v[0] = v[valor];
       for(int k=valor;k>1;k--)
       {
           v[k] = v[k-1];
       }
       v[1] = temp;
    }    
}

void Dividir(vector<bool> &C0, vector<bool> &D0 , vector<bool> v)
{
    for(int i = 0 ;i < 28;i++)
    {
            C0.push_back(v[i]);      
    }    
    for(int j = 28 ;j < 56;j++)
    {
            D0.push_back(v[j]);
    }
}

void Rotar(vector<bool> &v ,int n)
{
    for(int k = 0;k<n;k++)
    {
        int temp = v[0];
        for(int i = 0;i<v.size();i++)
        {
            v[i] = v[i+1];
        }
        v[v.size()-1] = temp;
    }
}

vector<bool> CiDi(vector<bool>Ci , vector<bool>Di )
{
    vector<bool> Kn;
    for(int i=0 ; i<Ci.size();i++)
    {
        Kn.push_back(Ci[i]);
    }
    for(int j=0;j<Di.size();j++)
    {
        Kn.push_back(Di[j]);
    }   
    return Kn;
}

string ResParcial(vector<bool> Cii, vector<bool> Dii)
{
   int Table[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
    vector<bool> Kn;
    string res;
    for(int i=0;i<16;i++)
    {
        Rotar(Cii,Table[i]);
        Rotar(Dii,Table[i]);
        Kn = CiDi(Cii,Dii);
        PC2(Kn);
        ZZ primoParcial = Convertir_a_Decimal(Kn);
        if(m_r(primoParcial,10)==1)
        {
            res = Convertir_a_String(Kn);
            return res;
        }
    }
    Kn[Kn.size()-1] = 1;
    res = Convertir_a_String(Kn);
    return res;
}


ZZ RandomNumber(int bits)
{
    string Bloques = funcion(bits,56,15,7,10);
    string GG=Bloques;
    string Num;

    while(1)
    {

        for(int i = 0;i<GG.size(); i+= 64)
        {
            string aux(GG,i,64);
            vector<bool> Bloq = ConvertirString_a_Vector(aux);
            PC1(Bloq);
            vector<bool> Ci , Di;
            
            Dividir(Ci,Di,Bloq);
        
            Num += ResParcial(Ci,Di);
        }
        
        //cout<<"NUMERO DE BITS : "<<Num<<endl;
        
        vector<bool> Primo = ConvertirString_a_Vector(Num);
        Primo[Primo.size()-1] = 1;
        ZZ PPrimo=Convertir_a_Decimal(Primo);
       // cout<<"NUMEROS GENERADOS: "<<PPrimo<<endl;
        if(test_primalidad_MIll_Rabin(PPrimo,20)==1)
        {
            //cout<<"Es primo!!!"<<endl;
            return PPrimo;
        }
        GG = funcionRotar(GG,bits);
        Num = "";
    }
    return (ZZ)0;
}

int main()
{
    cout<<RandomNumber(1024)<<endl;
    return 0;
}
