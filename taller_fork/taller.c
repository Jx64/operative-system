#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int *vector = NULL;
int leer_datos(char *, int **);
void funcion_de_hijos(int, int, int);

int main(int argc, char **argcv){
    int totalhijos = atoi(argcv[1]), j;
    int tn = leer_datos(argcv[2], &vector);
    for (int i = 0; i < tn; i++) printf("%d\n", vector[i]);
    for (j = 0; j < totalhijos; j++) if(!fork()) break;
    if(j == totalhijos){
        for (int i = 0; i < totalhijos; i++) wait(NULL);
        FILE *f = fopen("out.txt", "r");
        if(!f) {printf("Error al abrir padre\n"); exit(4);}
        int sumatotal = 0, t;
        for (int i = 0; i < totalhijos; i++){
            fscanf(f, "%d", &t);
            sumatotal += t;
        }
        printf("Suma total es: %d\n", sumatotal);
        fclose(f);
        remove("out.txt");
    } else{
        int delta = tn/totalhijos;
        funcion_de_hijos(j, j*delta, (j*delta)+delta);
    }

    return 0;
}

int leer_datos(char *filename, int **v){
    int totalnumeros =0, temp;
    FILE *file = fopen(filename, "r");
    if(!file){printf("Error al abrir\n"); exit(1);}
    fscanf(file, "%d", &totalnumeros);
    (*v) = (int *)calloc(totalnumeros, sizeof(int));
    if(!(*v)){printf("Error memoria\n"); exit(2);}
    for(int i=0; i<totalnumeros; i++){
        fscanf(file, "%d", &temp);
        (*v)[i] = temp;
    } 
    return totalnumeros;
}

void funcion_de_hijos(int j, int a, int b){
    int sumaparcial = 0;
    FILE *f;
    for (int i = a; i < b; i++) sumaparcial += vector[i];
    printf("suma parcial hijo %d: %d\n", j,sumaparcial);
    f = fopen("out.txt", "a");
    if(!f) {printf("Error al abrir hijo\n"); exit(3);}
    fprintf(f, "%d\n", sumaparcial);
    fclose(f);
}