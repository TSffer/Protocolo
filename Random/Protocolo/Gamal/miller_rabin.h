ZZ k_q(ZZ q)
{
  ZZ k;
  q--;
  k=0;
  while((q&1)==0)
  {
    q=q>>1;
    k++;
  }
  return q;
}

bool m_r(ZZ n,int max)
{
  ZZ e,a,temp,r;
  ZZ q=k_q(n);
  for(int i=0;i<max;i++)
  {
    a=XorShift(n-1)+1;
    temp = q;
    r=exp(a,temp,n);
    while(temp != n-1 && r!=1 && r!=n-1)
    {
      r = MultMod(r,r,n);
      temp = temp << 1;
    }
    e=temp & 1;
    if(r!=n-1 && e== 0)
      return false;
  }
  return true;
}
