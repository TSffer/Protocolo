#include <iostream>
#include <string>
#include <vector>
#include <NTL/ZZ.h>
using namespace NTL;
using namespace std;
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
string Convertir_a_String(vector<bool> v)
{
    string Conv="";
    for(int i=0;i<v.size();i++)
    {
        Conv += to_string(v[i]);
    }
    return Conv;
}

string Convertir_a_Binario(ZZ num)
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
        NUM += to_string(*it);
    }
    return NUM;
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

string Codificacion(ZZ N_encriptado,ZZ q, int n)
{
    string tmp;
    string NumE_binario = Convertir_a_Binario(N_encriptado);
    long long cont,k=q.size()*n;
    long long TB_q = Convertir_a_Binario(q).size()-1;

    for(long long a=0;a<TB_q;a++)
    {
        tmp+= NumE_binario[a];
    }
    return NumE_binario;
}


int main()
{
    /*
    ZZ a;
    ZZ q;
    a = ZZ(7889797978978798797987987984646546464111222235); 
    cout<<Codificacion(a,q)<<endl;*/
    string v = "hola";
    cout<<v<<endl;
    v[0] = '';
    cout<<v<<endl;

    return 0;
}

//g++ -std=c++11 -ggdb `pkg-config --cflags opencv` -o  `basename main.cpp .cpp` main.cpp `pkg-config --libs opencv` -lsfml-graphics -lsfml-window -lsfml-system -lntl