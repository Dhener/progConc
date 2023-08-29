// Nome: Dhener Rosemiro Silva
// DRE: 121043412

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 8 // total de threads a serem criadas
#define N 10000	   // total de elementos no array

int a[N]; // vetor com N posições

typedef struct
{
	int thread_id;
	int inicio;
	int final;
} t_Args;

// Inicializando o vetor a
void initializeVector()
{
	for (int j = 0; j < N; j++)
	{
		a[j] = j;
	}
}

// funcao executada pelas threads
void *ElevarElementoAoQuadrado(void *arg)
{
	t_Args args = *(t_Args *)arg;
	args.inicio = args.thread_id * (N / NTHREADS);
	args.final = args.inicio + (N / NTHREADS);
	for (int i = args.inicio; i < args.final; i++)
	{
		a[i] = a[i] * a[i];
	}
	free(arg);
	pthread_exit(NULL);
}

// verificanfo o resultado no vetor a
void verificarResultado()
{
	for (int i = 0; i < N; i++)
	{
		if (a[i] != (i) * (i))
		{
			printf("Resultado incorreto!\n");
			return;
		}
	}
	printf("Resultado correto!\n");
}

// funcao principal do programa
int main(void)
{
	pthread_t tid_sistema[NTHREADS]; // identificadores das threads no sistema
	t_Args *args;					 // receberá os argumentos para a thread

	initializeVector();

	for (int i = 0; i < NTHREADS; i++)
	{
		printf("--Cria a thread %d\n", i);
		args = malloc(sizeof(t_Args));
		if (args == NULL)
		{
			printf("--ERRO: malloc()\n");
			exit(-1);
		}

		args->thread_id = i;
		if (pthread_create(&tid_sistema[i], NULL, ElevarElementoAoQuadrado, (void *)args))
		{
			printf("--ERRO: pthread_create()\n");
			exit(-1);
		}
	}

	//--espera todas as threads terminarem
	for (int i = 0; i < NTHREADS; i++)
	{
		if (pthread_join(tid_sistema[i], NULL))
		{
			printf("--ERRO: pthread_join() \n");
			exit(-1);
		}
	}

	printf("--Thread principal terminou\n");

	verificarResultado();

	pthread_exit(NULL);
}
