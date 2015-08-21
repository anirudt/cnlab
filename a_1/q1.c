#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define packet 1500
#define slots 100
#define NUM_POINTS 100
#define NUM_COMMANDS 2

/* Following Function generates 
 * exponentially distributed 
 * random numbers 
 */
double next_time(double lambda)
{
  double U, E;
  U = (random()%RAND_MAX+1)/(1.0*RAND_MAX);
  E = -1.0*log(U)/lambda;
  return E;
}

/* Following function generates
 * random numbers greater and lesser than one
 * and proceeds according to the Poisson Distribution
 */ 
double pareto(double alpha, double *x)
{
  *x = (random()%RAND_MAX+1)/(0.1*RAND_MAX);
  if(*x>=1)
    return (alpha*1.0/pow(*x, alpha));
  else
    return 0;
}

int main()
{
  double lambda, E[packet], sum=0, interval, val;
  int i, freq[slots], j, packets[slots];
  double count = 0.0;
  int choice;
  // printf("Enter lambda:\n");
  scanf("%lf", &lambda);
  srand(time(NULL));
  for(i=0; i<packet; i++)
    E[i] = 0;
  for(i=1; i<packet; i++)
  {
    val = next_time(lambda);
    E[i] = E[i-1] + val;
  }
  interval = E[packet-1]/slots;
  
  for(i=0; i<slots; i++)
  {
    freq[i]=0;
    packets[i] = 0;
  }
  /* Denoting which arrival slot the packets are coming in */
  for(i=0; i<slots; i++)
  {
    for(j=0; j<packet; j++)
    {
      if(E[j]>=i*interval && E[j]<(i+1)*interval)
        packets[i]++;
    }
  }

  /* Ref counting the no. of arrivals in the slot*/
  int c = 0;
  for(i=0; i<slots; i++)
  {
    c = 0;
    for(j=0; j<slots; j++)
    {
      if(packets[j]==i)
        freq[i]++; 
    }
  }
  
  double pdf[slots], pdf_sum = 0.0;
  for(i=0; i<slots; i++)
  {
    pdf[i] = freq[i]*1.0/packet;
    pdf_sum += pdf[i];
    //printf("%d %lf\n", i, pdf[i]);
  }
  for(i=0; i<slots; i++)
    pdf[i] /= pdf_sum;

  /* Pareto Traffic Generation */
  double alpha, P[packet], X[packet], x;
  //printf("Enter the value of alpha: \n");
  scanf("%lf", &alpha);
  for(i=0; i<packet; i++)
  {
    P[i] = 0; X[i] = 0;
  }

  /* PDF of the packets */
  for(i=0; i<packet; i++)
  {
    P[i] = pareto(alpha, &x);
    X[i] = x;
    //printf("%lf %lf\n", X[i], P[i]);
  }
  scanf("%d", &choice);
  /* Choices for printing the Poisson/Pareto Distribution */
  switch(choice)
  {
    case 1: for(i=0; i<slots; i++)
            {
              printf("%d %lf\n", i, pdf[i]);
            }
            break;
    case 2: for(i=0; i<packet; i++)
            {
              printf("%lf %lf\n", X[i], P[i]);
            }
            break;
  }
  return 0;
}
