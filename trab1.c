/* Trabalho 1 de Computação Concorrente. */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
#include<stdbool.h>

int *vet; //vetor de entrada
long long int tVetor; //tamanho do vetor escolhido pelo usuario
int nthreads; //numero de threads
long long int nBusca; //numero a ser buscado
bool achou = false;
pthread_mutex_t mutex;


//funcao que as threads executarao
void * tarefa(void *arg) {
   long int id = (long int) arg; //identificador da thread
   long int tamBloco = tVetor/nthreads;  //tamanho do bloco de cada thread 
   long int ini = id * tamBloco; //elemento inicial do bloco da thread
   long int fim; //elemento final(nao processado) do bloco da thread
   if(id == nthreads-1) fim = tVetor;
   else fim = ini + tamBloco; //trata o resto se houver
   for(long int i=ini; i<fim; i++){ 
   //for(int i=args->id; i<args->tVetor; i+=nthreads)
   
        if(vet[i] == nBusca){
        	achou = true;
        	printf("Numero %lld achado na posicao %ld\n", nBusca, i);
        }	
	
   }     	
   pthread_exit(NULL);
   
}

//Dando aleatoriedade para a entrada
  void troca(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
  }

void randomize(int vetor[], int tamanho) {
    srand(time(NULL));
    int i;
    for(i = tamanho-1; i > 0; i--) {
        int j = rand() % (i+1);
        troca(&vetor[i], &vetor[j]);
    }
}

//fluxo principal
int main(int argc, char* argv[]) {
   pthread_t *tid; //identificadores das threads no sistema
   double inicio, fim, delta; // variaveis para calculo de tempo
   int escolha;
      

   //leitura e avaliacao dos parametros de entrada
   if(argc<4) {
      printf("Digite: %s <numero inteiro a ser buscado de 1 ao numero do vetor> <tamanho do vetor> <numero de threads>\n", argv[0]);
      return 1;
   }
   nBusca = atoi(argv[1]);
   tVetor = atoi(argv[2]);
   nthreads = atoi(argv[3]);
   if (nthreads > tVetor) nthreads=tVetor;

  


   GET_TIME(inicio);  
   //alocacao das estruturas
   vet = (int *) malloc(sizeof(int)* tVetor);
   if(vet==NULL) {puts("ERRO--malloc"); return 2;}
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}


 
  //inicializacao das estruturas de dados de entrada e saida
   printf("Inicializando vetor...\n");
   for(int i=0; i<tVetor; i++) {     
      vet[i] = i+1;
   }
  randomize(vet, tVetor);
  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo de inicializacao e randomizacao do vetor: %lf\n", delta);
  printf("\n");
  
  //Opcao para forcar o pior caso para uma melhor medicao dos ganhos
  printf("Forcar pior caso? <1> Sim <2> Nao\n");
  scanf("%d", &escolha);
  if(escolha == 1) {nBusca = vet[tVetor-1];}

  	
 /* printf("{ ");
   for(int i=0; i<tVetor; i++) {     
     printf("[%d] ", vet[i]);
  }
  printf("}");
   printf("\n");*/ 
 
   //criacao das threads
   GET_TIME(inicio);
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   //espera pelo termino da threads
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   if(!achou){
	printf("Numero nao existente\n");
	}
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("\n");
   printf("Tempo de Busca: %lf\n", delta);

   //exibicao dos resultados
   /*puts("Vetor de saida:");
   for(int j=0; j<dim; j++)
      printf("%.1f ", saida[j]);
   puts("");
   */

   //liberacao da memoria
   GET_TIME(inicio);
   free(vet);
   free(tid);
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("\n");
   printf("Tempo finalizacao:%lf\n", delta);

   return 0;
}
