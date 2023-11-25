#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/shm.h>

pthread_mutex_t my = PTHREAD_MUTEX_INITIALIZER;

int suma = 0;

typedef struct{
    int id;
    int cantidad;
    int **patron;
    int **vegetacion;
    int nHilos;
    int start;
    int end;
} Data;

int celdas(int **matrix, int i, int j){
    int cont = 0;
    //7
    if(matrix[i-1][j-1] ==7){
        cont++;
    }
    if(matrix[i-1][j+1]==7){
        cont++;
    }
    if(matrix[i+1][j-1]==7){
        cont++;
    }
    if(matrix[i+1][j+1]==7){
        cont++;
    }

    //5
    if(matrix[i-1][j]==5){
        cont++;
    }
    if(matrix[i+1][j]==5){
        cont++;
    }
    if(matrix[i][j-1]==5){
        cont++;
    }
    if(matrix[i][j+1]==5){
        cont++;
    }
    
    return cont;
}

void *funcionHilo(void *arg){
    Data *data = (Data *) arg;
    int rows = data->start+1, rowsEnd = data->end-1;
    if(data->nHilos != 1){
        if(data->id==0){
            rowsEnd = data->end;
        }else if(data->id == (data->nHilos-1)){
            rows = data->start;
        }else{
            rows = data->start;
            rowsEnd = data->end;
        }
    }
    
    for(int i = rows; i < rowsEnd; i++){
        for(int j = 1; j < 100-1; j++){
            if(data->vegetacion[i][j]==2){
                pthread_mutex_lock(&my);
                int nAdyacentes = celdas(data->vegetacion,i,j);
                if(nAdyacentes >= 8){
                        printf("(%d, %d)\n", i, j);
                        data->cantidad += 1;
                    
                }
                pthread_mutex_unlock(&my);
            }
        }
    }

    pthread_mutex_lock(&my);
    suma += data->cantidad;
    pthread_mutex_unlock(&my);

    pthread_exit(NULL);
}



int main(int argc, char** argv){
    FILE *file;
    int filas, columnas, i, j;

    file = fopen("patron.txt", "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Leer las dimensiones del patrón
    fscanf(file, "%d", &filas);
    fscanf(file, "%d", &columnas);

    // Crear una matriz para almacenar el patrón
    int **patron = (int **)malloc(filas * sizeof(int *));
    for (i = 0; i < filas; i++) {
        patron[i] = (int *)malloc(columnas * sizeof(int));
    }

    // Leer los valores del patrón
    for (i = 0; i < filas; i++) {
        for (j = 0; j < columnas; j++) {
            fscanf(file, "%d", &patron[i][j]);
        }
    }
    
    printf("Patrón:\n");
    for (i = 0; i < filas; i++) {
        for (j = 0; j < columnas; j++) {
            printf("%d ", patron[i][j]);
        }
        printf("\n");
    }

    fclose(file);

    file = fopen("matriz_vegetacion.txt", "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    fscanf(file, "%d", &filas);
    fscanf(file, "%d", &columnas);

    // Crear una matriz para almacenar vegetacion
    int **vegetacion = (int **)malloc(filas * sizeof(int *));
    for (i = 0; i < filas; i++) {
        vegetacion[i] = (int *)malloc(columnas * sizeof(int));
    }

    // Leer los valores de vegetacion
    for (i = 0; i < filas; i++) {
        for (j = 0; j < columnas; j++) {
            fscanf(file, "%d", &vegetacion[i][j]);
        }
    }
    

    int nHilos = atoi(argv[1]);
    pthread_t hilos[nHilos];

    Data *data = (Data *)malloc(nHilos * sizeof(Data));
    for(int i = 0; i<nHilos; i++){
        data[i].id = i;
        data[i].patron = patron;
        data[i].nHilos = nHilos;
        data[i].vegetacion = vegetacion;
        data[i].start = i*filas/nHilos;
        data[i].end = (i+1)*filas/nHilos;
    }

    for (int i = 0; i < nHilos; i++) {        
        pthread_create(&hilos[i], NULL, funcionHilo, (void*)&data[i]);
    }

    for (int i = 0; i < nHilos; ++i) {
        pthread_join(hilos[i], NULL);
    }

    printf("suma %d\n", suma);

    free(data);

    return EXIT_SUCCESS;
}
