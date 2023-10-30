#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <sys/types.h>

int *vector = NULL;

int leer_datos(char *filename, int **v){
    int totalnumeros =0, temp;
    FILE *file = fopen(filename, "r");

    if(!file){
        printf("Error al abrir\n"); 
        exit(1);
    }
    fscanf(file, "%d", &totalnumeros);
    
    (*v) = (int *)calloc(totalnumeros, sizeof(int));
    
    if(!(*v)){
        printf("Error memoria\n"); 
        exit(2);
    }
    
    for(int i=0; i<totalnumeros; i++){
        fscanf(file, "%d", &temp);
        (*v)[i] = temp;
    } 
    return totalnumeros;
}

int main(int argc, char **argv){
    pid_t child[2], nieto[2];

    int tuberia[3][2];
    int tuberiaN[4][2];

    int tamanoDatos;

    int i, h;
    
    for(int k=0; k<3; k++){
        pipe(tuberia[k]);
    }

    for(int k = 0; k < 4; k++){
        pipe(tuberiaN[k]);
    }

    for(i = 0; i < 2; i++){
        child[i] = fork();
        if(!child[i]){
            if(i==1){
                for(h = 0; h < 2; h++){
                    nieto[h] = fork();
                    if(!nieto[h]){
                        break;
                    }
                }
            }
            break;
        }
    }

    //Es el padre
    if(i==2){
        close(tuberia[0][1]);
        close(tuberia[1][1]);
        close(tuberia[2][0]);

        //Cerramos tuberias nietos para padre
        for (int j = 0; j < 4; j++){
            close(tuberiaN[j][0]);
            close(tuberiaN[j][1]);
        }

        wait(NULL);

        read(tuberia[0][0], &tamanoDatos, sizeof(int));

        printf("El proceso padre recibio el tamaño: %d\n", tamanoDatos);
        //Recibir vector
        int buffer[tamanoDatos];
        read(tuberia[0][0], buffer, sizeof(buffer));

        printf("El padre envio el tamaño del archivo: %d\n", tamanoDatos);

        write(tuberia[2][1], &tamanoDatos, sizeof(int));
        //Enviar vector
        write(tuberia[2][1], buffer, sizeof(buffer));

        wait(NULL);

        read(tuberia[1][0], &tamanoDatos, sizeof(int));
        //Recibir vector
        read(tuberia[1][0], buffer, sizeof(buffer));

        printf("El proceso padre recibio el tamaño: %d\n", tamanoDatos);
        printf("El array final es: "); fflush(stdout);
        for(int j = 0; j < tamanoDatos; j++){
            printf("[%d] ", buffer[j]); fflush(stdout);
        }
        printf("\n");
    }
    //Hijos
    else{
        //Hijo 0
        if(i==0){
            for(int j = 1; j < 3; j++){
                close(tuberia[j][0]);
                close(tuberia[j][1]);
                //No break porque no hay hijos, no se duplica
            }

            //Cerramos tuberias nietos para hijo 0
            for (int j = 0; j < 4; j++){
                close(tuberiaN[j][0]);
                close(tuberiaN[j][1]);
            }

            close(tuberia[0][0]);

            tamanoDatos = leer_datos("./datos.txt", &vector);

            printf("Se envio el array: "); fflush(stdout);
            for(int j = 0; j < tamanoDatos; j++){
                printf("[%d] ", vector[j]); fflush(stdout);
            }
            printf("\n");
            printf("Hijo [%d] envio el tamaño del archivo: %d\n", i, tamanoDatos);

            write(tuberia[0][1], &tamanoDatos, sizeof(int));
            write(tuberia[0][1], vector, tamanoDatos*sizeof(int));
        }
        //Hijo 1
        if(i==1){
            if(h == 2){
                close(tuberia[0][0]);
                close(tuberia[0][1]);

                close(tuberia[1][0]);
                close(tuberia[2][1]);

                //Cerrar las tuberias para nietos
                close(tuberiaN[0][1]);
                close(tuberiaN[1][0]);
                close(tuberiaN[2][1]);
                close(tuberiaN[3][0]);

                read(tuberia[2][0], &tamanoDatos, sizeof(int));

                printf("El hijo [%d] recibio el tamaño: %d\n", i, tamanoDatos);

                int buffer[tamanoDatos];
                read(tuberia[2][0], buffer, sizeof(buffer));

                //Envia los datos a nieto 0 y nieto 1

                printf("Hijo [%d] envio el tamaño del archivo: %d\n", i, tamanoDatos);

                write(tuberiaN[1][1], &tamanoDatos, sizeof(int));
                write(tuberiaN[1][1], buffer, sizeof(buffer));
                write(tuberiaN[3][1], &tamanoDatos, sizeof(int));
                write(tuberiaN[3][1], buffer, sizeof(buffer));

                for(int j = 0; j < 2; j++){ wait(NULL); }

                //Recibe los datos del nieto 0 y nieto 1

                read(tuberiaN[0][0], &tamanoDatos, sizeof(int));
                read(tuberiaN[0][0], buffer, sizeof(buffer));
                read(tuberiaN[2][0], &tamanoDatos, sizeof(int));
                read(tuberiaN[2][0], buffer, sizeof(buffer));

                printf("El hijo [%d] recibio el tamaño: %d\n", i, tamanoDatos);

                //Envia los datos a padre
                printf("Hijo [%d] envio el tamaño del archivo: %d\n", i, tamanoDatos);

                write(tuberia[1][1], &tamanoDatos, sizeof(int));
            }
            //Nieto 0
            else if(h == 0){
                //Me cierra las tuberias que no son del nieto
                for(int j = 0; j < 3; j++){
                    close(tuberia[j][0]);
                    close(tuberia[j][1]);
                }

                for(int j = 2; j < 4; j++){
                    close(tuberiaN[j][0]);
                    close(tuberiaN[j][1]);
                }

                close(tuberiaN[0][0]);
                close(tuberiaN[1][1]);
                
                //Recibe datos de hijo 1
                read(tuberiaN[1][0], &tamanoDatos, sizeof(int));

                printf("El nieto [%d] recibio el tamaño: %d\n", h, tamanoDatos);

                int buffer[tamanoDatos];
                read(tuberiaN[1][0], buffer, sizeof(buffer));

                printf("Nieto [%d] envio el tamaño del archivo: %d\n", h, tamanoDatos);

                write(tuberiaN[0][1], &tamanoDatos, sizeof(int));
                write(tuberiaN[0][1], buffer, sizeof(buffer));
            }
            //Nieto 1
            else if(h == 1){
                //Me cierra las tuberias que no son del nieto
                for(int j = 0; j < 3; j++){
                    close(tuberia[j][0]);
                    close(tuberia[j][1]);
                }

                for(int j = 0; j < 2; j++){
                    close(tuberiaN[j][0]);
                    close(tuberiaN[j][1]);
                }

                close(tuberiaN[2][0]);
                close(tuberiaN[3][1]);

                //Recibe datos de hijo 1
                read(tuberiaN[3][0], &tamanoDatos, sizeof(int));

                int buffer[tamanoDatos];
                read(tuberiaN[3][0], buffer, sizeof(buffer));

                printf("El nieto [%d] recibio el tamaño: %d\n", h, tamanoDatos);
            
                printf("Nieto [%d] envio el tamaño del archivo: %d\n", h, tamanoDatos);

                write(tuberiaN[2][1], &tamanoDatos, sizeof(int));
                write(tuberiaN[2][1], buffer, sizeof(buffer));
            }
        }
    }

    return 0;
}
