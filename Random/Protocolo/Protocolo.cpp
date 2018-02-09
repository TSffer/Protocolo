#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Protocolo::Protocolo()
{
    cout<<"CREANDO CLAVES............"<<endl;
}

void Protocolo::generarClaves(int nbits)
{
    ofstream clave_N;
    ofstream clave_g;
    ofstream clave_q;
    ofstream clave_e;

    clave_N.open("clave_N.txt",ifstream::binary);
    clave_g.open("clave_g.txt",ifstream::binary);
    clave_q.open("clave_q.txt",ifstream::binary);
    clave_e.open("clave_e.txt",ifstream::binary);

    this->q = GP(nbits);
    this->g = RaizPrimitiva(q);
    this->p1 = funcion(nbits/2,5,2,16,5);
    cout<<"-----------------------------"<<endl;
    this->p2 = funcion(nbits/2,4,15,3,4);

    this->N = p1*p2;

    //GENERACION DE CLAVES
    this->fn = (p1-1)*(p2-1);

    while(mcd(e,fn) != 1)
    {
        this->e = funcion(nbits/2-10,5,6,17,9);
    }

    this->d = mod(Inverso(e,fn),fn);

    this->dp = mod(d,p1-1);
	this->dq = mod(d,p2-1);

	this->P1 = N/p1;
	this->P2 = N/p2;
	this->q1 = exp(P1,p1-2,p1);
	this->q2 = exp(P2,p2-2,p2);


    // CLAVE PUBLICA (N , g , q ,e)
    clave_e << e;
    clave_q << q;
    clave_g << g;
    clave_N << N;

    clave_e.close();
    clave_q.close();
    clave_g.close();
    clave_N.close();
}

Protocolo::Protocolo(int n_bits,ZZ N , ZZ g , ZZ q ,ZZ e , ZZ d , ZZ n_prop)
{
    this->a = GA(n_bits/2-10);
    this->k = exp(g,a,q);
    this->Ca = exp(a,e,N);
    this->q = q;
    this->e = e;
    this->N = N;
    this->n_prop = n_prop;
    this->d = d;
}

string Protocolo::encriptar(string mns,string Rubrica)
{
    string encriptar;
    for(int i=0;i<mns.size();i++)
    {
        encriptar += ToString(to_ZZ(alfabeto.find(mns[i])),2);
    }
      
    string encriptar1 = Rubrica + encriptar; 
    int tam = ele(q)-1;
    while(modE(encriptar1.size(),tam))
    {
        encriptar1 += "0";
    }

    string C2;
    for(int i=0;i<encriptar1.size();i+=tam)
    {
            string aux(encriptar1,i,tam);
            ZZ pos(INIT_VAL,aux.c_str());
            pos = mod((pos*k),q);
            C2 += (ToString(pos,tam+1));
    }
    cout<<"********************Cifrado Exitoso**********************"<<endl;
    return ToString(Ca,tam+1)+C2;
}

string Protocolo::desencriptar(string mns)
{
    ZZ dd = stoZZ (get_file_contents("clave_d.txt"));
    ZZ qq = stoZZ (get_file_contents("clave_q.txt"));
    ZZ NN = stoZZ (get_file_contents("clave_N.txt"));
    ZZ gg = stoZZ (get_file_contents("clave_g.txt"));


    string C1(mns,0,ele(qq));
    Ca = to_ZZ(C1.c_str());

    a = exp(Ca,dd,NN);
    k = exp(gg,a,qq);
    kiv = Inverso(k,qq);

    string aux;
    for(int i=ele(qq);i<mns.size();i+=ele(qq))
    {
        string pos(mns,i,ele(qq));

        ZZ c(INIT_VAL,pos.c_str());
        aux += TToString(mod((c*kiv),qq), ele(qq)-1);
    }          
    // SEPARAR  LA RUBRICA DEL MENSAJE 
    // RUBRICA/MENSAJE   EN NUMERO
    //RUBRICA - MENSAJE    
    //APLICAR EN LA RUBRICA RSA  Y MANDAR LA OTRA AQUI ABAJO
    //cout<<"Tamaño del mensaje "<<aux.size()<<endl;
    string Rubrica(aux,0,tamBloque());
    string mensaje(aux,tamBloque(),aux.size()); 

      //Verificar este paso       
    /*if(mensaje.size()%2 == 1 )///////
    {///
        mensaje = "1"+mensaje;
    }//*/
     //Verificar este paso     
    //cout<<"RUBRICA DESCIFRADO : "<<Rubrica<<endl;
    string img = VerFirma(Rubrica);  
    recuperarImagen(img);

    string ans;
    cout<<"ESTE ES ANS: "<<mensaje<<endl;
    for(int i=0;i<mensaje.size();i+=2)
    {
        string pos(mensaje,i,2);
        ans.push_back(alfabeto[stoi(pos)]);
    }
    cout<<"*********************Descifrado Exitoso***************************"<<endl;
    cout<<"MENSAJE: "<<ans<<endl;
    //recuperarImagen(img);
    return ans;
}

string Protocolo::Imagen()
{
    Mat src = imread("img2.jpg",CV_LOAD_IMAGE_COLOR);
    if(src.empty())
    {
        cout<<"Error : La Imagen no  se cargo........."<<endl;
        return 0;
    }
    ofstream archPlano;
    archPlano.open("RGB.txt",std::ifstream::binary);
    string rgb;

    for(int i=0;i<src.rows;i++)
    {
        for(int j=0;j<src.cols;j++)
        {
            rgb +=ToString( (ZZ)src.at<Vec3b>(i,j)[0],3 );
            rgb +=ToString( (ZZ)src.at<Vec3b>(i,j)[1],3 );
            rgb +=ToString( (ZZ)src.at<Vec3b>(i,j)[2],3 );
        }
    }
    archPlano << rgb;
    archPlano.close();
    return rgb;
}

void Protocolo::recuperarImagen(string C)
{
    Mat src(32,32,CV_8UC3);
    int k =0,v;
    for(int i=0;i<32;i++)
    {
        for(int j=0;j<32;j++)
        {
            int cont =0;
            v = 0;
            for(int a = k ;cont<9;a+=3)
            {
                string st(C,a,3);
                src.at<Vec3b>(i,j)[v] = stoi(st);
                v++;
                cont+=3;
            }
            k+=9;
        } 
    }
    cout<<"dibujando"<<endl;
    namedWindow("Display window" , CV_WINDOW_AUTOSIZE);
    imshow("Display window" , src);
    waitKey(0);
    destroyWindow("Display window");
    cout<<"Dibujo terminado"<<endl;
}



string Protocolo::FirmarMensaje(string tPlano)
{
	string MensajeFirmado;
	string Rubrica;
	//ZZ dd = stoZZ (get_file_contents("clave_d.txt"));     //Clave privada
	//ZZ n_prop = stoZZ (get_file_contents("clave_N.txt"));
    //ZZ ee = stoZZ (get_file_contents("clave_e.txt"));  
	//////////////////////////////////////////////////////
	long long tamBloque = ZZtos(n_prop).size()-1; 
	string tPlanoZZstring;
	long long tPlanoZZ;
	long long aux;
	string auxS;

	tPlanoZZstring = tPlano;
	int contaux=0;
		while( mod(ZZ(tPlanoZZstring.size()),ZZ(tamBloque)) != 0 )
		{
			contaux++;
			tPlanoZZstring += "0";
		}

	//   CIFRADO
	ZZ posC=ZZ(0);
	string tmp;
	string tmp2;
	string posCstring="";
	ofstream archPlano;
	archPlano.open("Firma.txt",std::ifstream::binary);
	long long cont ,k=0;

	if( archPlano.is_open())
	{
		for( ; k< tPlanoZZstring.size(); k+=tamBloque)
		{
			cont=0;
			for(long long a=k ; cont<tamBloque; a++)
			{
				tmp+=tPlanoZZstring[a];
				cont++;
			}
			ZZ posPlano = stoZZ(tmp);
			ZZ posC = exp(posPlano,d,n_prop);
			tmp ="";
			posCstring = ZZtos(posC);
			if(posCstring.size() < tamBloque+1)
			{
				for (int l=0; l<(tamBloque+1)-posCstring.size(); l++ )
					tmp2+="0";

				posCstring = tmp2+posCstring;
				tmp2="";
			}
			Rubrica += posCstring;
		}

		//SEGUNDO PASO
		long long tamBloque_ = ZZtos(N).size()-1; 
		string tPlanoZZstring_;
		long long tPlanoZZ_;
		string tPlano_ = Rubrica;
		long long aux_;
		string auxS_;

		tPlanoZZstring_ = tPlano_;
		int contaux=0;
		while( mod(ZZ(tPlanoZZstring_.size()),ZZ(tamBloque_)) != 0 )
		{
			contaux++;
			tPlanoZZstring_ += "0";
		}

		ZZ posC_=ZZ(0);
		string tmp_;
		string tmp2_;
		string posCstring_="";
		long long cont_ ,k_=0;		

		for( ; k_< tPlanoZZstring_.size(); k_+=tamBloque_)
		{
			cont_=0;
			for(long long a=k_ ; cont_<tamBloque_; a++)
			{
				tmp_+=tPlanoZZstring_[a];
				cont_++;
			}
			ZZ posPlano = stoZZ(tmp_);
			ZZ posC = exp(posPlano,e,N);
			tmp_ ="";
			posCstring_ = ZZtos(posC);
			if(posCstring_.size() < tamBloque_+1)
			{
				for (int l=0; l<(tamBloque_+1)-posCstring_.size(); l++ )
					tmp2_+="0";

				posCstring_ = tmp2_+posCstring_;
				tmp2_="";
			}
			
			archPlano << posCstring_;
			MensajeFirmado += posCstring_;
		}
	}
	else
	{
		cout << "Error: No se pudo crear Cifrado.txt " << endl;
	}
	archPlano.close();
	cout << " *****************Cifrado Satisfactorio****************** "<< endl;
	return MensajeFirmado;
}

string Protocolo::VerFirma(string tmp)
{
    ZZ dd = stoZZ (get_file_contents("clave_d.txt"));
    ZZ qq = stoZZ (get_file_contents("clave_q.txt"));
    ZZ NN = stoZZ (get_file_contents("clave_N.txt"));
    ZZ gg = stoZZ (get_file_contents("clave_g.txt"));

	string Rubrica;
	string Firma;
	///////////////////
	cout << "\n\t *****************  Descifrado ************* "<< endl;
	long long nTam = ZZtos(NN).size();
	cout << "Tamaño de clave n: "<<nTam << endl;

	string aux1;
	string tDescifradoTmp;
	string tDescifrado;
	long long tDescifradoInt;

	ofstream archDescifrado;
	archDescifrado.open("Mensaje Descifrado.txt",ofstream::binary);

	if(archDescifrado.is_open())
	{
		///////////////////////////////////////////
		/*string Prueba(tmp,0,tamBloque());          
		/cout<<"BLOQUE A DESIFRAR : "<<Prueba<<endl;*/
		///////////////////////////////////////////
		long long k,cont;
		for(int i=0; i<tmp.size(); i+=nTam)
		{
			cont =0;
			for(k=i; cont<nTam ; k++)
			{
				tDescifradoTmp+=tmp[k];
				cont++;
			}
			
			ZZ posPlano = stoZZ(tDescifradoTmp);
			ZZ posC = exp(posPlano,dd,NN);
			//ZZ posC = resto_chino_G(posPlano);
            aux1 = ZZtos(posC);
			tDescifradoTmp ="";
			for(int i=aux1.size(); i<nTam-1; i++ )
				tDescifrado += "0";
			tDescifrado += aux1;

		}

		Rubrica = tDescifrado;
	//SEGUNDO PASO
	ZZ	n_publico = stoZZ (get_file_contents("N_publico.txt"));     //Clave publica
	ZZ	e_publico = stoZZ (get_file_contents("E_publico.txt"));    //Clave publica
	long long nTam_ = ZZtos(n_publico).size();
	string aux1_;
	string tDescifradoTmp_;
	string tDescifrado_;
	long long tDescifradoInt_;
	long long k_,cont_;
	for(int i=0; i<Rubrica.size(); i+=nTam_)
	{
		cont_ =0;
		for(k_=i; cont_<nTam_ ; k_++)
		{
			tDescifradoTmp_+=Rubrica[k_];
			cont_++;
		}
		
		ZZ posPlano = stoZZ(tDescifradoTmp_);
		ZZ posC = exp(posPlano,e_publico,n_publico);
		aux1_ = ZZtos(posC);
		tDescifradoTmp_ ="";
		for(int i=aux1_.size(); i<nTam_-1; i++ )
			tDescifrado_ += "0";
		tDescifrado_ += aux1_;

	}
		Firma = tDescifrado_;
		archDescifrado << tDescifrado_;
	}
	else
		cout << "ERROR: No se pudo abrir el archivo Cifrado.txt "<<endl;

	archDescifrado.close();
	return Firma;
}

long Protocolo::tamBloque()
{

    ZZ	n_publico = stoZZ (get_file_contents("N_publico.txt"));     //Clave publica

    long long Vrgb = 9216;
    long tPublica = ele(n_publico)-1;
    long tPropia = ele(N)-1;

    long aux = Vrgb/tPublica;
    if(modE(Vrgb,tPublica) !=0)
    {
        aux += 1;
    }
    long v=aux*ele(n_publico);

    long tem1 = v/tPropia;
    if(modE(v,tPropia) !=0 )
    {
        tem1 += 1;
    }
    long tamB = tem1*ele(N);
    return tamB;
}

ZZ Protocolo::getD()
{
    return d;
}

ZZ Protocolo::getN()
{
    return N;
}

ZZ Protocolo::resto_chino_G(ZZ c)
{
	//ZZ p = stoZZ(get_file_contents("clave_p.txt")); 
	ZZ C1 = exp(c,dp,p1);

	ZZ C2 = exp(c,dq,p2);
	
	ZZ R1 = mod(C1*P1*q1,N);
	ZZ R2 = mod(C2*P2*q2,N);

	ZZ R = mod((R1 + R2),N);
	return R;
}

int Protocolo::modE(int D,int d)
{
    int q = (D/d);
    int r = (D-(q*d));
    if(r<(0))
    {
        q--;
        r += d;
    }
    return r;
}

//g++ -std=c++11 -ggdb `pkg-config --cflags opencv` -o  `basename main.cpp .cpp` main.cpp `pkg-config --libs opencv` -lntl

