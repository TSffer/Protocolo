
ZZ mod(ZZ D,ZZ d)
{
    ZZ q = ZZ(D/d);
    ZZ r = ZZ(D-(q*d));
    if(r<ZZ(0))
    {
        q--;
        r += d;
    }
    return r;
}

ZZ exp(ZZ a,ZZ b,ZZ n)
{
    ZZ d = (ZZ)1;
    long tamano = NumBits(b);
    for(long i = tamano-1;i>=0;i--)
    {
        d = mod(d*d,n);
        if(bit(b,i)==1)
        {
            d = mod(d*a,n);
        }
    }
    return d;
}



ZZ mcd( ZZ a , ZZ b )
{
	ZZ aux = ZZ(1) ;
	ZZ tmp;

	while( (a&1) == 0 and (b&1) == 0 )
	{

		a >>= 1 ;
		b >>= 1 ;
		aux <<= 1 ;
	}

	while ( a != 0 )
	{
		if ( (a&1) == 0)
			a >>= 1 ;
		else if ( (b&1) == 0 )
			b >>= 1;
		else { //3er caso : a y b son impares
			tmp = abs(a-b);
			tmp >>= 1 ;
			if (a >= b )
				a = tmp;
			else
				b = tmp;
		}
	}
	return aux*b;
}

ZZ Inverso(ZZ a ,ZZ b)
{
	ZZ  s1, s2 , r,s,q;
	s1 = ZZ(1) ; s2 = ZZ(0)  ; 
	while( b > ZZ(0))
	{
		q = a/b;
		r = a -(q*b);
		a = b ; 
        b = r ;

		s= s1 -(q*s2);
		s1 = s2 ; 
        s2 = s ;
	}
	return s1;
}


ZZ MultMod(ZZ a,ZZ b)
{
  ZZ ans,e;
  if(b==0)
  {
    ans=0;
    return ans;
  }
  ans=0;
  while(b!=1)
  {
    e=b&1;
    if(e==1) ans=(ans+a);
    a=(a+a);
    b=b>>1;
  }
  return (ans+a);
}

ZZ MultMod(ZZ a,ZZ b,ZZ c)
{
  ZZ ans,e;
  if(b==0)
  {
    ans=0;
    return ans;
  }
  ans=0;
  while(b!=1)
  {
    e=b&1;
    if(e==1) ans=(ans+a) % c;
    a=(a+a) % c;
    b=b>>1;
  }
  return (ans+a) % c;
}

ZZ ExpMod(ZZ a,ZZ b)
{
  ZZ ans,e;
  if(a==0)
  {
    ans=0;
    return ans;
  }
  if(b==0)
    return to_ZZ("1");
  ans=1;
  while (b != 1)
  {
    e = b&1;
    if(e == 1) ans = ans*a;
    a = a*a;
    b = b>>1;
  }
  return MultMod(ans,a);
}

string get_file_contents(const char *filename)
{
  ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    return(string( (std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>() ));
  }
}

string leer(string in)
{
  string st;
  ifstream LeerArc;
  LeerArc.open(in);
  char c;
  while(LeerArc.get(c))
  {
    st.push_back(c);
  }
  LeerArc.close();
  return st;
}

int ele(ZZ &a)
{
  stringstream stream;
  stream << a;
  string cadena = stream.str();
  return cadena.size();
}

void copiar(string out,string mns)
{
  ofstream CopArc(out,ios::out);
  CopArc << mns;
  CopArc.close();

}

string ToString(ZZ a)
{
  stringstream stream;
  stream.str();
  stream << a;
  string cadena=stream.str();
  return cadena;
}

string ToString(ZZ a,int tam)
{
  stringstream stream;
  stream.str();
  stream << a;
  string cadena=stream.str();
  string ans(tam-cadena.size(),'0');
  return ans+cadena;
}

string TToString(ZZ a,int tam)
{
  stringstream stream;
  stream.str();
  stream << a;
  string cadena=stream.str();
  if(tam-cadena.size()>10)
  {
    return cadena;
  }
  string ans(tam-cadena.size(),'0');
  return ans+cadena;
}


string ZZtos(ZZ num)
{
	stringstream conv;
	conv << num;
	return conv.str();
}

ZZ stoZZ(string num)
{
	ZZ conv(INIT_VAL , num.c_str());
	return conv;
}

bool ZZtoBool(ZZ number)
{
	stringstream conv;
	conv << number;
	string tmp = conv.str();
	return stoi(tmp);
}
