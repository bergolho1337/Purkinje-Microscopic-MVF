#include "../include/linSolver.h"

double *pivot;

void solveLinearSystem_CG (double *A, double *b, double *x, int N)
{
  //printf("[+] Resolvendo o sistema linear associado ... ");
  //fflush(stdout);
  int k = 0;
  double alpha, beta, aux;
  double *r, *r_ant, *p, *s;

  // Alocar memoria
  r = (double*)calloc(N,sizeof(double));
  r_ant = (double*)calloc(N,sizeof(double));
  p = (double*)calloc(N,sizeof(double));
  s = (double*)calloc(N,sizeof(double));

  // O primeiro residuo eh o vetor 'b'
	memcpy(r,b,sizeof(double)*N);

	// Faz o primeiro chute da solucao ser o vetor nulo
	memset(x,0,N*sizeof(double));

	// Metodo iterativo
	while (norm(r,N) > EPSILON && k < ITER_MAX)
  {
    k++;
		aux = dotProduct(r,r,N);

    if (k == 1)
      memcpy(p,r,sizeof(double)*N);
    else
    {
      beta = aux / dotProduct(r_ant,r_ant,N);
      saxpy2(p,r,beta,N);
    }
    // Copia o ultimo vetor 'r'
    memcpy(r_ant,r,sizeof(double)*N);

    matvec(A,p,s,N);
    alpha = aux / dotProduct(p,s,N);
    saxpy(x,p,alpha,N);
    saxpy(r,s,-alpha,N);
  }
  //printf("ok\n");
	//printf("k = %d || Norm = %e\n",k,norm(r,N));

  free(r);
  free(r_ant);
  free(p);
  free(s);
}

// Funcao que calcula o produto interno entre dois vetores 'a' e 'b' de tamanho 'n'
double dotProduct (double *a, double *b, int n)
{
  int i;
  double dot = 0.0;
  for (i = 0; i < n; i++)
    dot += a[i]*b[i];
  return dot;
}

// Funcao que calcula SAXPY entre dois vetores 'x' e 'y' e utilizando um escalar 'alpha'
// x_i = x_i + alpha*y_i
void saxpy (double *x, double *y, double alpha, int n)
{
  int i;
  for (i = 0; i < n; i++)
    x[i] += y[i]*alpha;
}

// Funcao que calcula SAXPY entre dois vetores 'x' e 'y' e utilizando um escalar 'alpha'
// x_i = y_i + alpha*x_i
void saxpy2 (double *x, double *y, double alpha, int n)
{
  int i;
  for (i = 0; i < n; i++)
    x[i] = y[i] + alpha*x[i];
}

// Funcao que calcula a multiplicacao matriz-vetor (x = A*b) usando a representacao de matriz esparsa CSR
void matvec (double *A, double *b, double *x, int n)
{
  int i, j;
  double sum;
  for (i = 0; i < n; i++)
  {
    sum = 0;
    for (j = 0; j < n; j++)
      sum += A[i*n+j]*b[j];
    x[i] = sum;
  }
}

// Funcao que calcula a norma euclidiana de um vetor 'v' de tamanho 'n'
double norm (double *v, int n)
{
  int i;
  double norm = 0.0;
  for (i = 0; i < n; i++)
    norm += v[i]*v[i];
  return sqrt(norm);
}

double checkSystem (double *A, double *b, double *x, int N)
{
  int i, j;
  double sum;
  double norm = 0;
  for (i = 0; i < N; i++)
  {
    sum = 0;
    for (j = 0; j < N; j++)
    {
      sum += A[i*N+j]*x[j];
    }
    printf("%e\n",b[i]-sum);
    norm += pow(b[i]-sum,2);
  }
  printf("Norma do sistema = %e\n",sqrt(norm));
  return sqrt(norm);
}

void LUDecomposition (double *A, int N)
{
  printf("[!] Fazendo decomposicao LU ... ");
  fflush(stdout);
  int i, j, k, p;
	pivot = (double*)calloc(N,sizeof(double));
	double Amax, t, aux, r, Mult;
	// 1 PASSO: Transformar a matriz A do problema em duas matrizes triangulares L e U.
	for (i = 0; i < N; i++)
		pivot[i] = i;
	for (j = 0; j < N-1; j++)
	{
		// Escolher pivot
		p = j;
		Amax = fabs(A[j*N+j]);
		// Verifica na coluna a ser eliminada qual elemento possui o maior valor absoluto, este elemento será o pivô.
		for (k = j+1; k < N; k++)
		{
			if (fabs(A[k*N+j]) > Amax)
			{
				Amax = fabs(A[k*N+j]);
				p = k;
			}
		}
		// Se (p != j) então deve-se trocar de linhas
		if (p != j)
		{
			for (k = 0; k < N; k++)
			{
				t = A[j*N+k];
				A[j*N+k] = A[p*N+k];
				A[p*N+k] = t;
			}
			aux = pivot[j];
			pivot[j] = pivot[p];
			pivot[p] = aux;
		}
		if (fabs(A[j*N+j]) != 0)
		{
			// Eliminação de Gauss
			r = 1 / A[j*N+j];
			for (i = j+1; i < N; i++)
			{
				Mult = A[i*N+j]*r;
				A[i*N+j] = Mult;
				for (k = j+1; k < N; k++)
					A[i*N+k] = A[i*N+k] - Mult*A[j*N+k];
			}
		}
	}
  printf("ok\n");
}

void solveLinearSystem_LU (double *A, double *b, double *x, int N)
{
  int i, j, k;
  double y[N];
	double soma;
	k = pivot[0];
	y[0] = b[k];
	// Realizar substituicoes progressivas para resolver o sistema triangular inferior: Ly = b
	for (i = 1; i < N; i++)
	{
		soma = 0;
		for (j = 0; j <= i-1; j++)
		{
			soma += A[i*N+j]*y[j];
		}
		k = pivot[i];
		y[i] = b[k] - soma;
	}
	// Realizar substituicoes retroativas para resolver o sistema triangular superior: Ux = y
	x[N-1] = y[N-1] / A[(N-1)*N+(N-1)];
	for (i = N-2; i >= 0; i--)
	{
		soma = 0;
		for (j = i+1; j < N; j++)
			soma += A[i*N+j]*x[j];
		x[i] = (y[i] - soma) / A[i*N+i];
    //if (isnan(x[i]))
    //{
     // printf("Error\n");
     // exit(1);
    //}
	}
}

// Escreve a matriz da decomposicao LU em um arquivo separado
void writeLUDecomposition (double *A, int N)
{
  FILE *out = fopen("Output/lu.dat","w+");
  // Escrever a matriz
  fprintf(out,"%d\n",N);
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
      fprintf(out,"%.10lf ",A[i*N+j]);
    fprintf(out,"\n");
  }
  // Escrever o vetor pivot
  for (int i = 0; i < N; i++)
    fprintf(out,"%.10lf\n",pivot[i]);
  fclose(out);
}

double* readLUDecompositionFromFile (char filename[])
{
  printf("[!] Lendo decomposicao LU de arquivo ... ");
  fflush(stdout);
  int N;
  double *A;
  FILE *in = fopen(filename,"r");
  if (!fscanf(in,"%d",&N)) Error("Reading file!");
  A = (double*)calloc(N*N,sizeof(double));
  pivot = (double*)calloc(N,sizeof(double));
  // Ler a matriz
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
      if (!fscanf(in,"%lf",&A[i*N+j])) Error("Reading file!");
  }
  // Ler o pivot
  for (int i = 0; i < N; i++)
      if (!fscanf(in,"%lf",&pivot[i])) Error("Reading file!");
  fclose(in);
  printf("ok\n");

  return A;
}

void Error (const char *msg)
{
  printf("[-] ERROR ! %s\n",msg);
  exit(-1);
}