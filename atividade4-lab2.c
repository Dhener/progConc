#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "timer.h"

// Função para verificar se um número é primo
bool is_prime(long long int n)
{
    if (n <= 1)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;

    for (long long int i = 3; (i-1) * (i-1) < n; i += 2)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

// Estrutura para armazenar os dados da thread
typedef struct
{
    long long int start;
    long long int end;
    long long int count;
} ThreadData;

// Função executada por cada thread
void *find_primes(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    data->count = 0;

    for (long long int i = data->start; i <= data->end; i++)
    {
        if (is_prime(i))
        {
            data->count++;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    double inicio, fim, delta;
    GET_TIME(inicio);
    if (argc != 3)
    {
        printf("Uso: %s <N> <threads>\n", argv[0]);
        return 1;
    }

    long long int N = atoll(argv[1]);
    int num_threads = atoi(argv[2]);

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    long long int chunk_size = N / num_threads;
    long long int start = 1;
    long long int end = chunk_size;

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de inicialização: %lf\n", delta);
    
    GET_TIME(inicio);

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].start = start;
        thread_data[i].end = end;

        if(pthread_create(&threads[i], NULL, find_primes, &thread_data[i])){
		printf("--ERRO: pthread_create()\n");
		exit(-1);
	}

        start = end + 1;
        end += chunk_size;
        if (i == num_threads - 2)
        {
            end = N;
        }
    }

    long long int total_primes = 0;
    
    // Espera todas as threads terminarem
    for (int i = 0; i < num_threads; i++)
    {
        if(pthread_join(threads[i], NULL)){
		printf("ERRO: pthread_join() \n");
		exit(-1);
	}
        total_primes += thread_data[i].count;
    }

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de concorrência: %lf\n", delta);

    printf("Total de números primos encontrados: %lld\n", total_primes);

    return 0;
}
