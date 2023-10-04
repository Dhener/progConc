#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_LINES 100       // Define um tamanho máximo para o buffer
#define MAX_LINE_LENGTH 100 // Define um tamanho máximo para as linhas do arquivo

// Estrutura para o buffer compartilhado
typedef struct
{
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    sem_t mutex, full, empty;
} Buffer;

Buffer buffer;

// Função para inserir uma linha no buffer (produtor)
void insertLine(char *line)
{
    static int in = 0;
    sem_wait(&buffer.empty);
    sem_wait(&buffer.mutex);

    strcpy(buffer.lines[in], line);
    in = (in + 1) % MAX_LINES;

    sem_post(&buffer.mutex);
    sem_post(&buffer.full);
}

// Função para retirar uma linha do buffer e imprimi-la (consumidor)
void consumeLine()
{
    static int out = 0;
    sem_wait(&buffer.full);
    sem_wait(&buffer.mutex);

    printf("%s", buffer.lines[out]);
    out = (out + 1) % MAX_LINES;

    sem_post(&buffer.mutex);
    sem_post(&buffer.empty);
}

// Função que cada thread consumidora irá executar
void *consumer(void *arg)
{
    while (1)
    {
        consumeLine();
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Uso: %s <arquivo_de_entrada> <N_consumidores>\n", argv[0]);
        exit(1);
    }

    // Inicializa o buffer compartilhado e os semáforos
    sem_init(&buffer.mutex, 0, 1);
    sem_init(&buffer.full, 0, 0);
    sem_init(&buffer.empty, 0, MAX_LINES);

    // Abre o arquivo de entrada
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    // Cria threads consumidoras
    int N = atoi(argv[2]);
    pthread_t consumers[N];
    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&consumers[i], NULL, consumer, NULL))
        {
            printf("Erro na criacao do thread consumidor\n");
            exit(1);
        }
    }

    // Lê o arquivo linha a linha e insere no buffer
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        insertLine(line);
    }

    // Aguarda as threads consumidoras terminarem
    for (int i = 0; i < N; i++)
    {
        if (pthread_join(consumers[i], NULL))
        {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
    }

    // Libera recursos e fecha o arquivo
    fclose(file);
    sem_destroy(&buffer.mutex);
    sem_destroy(&buffer.full);
    sem_destroy(&buffer.empty);

    return 0;
}
