#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// parámetros iniciales
#define TAM_POBLACION 50
#define MAX_EVALUACIONES 100000
#define NUM_EJECUCIONES 11
#define NUM_DATASETS 6

typedef struct {
    int peso;
    int valor;
} Objeto;

int evaluar(Objeto* objetos, int n, int W, int* solucion) {
    int peso_total = 0, valor_total = 0;
    for (int i = 0; i < n; i++) {
        if (solucion[i]) {
            peso_total += objetos[i].peso;
            valor_total += objetos[i].valor;
            if (peso_total > W) {
                return valor_total - (peso_total - W) * 10;
            }
        }
    }
    return valor_total;
}

void inicializar_poblacion(int** poblacion, int n) {
    for (int i = 0; i < TAM_POBLACION; i++) {
        for (int j = 0; j < n; j++) {
            poblacion[i][j] = rand() % 2;
        }
    }
}

void seleccion(int** poblacion, int* fitness, int** nueva_poblacion, int n, int TORNEO_SIZE) {
    for (int i = 0; i < TAM_POBLACION; i++) {
        int mejor = rand() % TAM_POBLACION;
        for (int j = 1; j < TORNEO_SIZE; j++) {
            int oponente = rand() % TAM_POBLACION;
            if (fitness[oponente] > fitness[mejor]) {
                mejor = oponente;
            }
        }
        memcpy(nueva_poblacion[i], poblacion[mejor], n * sizeof(int));
    }
}

void crossover(int* padre1, int* padre2, int* hijo1, int* hijo2, int n) {
    int punto_cruce = rand() % n;
    for (int i = 0; i < n; i++) {
        if (i < punto_cruce) {
            hijo1[i] = padre1[i];
            hijo2[i] = padre2[i];
        } else {
            hijo1[i] = padre2[i];
            hijo2[i] = padre1[i];
        }
    }
}

void mutacion(int* individuo, int n, double PROB_MUTACION) {
    for (int i = 0; i < n; i++) {
        if ((double)rand() / RAND_MAX < PROB_MUTACION) {
            individuo[i] = 1 - individuo[i];
        }
    }
}

int leerDatos(Objeto **objetos, const char *filename, int *capacidad) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error al abrir el archivo %s\n", filename);
        return 0;
    }
    int n, max_peso;
    fscanf(file, "%d %d", &n, &max_peso);
    *objetos = (Objeto *)malloc(n * sizeof(Objeto));
    *capacidad = max_peso;
    for (int i = 0; i < n; i++) {
        fscanf(file, "%d %d", &(*objetos)[i].valor, &(*objetos)[i].peso);
    }
    fclose(file);
    return n;
}

int main() {
    srand(time(NULL));
    char *files[NUM_DATASETS] = {
        "f1_l-d_kp_10_269.txt", 
        "f2_l-d_kp_20_878.txt", 
        "f8_l-d_kp_23_10000.txt", 
        "knapPI_1_100_1000_1.txt", 
        "knapPI_2_100_1000_1.txt", 
        "knapPI_3_100_1000_1.txt"
    };
    int optima[NUM_DATASETS] = {295, 1024, 9767, 9147, 1514, 2397};

    // Parámetros modificables
    double PROB_MUTACION;
    int TORNEO_SIZE;

    // Lista de pruebas
    double prob_mutaciones[] = {0.05, 0.1, 0.2};
    int tamano_torneos[] = {2, 3, 4};

    for (int file_idx = 0; file_idx < NUM_DATASETS; file_idx++) {
        Objeto *objetos;
        int capacidad, n = leerDatos(&objetos, files[file_idx], &capacidad);
        if (n == 0) continue;

        for (int pm_idx = 0; pm_idx < 3; pm_idx++) {
            for (int ts_idx = 0; ts_idx < 3; ts_idx++) {
                PROB_MUTACION = prob_mutaciones[pm_idx];
                TORNEO_SIZE = tamano_torneos[ts_idx];

                printf("Prueba con PROB_MUTACION = %.2f y TORNEO_SIZE = %d para el archivo %s\n", PROB_MUTACION, TORNEO_SIZE, files[file_idx]);

                double total_gap = 0.0;
                for (int ejecucion = 0; ejecucion < NUM_EJECUCIONES; ejecucion++) {
                    int** poblacion = (int**)malloc(TAM_POBLACION * sizeof(int*));
                    for (int i = 0; i < TAM_POBLACION; i++) {
                        poblacion[i] = (int*)malloc(n * sizeof(int));
                    }
                    inicializar_poblacion(poblacion, n);
                    int* fitness = (int*)malloc(TAM_POBLACION * sizeof(int));
                    int mejor_valor = 0;
                    int contador_evaluaciones = 0;

                    while (contador_evaluaciones < MAX_EVALUACIONES) {
                        for (int i = 0; i < TAM_POBLACION; i++) {
                            fitness[i] = evaluar(objetos, n, capacidad, poblacion[i]);
                            if (fitness[i] > mejor_valor) {
                                mejor_valor = fitness[i];
                            }
                        }
                        int** nueva_poblacion = (int**)malloc(TAM_POBLACION * sizeof(int*));
                        for (int i = 0; i < TAM_POBLACION; i++) {
                            nueva_poblacion[i] = (int*)malloc(n * sizeof(int));
                        }
                        seleccion(poblacion, fitness, nueva_poblacion, n, TORNEO_SIZE);
                        for (int i = 0; i < TAM_POBLACION; i += 2) {
                            crossover(nueva_poblacion[i], nueva_poblacion[i + 1], poblacion[i], poblacion[i + 1], n);
                        }
                        for (int i = 0; i < TAM_POBLACION; i++) {
                            mutacion(poblacion[i], n, PROB_MUTACION);
                        }
                        for (int i = 0; i < TAM_POBLACION; i++) {
                            free(nueva_poblacion[i]);
                        }
                        free(nueva_poblacion);
                        contador_evaluaciones += TAM_POBLACION;
                    }

                    double gap = ((double)(optima[file_idx] - mejor_valor) / optima[file_idx]) * 100.0;
                    total_gap += gap;
                    printf("Ejecución %d, Mejor Valor: %d, GAP: %.2f%%, PROB_MUTACION = %.2f, TORNEO_SIZE = %d\n", 
                            ejecucion, mejor_valor, gap, PROB_MUTACION, TORNEO_SIZE);

                    for (int i = 0; i < TAM_POBLACION; i++) {
                        free(poblacion[i]);
                    }
                    free(poblacion);
                    free(fitness);
                }
                printf("Promedio GAP para %s: %.2f%% (PROB_MUTACION = %.2f, TORNEO_SIZE = %d)\n", 
                        files[file_idx], total_gap / NUM_EJECUCIONES, PROB_MUTACION, TORNEO_SIZE);
            }
        }
        free(objetos);
    }
    return 0;
}