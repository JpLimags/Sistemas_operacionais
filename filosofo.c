#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
#define PENSANDO 0
#define FAMINTO 1
#define COMENDO 2
#define GARFO_ESQ (filosofo_num + NUM_FILOSOFOS - 1) % NUM_FILOSOFOS
#define GARFO_DIR (filosofo_num + 1) % NUM_FILOSOFOS

int estado[NUM_FILOSOFOS];
pthread_mutex_t mutex;
pthread_cond_t condicao[NUM_FILOSOFOS];

void pensar(int filosofo_num) {
    printf("Filósofo %d está pensando.\n", filosofo_num);
    sleep(1);
}

void comer(int filosofo_num) {
    printf("Filósofo %d está comendo.\n", filosofo_num);
    sleep(1);
}

void testar(int filosofo_num) {
    if (estado[filosofo_num] == FAMINTO && estado[GARFO_ESQ] != COMENDO && estado[GARFO_DIR] != COMENDO) {
        estado[filosofo_num] = COMENDO;
        printf("Filósofo %d pegou os garfos e está comendo.\n", filosofo_num);
        pthread_cond_signal(&condicao[filosofo_num]);
    }
}

void pegar_garfos(int filosofo_num) {
    pthread_mutex_lock(&mutex);
    estado[filosofo_num] = FAMINTO;
    printf("Filósofo %d está faminto e tenta pegar os garfos.\n", filosofo_num);
    testar(filosofo_num);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_lock(&mutex);
    while (estado[filosofo_num] != COMENDO) {
        pthread_cond_wait(&condicao[filosofo_num], &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void devolver_garfos(int filosofo_num) {
    pthread_mutex_lock(&mutex);
    estado[filosofo_num] = PENSANDO;
    printf("Filósofo %d devolveu os garfos e está pensando.\n", filosofo_num);
    testar(GARFO_ESQ);
    testar(GARFO_DIR);
    pthread_mutex_unlock(&mutex);
}

void *filosofo(void *arg) {
    int filosofo_num = *(int *)arg;

    while (1) {
        pensar(filosofo_num);
        pegar_garfos(filosofo_num);
        comer(filosofo_num);
        devolver_garfos(filosofo_num);
    }
}

int main() {
    pthread_t filosofos[NUM_FILOSOFOS];
    int filosofo_num[NUM_FILOSOFOS];

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_cond_init(&condicao[i], NULL);
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        filosofo_num[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo, (void *)&filosofo_num[i]);
    }

    // Deixe a simulação rodar por 1000 execuções
    sleep(1000);

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_cancel(filosofos[i]);
    }

    pthread_mutex_destroy(&mutex);

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_cond_destroy(&condicao[i]);
    }

    return 0;
}
