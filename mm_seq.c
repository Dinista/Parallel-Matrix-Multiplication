#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#define false 0
#define true 1

typedef int bool;

int **matriz_A, **matriz_B, **matriz_result;


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

/* Multiplica as matrizes */

void multi_matriz(int matriz_tamanho){
    int i, j, k;
    for (i = 0; i < matriz_tamanho; i++){
        for (j = 0; j < matriz_tamanho; j++){
            matriz_result[i][j] = 0;
            for (k = 0; k < matriz_tamanho; k++){
				matriz_result[i][j] += matriz_A[i][k] * matriz_B[k][j];
			}
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

/* Menu de ajuda */

void help(){
    fprintf(stdout, "\nMenu Ajuda:\n"
                    "	-h: Mostra esse menu.\n"
                    "	-m: Tamanho da matriz.\n"
                    "	-v: Vizualiza matriz A e B, e matriz resultado.\n"
					"\nRequisitos:\n"
					"	1. -m e um argumento obrigatorio.\n");
}


int main(int argc, char **argv){
    int opt = -1;
	int matriz_tamanho = -1;
    bool view = false;
	
	/* Menu de opções */
	
    while ((opt = getopt(argc, argv, "m:vh")) != -1){
        switch (opt){
        case 'm':
            matriz_tamanho = strtoul(optarg, NULL, 0);
			break;
        case 'v':
            view = true;
            break;
        case 'h':
            help();
            break;
        case '?':
            if (optopt == 'm'){
				fprintf(stderr, "Opção -%c precisa de um argumento.\n", optopt);
			}
            else{
				fprintf(stderr, "Opção desconhecida `\\x%x'.\n", optopt);
			}
            return 1;
        default:
            abort();
        }
    }
	
	/* Erros de argumentos */
	
    if (matriz_tamanho == -1){
        fprintf(stderr, "Argumento -m é necessário.\n");
        exit(EXIT_FAILURE);
    }
    
    
	/* Criação das matrizes */
	
	cria_matriz(matriz_tamanho);
	
    
    /* Vizualizando matriz A e B */
	
    if (view){
        printf("\nMatriz A:\n");
		display(matriz_tamanho, matriz_A);
        printf("\nMatriz B:\n");
        display(matriz_tamanho, matriz_B);
    }

    /* Começando a contagem de tempo*/
    time_t start = time(NULL);

    
	multi_matriz(matriz_tamanho); /* Multiplicando as matrizes*/

    
	/* Finalizando a contagem de tempo*/
	time_t end = time(NULL);
	
	double spent = difftime(end, start);


    /* Vizualizando matriz resultado */

    if (view){
    	printf("\nMatriz resultado:\n");
    	display(matriz_tamanho, matriz_result);
	}

    printf("\nTempo total gasto: %.2f segundos.\n", spent);
    return 0;
}
