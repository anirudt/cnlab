#include <time.h>
#include <stdio.h>

int main()
{
  clock_t t1, t2;
  t1 = clock();
  int i;
  for(i=0; i<1000000; i++)
    printf("");
  t2 = clock();
  double f = (double)(t2 - t1);
  printf("%lf\n", f);
  return 0;
}
