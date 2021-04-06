#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

/*
- Matriz de entrada
- Vetor de entrada
- Vetor de saida
*/
//Usando o escopo global para facilitar o acesso multithread
float *mat1; 
float *mat2;
float *saida;
int nthreads; //numero de threads

//funcao que as threads executarao
typedef struct {
	int id; //identificador do elemen que a thread ira processar
	int dim; //dimensao das estruturas de entrada}
	} tArgs;
	
void * tarefa(void *arg) {
	tArgs *args = (tArgs*) arg;
	//printf("Thread %d\n", args->id);
	for(int i = args->id; i < args->dim ; i+=nthreads) {
		for(int j=0;j<args->dim;j++) {
			for(int k=0; k < args->dim; k++) { 
				saida[i * (args->dim) + j] += mat1[i*(args->dim)+k]*mat2[k * (args->dim) + i]; 
				}
			}
	}	
	pthread_exit(NULL);
		
	}



	
int main(int argc, char * argv[]) {
	int dim; //dimensao da matriz de entrada
	pthread_t *tid; //identificadores das threads
	tArgs *args; //identificadores locais das threads e dimensao
	double inicio, fim, delta;
	
	GET_TIME(inicio);
	//leitura e avalia;ao dos parametros de entrada
	if(argc<3) { 
		printf("Digite: %s <dimensao da matriz><n. threads>\n", argv[0]);
		return 1; }
	
	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]);
	if (nthreads > dim) nthreads=dim;
		
	//alocacao de memoria para as estruturas de dados
	mat1 = (float *) malloc(sizeof(float) * dim * dim);
	if (mat1 == NULL) { printf("ERRO -- malloc\n"); return 2; }
	mat2 = (float *) malloc(sizeof(float) * dim * dim);
	if (mat2 == NULL) { printf("ERRO -- malloc\n"); return 2; }
	saida = (float *) malloc(sizeof(float) * dim * dim);
	if (saida == NULL) { printf("ERRO -- malloc\n"); return 2; }		
	//inicializacao das estruturas de dados
	for(int i=0; i<dim ; i++) {
		for(int j=0; j<dim; j++) {
			mat1[i*dim+j] = i+2; //equivalente mat1[i][j]
			mat2[i*dim+j] = 1;
		}
	}
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo inicializacao: %lf\n", delta);
	//multiplicacao da matriz
	//alocacao das estruturas
	tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	if(tid==NULL) {puts("Erro--malloc"); return 2;}
	args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
	if(args==NULL) {puts("Erro--malloc"); return 2;}
	
	//criacao das threads
	GET_TIME(inicio);
	for(int i=0;i<nthreads;i++) {
		(args+i)->id = i;
		(args+i)->dim = dim;
		if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))) {puts("erro--pthread_create"); return 3;}
		
	}
	
	
	//espera pelo termino das threads
	for(int i=0;i<nthreads;i++) {
		pthread_join(*(tid+i), NULL);}
	GET_TIME(fim);
	delta += fim - inicio;
	printf("Tempo de multiplicacao: %lf\n", delta);
	
	//exibicao dos resultados
	/*
	puts("Matriz de entrada: ");
	for(int i=0; i<dim ; i++) {
		for(int j=0; j<dim; j++)
			printf("%.1f ", mat1[i*dim+j]);
		puts(""); 
	}
	puts("Matriz de entrada: ");
	for(int j=0; j<dim; j++)
		printf("%.1f ", mat2[j]);
	puts("");
	puts("mat2or de saida: ");
	for(int j=0; j<dim; j++)
		printf("%.1f ", saida[j]);
	puts("");*/
	
	//liberacao da memoria
	free(mat1); 
	free(mat2); 
	free(saida); 
	free(args); 
	free(tid);
	GET_TIME(fim);
	delta += fim - inicio;
	printf("Tempo finalizacao: %lf\n", delta);
	


	return 0;
	}
