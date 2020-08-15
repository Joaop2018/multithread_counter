/* Data de submissao: 15/08/20
   Nome: João Pedro Martins Ferreira
   RA: 218762*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

/* define o numero maximo de threads em paralelo*/ 
#define N_THREADS 4

/* definicao dos estados da threadpool */
#define NOT_ASSIGNED 0 /* nao atribuido */
#define ASSIGNED 1 /* atribuido */
#define FINISHED 2 /* finalizado */

/* variaveis globais */
int res=0; /* varivael quantidade de numeros primos*/
int *mem; /* ponteiro threadpool */
int *v_entrada; /* ponteiro vetor de entrada */
int tam_vet = 0; /* tamanho do vetor de entrada */ 

/* definicao das travas do espaco de memoria */
pthread_mutex_t trava;

/* funcao para encontrar primo
   retorna 1 se primo e 0 se nao for primo*/
int primo(int num) {
  int div=0;
  for (int i = 1; i <= num; i++) {
  	if (num % i == 0) { 
	div++;
   	}
  }	 
  if (div == 2) return 1;
  else return 0;
}

void* worker(void *arg) {
  /* inicializacao: guardar o proprio numero da thread */
  int* N = (int*)(arg);
  int M = (*N);
  int instancia=0; /* a instancia do problema a ser trabalhado */
  while (1) {
    /* busca por uma instancia do problema que ainda nao foi atribuida ou finalizada */
    pthread_mutex_lock(&trava);

    while ( (mem[instancia]!=NOT_ASSIGNED) && (instancia < tam_vet)) {
      instancia++;
    }
    if (instancia >= tam_vet) { /* acabaram as instancias, programa finalizado */
      /* antes de sair do loop, a trava e liberada */
      pthread_mutex_unlock(&trava);
      break;
    }
    mem[instancia] = ASSIGNED; /* resolve a instancia encontrada */
    pthread_mutex_unlock(&trava);
    
    /* resolve o problema */
    res += primo(v_entrada[instancia]);
    
    /* agora estado no threadpool e atualizado para finalizado */
    pthread_mutex_lock(&trava);
    mem[instancia] = FINISHED;
    pthread_mutex_unlock(&trava);
  }
  return NULL;
}

int main() {
  /* recebimento dos dados */
  v_entrada = (int*) malloc(sizeof(int)*100);
  int entrada;
  do {
    scanf("%d", &entrada);
    v_entrada[tam_vet] = entrada;
    tam_vet++;
  }
  while ((entrada=getchar()) != '\n');
  
  /* criacao da lista de threadpool */
  mem = (int*) malloc (sizeof(int)*100);
  for (int i=0; i<tam_vet; i++) {
    mem[tam_vet] = NOT_ASSIGNED; /* define todos os estados como nao atribuidos */
  } 
  
  /* definicao das threads */
  pthread_t workers[N_THREADS];
  int thread_id[N_THREADS];

  /* Disparando threads */
  for (int i=0; i<N_THREADS; i++) {
    thread_id[i] = i;	
    pthread_create(&(workers[i]), NULL, worker, (void*) (&thread_id[i]));
  }

  /* Esperando threads */
  for (int i=0; i<N_THREADS; i++) {
    pthread_join(workers[i], NULL);
  }
  printf("%d\n",res);
  return 0;
}
