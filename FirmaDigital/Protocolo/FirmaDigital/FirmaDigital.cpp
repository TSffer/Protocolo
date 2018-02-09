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
#include "funciones.h"
#include "xorshift.h"
#include "miller_rabin.h"
#include "bbs.h"
#include "pollar.h"
#include "functions.h"
#include "FirmaDigital.h"


FirmaDigital::FirmaDigital()
{
	cout<<"CREANDO CLAVES......"<<endl;
}

FirmaDigital::FirmaDigital(ZZ nn ,ZZ ee , ZZ dd , ZZ n)
{
	long long sizeAlfabeto = alfabeto.size();
	n_publica = nn;
	e_publica = ee;

	this->n = n;
	this->d = dd;
}

void FirmaDigital::generarClaves(int nbits)
{
	ofstream clave_e;
	ofstream clave_n;
	ofstream clave_d;

	clave_e.open("clave_e.txt",ifstream::binary);
	clave_n.open("clave_n.txt",ifstream::binary);
	clave_d.open("clave_d.txt",ifstream::binary);
	
	srand(time(0));

	this->p = funcion(nbits,5,2,16,5);
	cout<<"--------------------------"<<endl;
	this->q = funcion(nbits,5,50,3,7);
	cout<<"--------------------------"<<endl;

	this->n = p*q;

	this->fn = (p-1)*(q-1);

	while(mcd(e,fn) != 1)
	{
		this->e = funcion(nbits,5,6,32,9);
	}

	this->d = mod(Inverso(e,fn),fn);
	this->dp = mod(d,p-1);
	this->dq = mod(d,q-1);

	this->P1 = n/p;
	this->P2 = n/q;
	this->q1 = exp(P1,p-2,p);
	this->q2 = exp(P2,q-2,q);

	cout<<" CLAVES GENERADAS "<<endl;
	clave_e << e;
	clave_n << n;
	clave_d << d;

	clave_e.close();
	clave_n.close();
	clave_d.close();
}

ZZ FirmaDigital::resto_chino_G(ZZ c)
{
	ZZ C1 = exp(c,dp,p);
	ZZ C2 = exp(c,dq,q);

	ZZ R1 = mod(C1*P1*q1,n);
	ZZ R2 = mod(C2*P2*q2,n);

	ZZ R = mod((R1+R2),n);
	return R;
}


string FirmaDigital::CifrarFirma(string mensaje)
{
	ZZ m , c;
	string enc , auxiliar;
	for(int i=0;i<mensaje.size();i++)
	{
		m = conv<ZZ>(alfabeto.find(mensaje[i]));
		auxiliar += (ToString(m,2));
	}
	int tam = (ele(n_publica)-1);
	int resto = auxiliar.size()%tam;
	if(resto != 0)
	{
		int falta = tam - resto;
		for(int i=0 ; i<falta;i++)
		{
			auxiliar.push_back('0');
		}
	}
	for(int i=0;i<auxiliar.size();i+=tam)
	{
		string auxiliar1(auxiliar,i,tam);
		ZZ m1(INIT_VAL, auxiliar1.c_str());
		c = exp(m1,e_publica,n_publica);
		enc += (ToString(c,ele(n_publica)));
	}
	return enc;
}


string FirmaDigital::Firmar(string firma)
{
	ZZ m,c;
	string auxiliar,enc;
	for(int i=0;i<firma.size();i++)
	{
		m = conv<ZZ>(alfabeto.find(firma[i]));
		auxiliar +=(ToString(m,2));
	}
	int tam = (ele(n) - 1);
	int resto = auxiliar.size()%tam;
	if(resto != 0)
	{
		int falta = tam-resto;
		for(int i=0;i<falta;i++)
		{
			auxiliar.push_back('0');
		}
	}
	for(int i=0;i<auxiliar.size();i+=tam)
	{
		string auxiliar1(auxiliar,i,tam);
		ZZ m1 = stoZZ(auxiliar1);
		c = exp(m1,d,n);
		enc += (ToString(c,ele(n)));
	}
	
	//string firmado = CifrarFirma(enc);

	return enc;
}

string FirmaDigital::Cifrar(string mensaje , string firma)
{
	ZZ m , c;
	string enc , auxiliar;

	string RUBRICA = Firmar(firma);
	string mns = RUBRICA+mensaje;

	for(int i=0;i<mns.size();i++)
	{
		m = conv<ZZ>(alfabeto.find(mns[i]));
		auxiliar += (ToString(m,2));
	}
	int tam = (ele(n_publica)-1);
	int resto = auxiliar.size() % tam;
	if(resto != 0)
	{
		int falta = tam - resto;
		for(int i=0 ; i<falta;i++)
		{
			auxiliar.push_back('0');
		}
	}
	for(int i=0;i<auxiliar.size();i+=tam)
	{
		string auxiliar1(auxiliar,i,tam);
        ZZ m1(INIT_VAL,auxiliar1.c_str());
		c = exp(m1,e_publica,n_publica);
		enc += (ToString(c,ele(n_publica)));
	}

	///visualizar rubrica
	/*cout<<"TAMAÑO DE RUBRICA   :"<<RUBRICA.size()<<endl;
	cout<<"TAMAÑO DE N PUBLICA   :"<<ele(n)<<endl;
	cout<<"TAMAÑO DE mendaje   :"<<enc.size()<<endl;*/
	return enc;
}









/*
string FirmaDigital::decifrarRubrica(string mns)
{
    n_publica = stoZZ(get_file_contents("clave_n.txt"));
	string ans,aux_1;

	string R1(mns,0,ele(n));

	//cout<<"R1 decifrar  "<<R1<<endl;
	string firma = Rubrica(R1);
	//cout<<"THIS   :"<<firma<<endl;
	string mns1(mns,0,mns.size());
		cout<<"Hola"<<endl;

		cout<<"N: "<<n<<endl;
	ZZ m;
	for(int i=0;i<mns1.size();i+=ele(n))
	{
		string aux(mns1,i,ele(n));
		ZZ c = stoZZ(aux);
		m = resto_chino_G(c);
		aux_1 += (ToString(m,ele(n)-1));
	}
	for(int i=0;i<aux_1.size();i+=2)
	{
		string aux(aux_1,i,2);
		ZZ c = stoZZ(aux);
		ans += alfabeto[conv<int>(c)];
	}
	cout<<firma<<endl;
	cout<<ans<<endl;

	string mensaje  = firma+ans;
	return mensaje;
}*/


string FirmaDigital::Rubrica(string mns)
{
    n_publica = stoZZ(get_file_contents("clave_n.txt"));
    e_publica = stoZZ(get_file_contents("clave_e.txt"));

	string ans,aux_1;

	//string R1(mns,0,ele(n));

	//cout<<"R1 decifrar  "<<R1<<endl;
	//string firma = Rubrica(R1);
	//cout<<"THIS   :"<<firma<<endl;
	//string mns1(mns,0,mns.size());
	//	cout<<"Hola"<<endl;

	//	cout<<"N: "<<n<<endl;
	ZZ m;
	for(int i=0;i<mns.size();i+=ele(n_publica))
	{
		string aux(mns,i,ele(n_publica));
		ZZ c(INIT_VAL, aux.c_str());
		m = exp(c,e_publica,n_publica);
		aux_1 += (ToString(m,ele(n_publica)-1));
	}
	for(int i=0;i<aux_1.size();i+=2)
	{
		string aux(aux_1,i,2);
		ZZ c(INIT_VAL, aux.c_str());
		ans.push_back(alfabeto[conv<int>(c)]);
	}
	//cout<<firma<<endl;
	//cout<<ans<<endl;

	//string mensaje  = firma+ans;
	return ans;
}

string FirmaDigital::decifrar(string mns)
{
    n_publica = stoZZ(get_file_contents("clave_n.txt"));
	string ans,aux_1;

	//string R1(mns,0,ele(n));

	//string firma = Rubrica(R1);
	//cout<<"THIS   :"<<firma<<endl;
	//string mns(mns,0,mns.size());

	ZZ m;
	for(int i=0;i<mns.size();i+=ele(n))
	{
		string aux(mns,i,ele(n));
		ZZ c(INIT_VAL, aux.c_str());
		m = resto_chino_G(c);
		aux_1 += (ToString(m,ele(n)-1));
	}
	for(int i=0;i<aux_1.size();i+=2)
	{
		string aux(aux_1,i,2);
		ZZ c(INIT_VAL, aux.c_str());
		ans.push_back(alfabeto[conv<int>(c)]);
	}
	
	string mensaje(ans,ele(n_publica),ans.size());
	cout<<"THIS MENSAJE : "<<mensaje<<endl;

	string Dfirma(ans,0,ele(n_publica));
	cout<<"THIS FIRMA : "<<Dfirma<<endl;
	string firma = Rubrica(Dfirma);
	cout<<"THIS FIRMA : "<<firma<<endl;
	
	
	return mensaje;
}