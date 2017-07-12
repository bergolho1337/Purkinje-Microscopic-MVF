#include "../include/plot.h"

// Imprime uma matriz NxN
void printMatrix (char *str, double *A, int N)
{
  int i, j;
  printf("\n%s\n",str);
  for (i = 0; i < N; i++)
  {
    printf("\n");
    for (j = 0; j < N; j++)
    {
      printf("%e ",A[i * N + j]);
    }
  }
  printf("\n");
}

// Imprime um vetor Nx1
void printVector (char *str, double *b, int N)
{
  int i;
  printf("\n%s\n",str);
  for (i = 0; i < N; i++)
    printf("%e\n",b[i]);
  printf("\n");
}

// Imprime a procentagem do servico que jah se encontra pronto
void printProgress (int iter, int max_iter)
{
  double perc = iter / (double)max_iter;
  fprintf(stdout,"\rProgress -- %.1lf%%",perc*100.0);
  fflush(stdout);
}

void printProgress2 (int iter, int max_iter)
{
  double progress = iter / (double)max_iter;
  int barWidth = 100;

  std::cout << "[";
  int pos = barWidth * progress;
  for (int i = 0; i < barWidth; ++i) 
  {
      if (i < pos) std::cout << "=";
      else if (i == pos) std::cout << ">";
      else std::cout << " ";
  }
  std::cout << "] " << int(progress * 100.0) << " %\r";
  std::cout.flush();
}

void printError (const char *msg)
{
  printf("[-] ERROR ! %s\n",msg);
  exit(-1);
}