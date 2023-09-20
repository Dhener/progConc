#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Variaveis globais */
int x = 0;
int num_threads;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

void *imprimeFrases(void *arg)
{
    // Função que imprime as frases
    int *my_id = (int *)arg; // variável forncerá a identificação de cada thread
    int again = 1;
    int vez = 1; // variável que dará a vez de imprimir a segunda frase e a terceira frase

    printf("Olá da thread %d\n", *my_id);
    pthread_mutex_lock(&x_mutex);

    while (again)
    {
        x++;                 // incrementa a cada vez que imprime a frase da vez
        if (x < num_threads) // testa se ainda possui threads imprimindo a frase da vez
        {
            pthread_cond_wait(&x_cond, &x_mutex);
        }

        // essa parte será executada quando alguma thread imprimir a frase da vez pela ultima vez
        // e liberará todas as threads bloqueadas

        x = 0; // zera a variável toda vez que
        pthread_cond_broadcast(&x_cond);
        if (vez == 1)
        {
            // imprime a segunda frase
            printf("Que dia bonito %d\n", *my_id);
        }
        else
        {
            // imprime a segunda frase
            printf("Ate breve da thread %d\n", *my_id);
            again = 0;
        }
        vez = 2; // passa a vez para a terceira frase
    }
    pthread_cond_broadcast(&x_cond);
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <numero de threads>\n", argv[0]);
        return 1;
    }

    num_threads = atoi(argv[1]);
    pthread_t threads[num_threads];
    int thread_ids[num_threads];
    // aloca espaco para os identificadores das threads

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    /* Cria as threads */
    for (int i = 0; i < num_threads; i++)
    {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, imprimeFrases, &thread_ids[i]);
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < num_threads; i++)
    {
        if (pthread_join(threads[i], NULL))
        {
            printf("ERRO: pthread_join() \n");
            exit(-1);
        }
    }
    printf("FIM.\n");

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);

    return 0;
}
