//Soma todos os elementos de um vetor de inteiro
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <math.h>
#include <stdbool.h>
#include "timer.h"

long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
double *vetor; //vetor de entrada com dimensao dim 

//fluxo das threads
void * tarefa(void * arg) {
   long int id = (long int) arg; //identificador da thread
   double *somaLocal; //variavel local da soma de elementos
   somaLocal = (double*) malloc(sizeof(double));
   if(somaLocal==NULL) {exit(1);}
   long int tamBloco = dim/nthreads;  //tamanho do bloco de cada thread 
   long int ini = id * tamBloco; //elemento inicial do bloco da thread
   long int fim; //elemento final(nao processado) do bloco da thread
   if(id == nthreads-1) fim = dim;
   else fim = ini + tamBloco; //trata o resto se houver
   //soma os elementos do bloco da thread
   for(long int i=ini; i<fim; i++) {
      *somaLocal += vetor[i];
      }
   //retorna o resultado da soma local
   pthread_exit((void *) somaLocal); 
}

//fluxo principal
int main(int argc, char *argv[]) {
   double somaSeq= 0; //soma sequencial
   double somaConc= 0; //soma concorrente
   double ini, fim; //tomada de tempo
   pthread_t *tid; //identificadores das threads no sistema
   double *retorno; //valor de retorno das threads
   double tempoConc = 0, tempoSeq = 0;

   //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
   if(argc < 3) {
       fprintf(stderr, "Digite: %s <Numero de Elementos (N) da serie> <numero threads>\n", argv[0]);
       return 1; 
   }
   dim = atoi(argv[1]);
   nthreads = atoi(argv[2]);
   //aloca o vetor de entrada
   vetor = (double*) malloc(sizeof(double)*dim);
   if(vetor == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //preenche o vetor de entrada
   double j = 1; //denominador da fracao da serie
   bool sinal = true; //bool pra alternar os sinais conforme a serie 
   for(long int i=0; i<dim; i++) {
        if(sinal) {
     	   vetor[i] = 1/j;
     	   j = j + 2;
        }
        if(!sinal) {
     	   vetor[i] = -1/j;
     	   j = j + 2;
        }  
	sinal = sinal ? false : true; //pra realizar a alternancia de sinais
 }

   //soma sequencial dos elementos
   GET_TIME(ini);
   for(long int i=0; i<dim; i++)
      somaSeq += vetor[i];   
   somaSeq = somaSeq*4;
   GET_TIME(fim);
   tempoSeq = fim-ini;
   printf("Tempo sequencial: %.15lf\n", fim-ini);

   //soma concorrente dos elementos
   GET_TIME(ini);
   tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //criar as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
   }
   //aguardar o termino das threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), (void**) &retorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
      //soma global
      somaConc += *retorno;
   }
   somaConc = somaConc * 4;
   
   GET_TIME(fim);
   tempoConc = fim-ini;
   printf("Tempo concorrente:  %lf\n", fim-ini);

   //exibir os resultados
   printf("Soma seq:  %.15lf\n", somaSeq);
   printf("Soma conc: %.15lf\n", somaConc);
   printf("Pi do math.h = %.15lf \n", M_PI);
   printf("Ganho de desempenho = %.15lf, ou aproximadamente %.1lf%%\n", tempoSeq/tempoConc, 100*(tempoSeq/tempoConc));

   //libera as areas de memoria alocadas
   free(vetor);
   free(tid);

   return 0;
}
