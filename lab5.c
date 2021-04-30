#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

//variaveis do problema
int *vet, nthreads;
int salto = 1, sinaliza = 0;
pthread_cond_t cond;
pthread_mutex_t mutex;


//funcao auxiliar pra printar o vetor
void printaVetor(int *vetor, int nthreads) {
    printf("{ ");
    for (int i = 0; i < nthreads; i++){
        printf("[%d] ", vetor[i]);
    }
    printf("}\n");  
}


//tarefa principal
void *somaVet(void *arg){
    int *id = (int *) arg;
    int aux;

    pthread_mutex_lock(&mutex);
    //barreira
    while(*id >= salto) { 
        aux = vet[(*id)-salto];
        sinaliza++;
        if(sinaliza == (nthreads - salto)){
            sinaliza = 0; 
            pthread_cond_broadcast(&cond);
            
        } else {            
            pthread_cond_wait(&cond, &mutex);                       
        }
        vet[*id] = vet[*id] + aux;
        sinaliza++;
        
        if(sinaliza == (nthreads - salto)){                        
            sinaliza = 0;
            salto = salto*2;
            pthread_cond_broadcast(&cond);
        } else {  
            pthread_cond_wait(&cond, &mutex);          
        }              
    }
    pthread_mutex_unlock(&mutex); 
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    //verificando o numero de argumentos da entrada
    if(argc < 2) {
        puts("Insira o expoente de 2 na entrada.\n");
        return -1;
    }
    //estabelecendo a potencia para a entrada
    int potencia = atoi(argv[1]);
    nthreads = pow(2,potencia);
    
    //variaveis da biblioteca pthread
    pthread_t tid[nthreads];
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
    
    //alocando estruturas de dados
    vet = (int*) malloc(sizeof(int)*nthreads);
    int *vet2 = (int*) malloc(sizeof(int)*nthreads);
    /*salto = 1;
    sinaliza = 0;
*/
    //inicializando os vetores
    for (int i = 0; i < nthreads; i++){
        vet[i] = i;
        vet2[i] = i; //vetor para comparar com o vetor original no final
    }
    puts("Vetor original:");
    printaVetor(vet, nthreads);    
    
    //armazenando os valores que a tarefa ira colocar
    for (int i = 0; i < nthreads; i++){
        vet2[i] = vet2[i] + vet2[i-1];
    }

    //criacao das threads
    for (int i = 0; i < nthreads; i++) {
        int *id = (int*) malloc(sizeof(int));
        *id = i;
        if(pthread_create(&tid[i], NULL, somaVet, (void*) id)) { 
            puts("Erro ao criar as threads");
            return -2;
        }
    }
    //esperando as threads terminarem
    for (int i = 0; i < nthreads; i++){
        pthread_join(tid[i], NULL);
    }

    //comparando o vetor original com o vetor 2 para verificar a corretude	
    for (int i = 0; i < nthreads; i++){
        if(vet[i] != vet2[i]) {
            puts("Erro de soma!");
            return -3;
        }
    }
    //se chegou ate aqui, tudo deu certo
    puts("Vetor somado:");
    printaVetor(vet, nthreads);
    
    //liberacao de memoria
    free(vet);
    free(vet2);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    pthread_exit(NULL);

}
