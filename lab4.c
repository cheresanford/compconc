/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread A */
void *thread1 (void *t) {
  int boba1, boba2;

  printf("thread1: Comecei\n");
  
  /* faz alguma coisa pra gastar tempo... */
  boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;
  
  printf("\n --------\nSeja bem-vindo!\n --------\n");
  pthread_mutex_lock(&x_mutex);
  x++;
  if (x==1) {
      printf("thread1:  x = %d, vai sinalizar a condicao \n", x);
      pthread_cond_broadcast(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread B */
void *thread2 (void *t) {
  printf("thread2: Comecei\n");
  
  pthread_mutex_lock(&x_mutex);
  if (x < 1) { 
     printf("thread2: x= %d, vai se bloquear...\n", x);
     pthread_cond_wait(&x_cond, &x_mutex);
     printf("thread2: sinal recebido e mutex realocado, x = %d\n", x);
     printf("thread2:  x = %d, vai sinalizar a condicao \n", x);
     pthread_cond_signal(&x_cond);
     
  }
  x++;
  printf("\n --------\nFique a vontade.\n --------\n");
  pthread_cond_signal(&x_cond);
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

void *thread3 (void *t) {
  printf("thread3: Comecei\n");
  
  pthread_mutex_lock(&x_mutex);
  if (x < 1) { 
     printf("thread3: x= %d, vai se bloquear...\n", x);
     pthread_cond_wait(&x_cond, &x_mutex);
     printf("thread3: sinal recebido e mutex realocado, x = %d\n", x);
     printf("thread3:  x = %d, vai sinalizar a condicao \n", x);
  }
  x++;
  printf("\n --------\nSente-se por favor.\n --------\n");
  pthread_cond_signal(&x_cond);
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

void *thread4 (void *t) {
  printf("thread4: Comecei\n");

  pthread_mutex_lock(&x_mutex);
  while (x < 3) { 
     printf("thread4: x= %d, vai se bloquear...\n", x);
     pthread_cond_wait(&x_cond, &x_mutex);
     printf("thread4: sinal recebido e mutex realocado, x = %d\n", x);
     
  }
  printf("\n --------\nVolte sempre!\n --------\n");
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}
/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[3], NULL, thread4, NULL);
  pthread_create(&threads[2], NULL, thread3, NULL);
  pthread_create(&threads[1], NULL, thread2, NULL);
  pthread_create(&threads[0], NULL, thread1, NULL);
 

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}
