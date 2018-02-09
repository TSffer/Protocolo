#include "gamal.h"

Gamal::Gamal()
{
    cout<<"Creando las claves .... "<<endl;
}

Gamal::Gamal(ZZ e1_,ZZ e2_,ZZ p_)
{
    this->e1 = e1_;
    this->e2 = e2_;
    this->p = p_;
}

void Gamal::generarClaves(int nbits)
{
    ofstream E1;
    ofstream E2;
    ofstream Pa;

    E1.open("E1.txt",ifstream::binary);
    E2.open("E2.txt",ifstream::binary);
    Pa.open("Pa.txt",ifstream::binary);  
    //ZZ q = GP(nbits);
    //this->p = (2*q) + 1;
    this->p = GP(nbits);
    this->e1 = RaizPrimitiva(p);
    //this->e1 = RG(p);
    this->d = GA(p-1)+1;
    this->e2 = exp(e1,d,p);

    cout<<" Claves Generadas "<<endl;

    E1<<e1;
    E2<<e2;
    Pa<<p;
    E1.close();
    E2.close();
    Pa.close();
}

void Gamal::darDatos()
{
    ifstream darDatos("Recuperacion",ios::in);
    darDatos >> e1;
    darDatos >> e2;
    darDatos >> p;
    darDatos >> d;
}

void Gamal::encriptar(string mns)
{
    r = GA(p-2)+1;
    c1 = exp(e1,r,p);
    km = exp(e2,r,p);
    string encriptar;
    for(int i=0;i<mns.size();i++)
    {
        encriptar += ToString(to_ZZ(alfabeto.find(mns[i])),2);
    }
    int tam = ele(p)-1;
    while(encriptar.size()%tam)
    {
        //encriptar += alfa"0";
        encriptar += to_string(alfabeto.size()-1);
    }

    ofstream archPlano;
    archPlano.open("Mensaje Cifrado.txt",ifstream::binary);

    string C2;
    if(archPlano.is_open())
    {
        for(int i=0;i<encriptar.size();i+=tam)
        {
            string aux(encriptar,i,tam);
            ZZ pos(INIT_VAL,aux.c_str());
            pos = mod((pos*km),p);
            C2 += (ToString(pos,tam+1));
        }
    }
    cout<<"********************Cifrado Exitoso**********************"<<endl;
    archPlano << ToString(c1,tam+1)+C2;
    archPlano.close();
    //return ToString(c1,tam+1)+C2;
}

void Gamal::desencriptar(string mns)
{
    //d = stoZZ(get_file_contents("D.txt"));
    //p = stoZZ(get_file_contents("Pa.txt"));
    string C1(mns,0,ele(p));
    //cout<<mns;
    c1 = to_ZZ(C1.c_str());

    km = exp(c1,d,p);

    km = Inverso(km,p);

    string aux;
    for(int i=ele(p);i<mns.size();i+=ele(p))
    {
        string pos(mns,i,ele(p));

        ZZ c(INIT_VAL,pos.c_str());
        aux += ToString(mod((c*km),p), ele(p)-1);
    }
  //  ofstream archDescifrado;
//    archDescifrado.open("Mensaje Descifrado.txt",ofstream::binary);
          
    string ans;
    for(int i=0;i<aux.size();i+=2)
    {
        string pos(aux,i,2);
        ans.push_back(alfabeto[stoi(pos)]);
    }
    cout<<"*********************Descifrado Exitoso***************************"<<endl;
    cout << ans<<endl;
   // return ans;
   //archDescifrado.close();
}

bool Gamal::esRP(ZZ g,ZZ n)
{
    ZZ m = phiEuler(n);
    map<ZZ,ZZ> M;
    factorization_brent(m,M);
    for(auto it=M.begin();it != M.end();it++)
    {
        ZZ mi = m/it->first;
        if(exp(g,mi,n) == 1)
        {
            return false;
        }
    }
    return true;
}

ZZ Gamal::RaizPrimitiva(ZZ p)
{
    ZZ rand = GA(p-1)+1;
    ZZ phi = phiEuler(p);
    while(!esRP(rand,p))
    {
        rand = GA(p-1)+1;
    }
    return rand;
}

ZZ Gamal::RG(ZZ p)
{
    ZZ q, found, g, i;
	q = (p-to_ZZ(1))/2;

	for (i = 1; i < p-1; i++)
    {
		g = p - i;
		if ((mod(g*g, p) != 1) && (exp(g, q, p) != 1))
        {
       			 break;
		}
	}
	return g;
}