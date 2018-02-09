#include <vector>
#include <NTL/ZZ.h>
#include <iostream>     // std::cout
#include <algorithm>    // std::count
#include <fstream>
#include <sstream>
#include <ctype.h>

typedef int tipo;

using namespace NTL;
using namespace std;

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
        xd = a +n*(((-1*a)/n)+1);
        if(xd == n)
        {
            return to_ZZ("0");
        }
        return xd;
    }
    return (a-n*(a/n));
}

vector<bool> llenar(vector<bool> &i_contenedor,int tam_bits);
void imprimir(vector<bool> &i_contenedor);

ZZ exponenciar_rapidamente(ZZ a,ZZ n)
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
    for(j =0;j<loops;j++)
    {
        ZZ rndom = modulo_for_ZZ(aleatorio(),n-2)+2;
        ZZ y = expo_modular_rapida_II(rndom,r,n);
        if(y != 1 && y!= n - to_ZZ("1"))
        {
            ZZ j = to_ZZ("1");
            while(j<=s-to_ZZ("1") && y!=n-to_ZZ("1"))
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

ZZ Conv_Bin(vector<bool> Total)
{
    ZZ suma;
    int j=0;
    for(int i=Total.size()-1;i>-1;i--)
    {
        if(Total[i]==1)
            suma+= exponenciar_rapidamente(to_ZZ(2),to_ZZ(j));
            j++;
    }
    return suma;
}

bool taps(vector<bool> comodin , int num_taps)
{
    bool total;
    for(int i=0;i<num_taps;i++)
    {
        total^=comodin[rand()%128+0];
    }
    return total;
}

ZZ funcion(int tam_bits, int largo_semilla,int num_vueltas,int num_particiones,int num_taps)
{
    vector<bool> i_contenedor;
    i_contenedor.resize(largo_semilla);
    llenar(i_contenedor,tam_bits);
    
    ZZ numero;

    vector<bool> comodin(i_contenedor.begin(),i_contenedor.end());
    for(int i=0;i<num_vueltas;i++)
    {
        int indice_1 = 0;
        int indice_2 = tam_bits/num_particiones;
        indice_2 = indice_2*2-1;
        int largo_bloque = tam_bits/num_particiones;

        for(int p = 0;p<num_particiones/2;p++)
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
        //comodin[comodin.size()-1] = 1;
    }

    numero = Conv_Bin(comodin);
    return numero;
}

void imprimir(vector<bool> &i_contenedor)
{
    for(int x:i_contenedor)
        cout<<x;
    cout<<endl;
}

vector<bool> copiar_model(vector<bool> &i_contenedo)
{
    i_contenedo.clear();
    vector<bool> copi;
    copi.resize(3);
    copi[0] = 1;
    copi[1] = 0;
    copi[2] = 1;
    for(int i=0;i<copi.size();i++)
        i_contenedo.push_back(copi[i]);
        return i_contenedo;
}

//***************AQUI DEBE DE GENERARSE LA SEMILLA***********************   
vector<bool> llenar(vector<bool> &i_contenedor,int tam_bits)
{
    srand(time(NULL));
    for(int i=0;i<i_contenedor.size();i++)
        i_contenedor[i] = rand()%2+0;

    int i_count_zero = count(i_contenedor.begin(),i_contenedor.end(),0);
    if(i_count_zero == i_contenedor.size())
        copiar_model(i_contenedor);

    int tamano_anterior = i_contenedor.size();
    i_contenedor.resize(tam_bits);
    for(int i=tamano_anterior+1,j=0;i<i_contenedor.size();i++,j++)
        i_contenedor[i] = (i_contenedor[j]^i_contenedor[j+1]);
    return i_contenedor;     
}

string ConvBin(ZZ num , vector<bool> &binario)
{
    vector<bool> v;
    while(num>0){
        v.push_back(num%2);
        num = num/2;
    }
    v.push_back(num%2);
    string NUM;
    vector<bool>::const_reverse_iterator it = v.rbegin();
    for(;it!= v.rend();it++)
    {
        binario.push_back(*it);
        NUM += to_string(*it);
    }
    return NUM;
    cout<<"THIS: "<<NUM<<endl;
}

//g++ -std=c++11 -ggdb `pkg-config --cflags opencv` -o  `basename main.cpp .cpp` main.cpp `pkg-config --libs opencv` -lntl

int main()
{
    ZZ a;
    a = funcion(1024,5,3,1,1);
    cout<<a<<endl;
    return 0;
}
/*
29430020221035210935525196627169197384754839822902321478264189869051687456638627156134304027715327094713542391186430257812409461557723985687914374781005333919472308341515570477586542904077167710125093963091877922012746267210501373893127096105487075600402466129966963783506977348730313191246964897472215132648
472861109198404347135956339089858629986194050966330393929580401080280475425237563311677541689139373553685484482621771155373589590368578135738193403876430995716938603322787691108042221631698827064571896520775736158871847588448188058891681048844984901686941921193336571543451830974035292850122507443174429132554728134849932176147757832043616816425943926384962104727399665006604581987651134502329329675425681269897860256975467744325587283350106674637354199993077346382891998821179509242596697013409957812619031020856366946385419919715688071613211188098889582043275880056767348107307490861186465172162854957442942884801
*/