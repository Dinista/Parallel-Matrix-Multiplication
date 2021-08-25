#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#define false 0
#define true 1

#define handle_error_en(en, msg)				\
  do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg)				\
  do { perror(msg); exit(EXIT_FAILURE); } while (0)


typedef int bool;

int **matriz_A, **matriz_B, **matriz_result;

/* Estrutura de dados */

struct matriz_div {
    int i_start;
	int i_end;
    int j_start;
	int j_end;
    int k_start;
	int k_end;
};

struct thread_info {     /* Argumentos usados na thread */
    pthread_t thread_id; /* ID retornado por pthread_create() */
    struct matriz_div info;    /* Estrutura matriz */
    int thread_num;      /* Definição da thread */
};

/* Cria matriz alocando espaço para mesma,
e preenche com números aleatórios */

void cria_matriz(int matriz_tamanho){
	
	int bytes = matriz_tamanho * (sizeof(int *));
	
	matriz_A = malloc(bytes);
    matriz_B = malloc(bytes);
    matriz_result = malloc(bytes);
	
	for(int i = 0; i < matriz_tamanho; i++){
        matriz_A[i] = malloc(bytes);
        matriz_B[i] = malloc(bytes);
        matriz_result[i] = malloc(bytes);
    }
    
    for (int i = 0; i < matriz_tamanho; i++){
        for (int j = 0; j < matriz_tamanho; j++) {
            matriz_A[i][j] = rand() % 10;
            matriz_B[i][j] = rand() % 10;
        }
    }
}

/* Printa as matrizes */

void display (int matriz_tamanho, int **matriz){
        for (int i = 0; i < matriz_tamanho; i++){
            for (int j = 0; j < matriz_tamanho; j++)
                printf("%d ", matriz[i][j]);
            printf("\n");
        }
}


/* Multiplica as matrizes */

void *multi_matriz(void *arg) {
    struct thread_info *tinfo = (struct thread_info *)arg;
    
    for (int i = tinfo->info.i_start; i < tinfo->info.i_end; i++){
        for (int j = tinfo->info.j_start; j < tinfo->info.j_end; j++){
            for (int k = tinfo->info.k_start; k < tinfo->info.k_end; k++){
                matriz_result[i][j] += matriz_A[i][k] * matriz_B[k][j];
        	}
		}
	}
}

/* Menu de ajuda */

void help(){
    fprintf(stdout, "\nMenu Ajuda:\n"
                    "	-m: Tamanho da matriz.\n"
                    "	-t: Numero de threads.\n"
                    "	-v: Vizualiza matriz A e B, e matriz resultado.\n"
					"	-h: Mostra Menu Ajuda.\n"
					"\nRequisitos:\n"
                    "	1. -m e -t sao argumentos obrigatorios.\n"
                    "	2. Tamanho da matriz precisa ser multiplo do numero de threads.\n"
					"	3. Numero de threads precisa ser no minimo 2.\n");
}


int main(int argc, char **argv){
    int opt;
	int num_threads = -1;
	int matriz_tamanho = -1;
    bool view = false;

    /* Menu de opções */
	
	while ((opt = getopt(argc, argv, "t:m:vh")) != -1){
        switch (opt){
        case 'm':
            matriz_tamanho = strtoul(optarg, NULL, 0);
            break;
        case 'v':
            view = true;
            break;
        case 't':
            num_threads = strtoul(optarg, NULL, 0);
            break;
        case 'h':
            help();
            break;
        case '?':
            if (optopt == 't' || optopt == 'm'){
				fprintf(stderr, "Opção -%c precisa de um argumento.\n", optopt);
			}
            else {
				fprintf(stderr, "Opção desconhecida `\\x%x'.\n", optopt);
			}
            break;
        default:
            abort();
        }
    }
	
	/* Erros de argumentos */
    if (matriz_tamanho == -1){
        fprintf(stderr, " Argumento -m é necessário.\n");
        exit(EXIT_FAILURE);
    }

    if (num_threads < 2){
        fprintf(stderr, "Numero de threads precisa ser pelo menos 2.\n");
        exit(EXIT_FAILURE);
    }

	/* Criação das matrizes */
	
    if (matriz_tamanho % num_threads != 0){
        fprintf(stderr, "O tamanho da matriz deve ser multiplo do numero de threads.\n");
        exit(EXIT_FAILURE);
    }
	else{
		cria_matriz(matriz_tamanho);
	}
	
	/* Vizualizando matriz A e B */
	
    if (view){
        printf("\nMatriz A:\n");
		display(matriz_tamanho, matriz_A);
        printf("\nMatriz B:\n");
        display(matriz_tamanho, matriz_B);
    }

    /* Iniciando criação da thread */

    pthread_attr_t attr;
    int s = pthread_attr_init(&attr);

    if (s != 0){
		handle_error_en(s, "pthread_attr_init");
	}

    /* Alocar memória para os argumentos do pthread_create() */

    struct thread_info *tinfo;
    
    tinfo = malloc(num_threads * sizeof(struct thread_info));
    
	/* Começando a contagem de tempo*/
    time_t start = time(NULL);

	/* Criando uma thread para cada bloco da matriz*/
    
    for (int tnum = 0; tnum < num_threads; tnum++){
        tinfo[tnum].thread_num = tnum + 1;
		
		struct matriz_div *info = &tinfo[tnum].info;
		int slice = matriz_tamanho / num_threads;
		
		info->k_start = tnum * slice;
    	info->k_end = info->k_start + slice;
    	info->j_start = 0;
    	info->j_end = matriz_tamanho;
    	info->i_start = 0;
    	info->i_end = matriz_tamanho;
		
        /* A chamada pthread_create () armazena o ID do thread
		no elemento correspondente de tinfo [] */

        int s = pthread_create(&tinfo[tnum].thread_id, &attr, &multi_matriz, &tinfo[tnum]);
        
		if (s != 0){
			handle_error_en(s, "pthread_create");
		}
    }

    /* Juntando cada thread */

    for (int tnum = 0; tnum < num_threads; tnum++){
        int s = pthread_join(tinfo[tnum].thread_id, NULL);
        if (s != 0){
			handle_error_en(s, "pthread_join");
		}
    }
	
	/* Finalizando a contagem de tempo*/
    time_t end = time(NULL);
	
	double spent = difftime(end, start);
    
	/* Vizualizando matriz resultado */

    if (view){
    	printf("\nMatriz resultado:\n");
    	display(matriz_tamanho, matriz_result);
    }

    printf("\nTempo total gasto: %.2f segundos.\n", spent);

    free(tinfo);
    return 0;
}
