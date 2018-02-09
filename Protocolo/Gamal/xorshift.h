ZZ xorshift_x, xorshift_y, xorshift_z, xorshift_w;

void SeedXorShift()
{
  srand(time(NULL));
  time_t timer = time(NULL);
  struct tm y2k;
  y2k.tm_hour = 0;
  y2k.tm_min = 0;
  y2k.tm_sec = 0;
  y2k.tm_year = 100;
  y2k.tm_mon = 0;
  y2k.tm_mday = 1;
  ZZ seconds = conv<ZZ> (difftime(timer,mktime(&y2k)));
  xorshift_x = seconds;
  xorshift_y = seconds ^ 123456789;
  xorshift_z = seconds << 31;
  xorshift_w = seconds >> 28321;
}

ZZ XorShift(ZZ MAX_XOR)
{
  ZZ tmp;
  tmp = xorshift_x^(xorshift_x<<11);
  xorshift_x = xorshift_y;
  xorshift_y = xorshift_z;
  xorshift_z = xorshift_w;
  xorshift_w = xorshift_w ^ (xorshift_w>>19)^(tmp^(tmp>>8));
  return xorshift_w % MAX_XOR;
}
