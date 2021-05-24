#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

sem_t cond1, cond2;


void *thread4(void *arg) {

    printf("Seja bem-vindo!\n");
    sem_post(&cond2); 

    pthread_exit(NULL);
}



void *thread3(void *arg) {

    sem_wait(&cond1);
    printf("Sente-se por favor.\n"); 
    sem_post(&cond2);

    pthread_exit(NULL);
}

void *thread2(void *arg) {

    sem_wait(&cond1);
    printf("Fique a vontade.\n");
    sem_post(&cond2);

    pthread_exit(NULL);
}

void *thread1(void *arg) { 
    sem_wait(&cond2);
    sem_post(&cond1);
    sem_wait(&cond2);
    sem_post(&cond1);
    sem_wait(&cond2);
    printf("Volte sempre!\n");     

    pthread_exit(NULL);
}

int main() {
    pthread_t tid[NTHREADS];  
    
    //inicializando os semaforos
    sem_init(&cond1, 0, 0);
    sem_init(&cond2, 0, 0);
    
    //inicializando as threads
    pthread_create(&tid[0], NULL, thread1, NULL);
    pthread_create(&tid[1], NULL, thread2, NULL);
    pthread_create(&tid[2], NULL, thread3, NULL);
    pthread_create(&tid[3], NULL, thread4, NULL);

    //aguardando o termino de todas as threads
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    
    //liberando os semaforos
    sem_destroy(&cond2);
    sem_destroy(&cond1);
}
    

