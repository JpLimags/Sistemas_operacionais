#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_LEITORES 5
#define NUM_ESCRITORES 3

int leitores_count = 0;
sem_t mutex, escrita, leitura;

void *leitor(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sleep(1);

        sem_wait(&leitura);
        sem_wait(&mutex);
        leitores_count++;
        if (leitores_count == 1)
            sem_wait(&escrita);
        sem_post(&mutex);
        sem_post(&leitura);

        printf("Leitor %d está lendo...\n", id);
        sleep(2); 

        sem_wait(&mutex);
        leitores_count--;
        if (leitores_count == 0)
            sem_post(&escrita);
        sem_post(&mutex);
    }
}

void *escritor(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sleep(2);
        sem_wait(&mutex);
        sem_wait(&leitura);
        sem_wait(&escrita);

        printf("Escritor %d está escrevendo...\n", id);
        sleep(2); 
        sem_post(&escrita);
        sem_post(&leitura);
        sem_post(&mutex);
    }
}

int main() {
    pthread_t leitores[NUM_LEITORES], escritores[NUM_ESCRITORES];
    int ids_leitores[NUM_LEITORES], ids_escritores[NUM_ESCRITORES];

    sem_init(&mutex, 0, 1);
    sem_init(&escrita, 0, 1);
    sem_init(&leitura, 0, 1);

    for (int i = 0; i < NUM_LEITORES; i++) {
        ids_leitores[i] = i;
        pthread_create(&leitores[i], NULL, leitor, (void *)&ids_leitores[i]);
    }

    for (int i = 0; i < NUM_ESCRITORES; i++) {
        ids_escritores[i] = i;
        pthread_create(&escritores[i], NULL, escritor, (void *)&ids_escritores[i]);
    }

    sleep(20);

    
    for (int i = 0; i < NUM_LEITORES; i++) {
        pthread_cancel(leitores[i]);
    }

    for (int i = 0; i < NUM_ESCRITORES; i++) {
        pthread_cancel(escritores[i]);
    }

    sem_destroy(&mutex);
    sem_destroy(&escrita);
    sem_destroy(&leitura);

    return 0;
}
