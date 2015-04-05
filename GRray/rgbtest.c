
#include <stdio.h>

void test01(char r, char g, char b)
{
  int y = (( 66 * r + 129 * g +  25 * b + 128)>>8) + 16;
  int u = ((-38 * r -  74 * g + 112 * b + 128)>>8);
  int v = ((112 * r -  94 * g -  18 * b + 128)>>8) + 128;
  
  char y2 = (char)y;
  char u2 = (char)u;
  char v2 = (char)v;
  
  printf("r g b : %i  %i  %i\n"  , r, g, b);
  printf("y u v : %i  %i  %i\n\n", y2, u2, v2);
}

int main()
{
  test01(30, 30, 30);
  test01(90, 90, 90);
  test01(150, 150, 150);
  test01(0, 30, 30);
  test01(0, 90, 90);
  test01(0, 150, 150);
  
  return 1;
}

