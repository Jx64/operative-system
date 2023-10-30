#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

int main(){
    pid_t hijo[2], nieto[2];

    int tuberia[4][2];

    int i;
    bool bandera = false;

    char *cadena;

    for(int t = 0; t < 4; t++){
        if(pipe(tuberia[t]) == -1){
            perror("Error al crear tuberia");
            exit(1);
        }
    }

    for(i = 0; i < 2; i++){
        hijo[i] = fork();
        if(!hijo[i]){
            nieto[i] = fork();
            if(!nieto[i]){
                bandera = true;
                break;
            }
            break;
        }
    }

    //Padre (1)
    if(i == 2){
        for(int t = 1; t < 4; t++){
            close(tuberia[t][0]);
            close(tuberia[t][1]);
        }
        close(tuberia[0][0]);

        printf("Ingrese la cadena de texto: "); fflush(stdout);

        scanf("%s", cadena);

        printf("Padre [%d] envio el char: %s\n", getpid(), cadena);

        write(tuberia[0][1], cadena, strlen(cadena)+1);

        for(int j = 0; j < 2; j++){wait(NULL);}

    } else if(i == 0){
        //Hijo 0 (2)
        if(bandera == false){
            for(int t = 2; t < 4; t++){
                close(tuberia[t][0]);
                close(tuberia[t][1]);
            }
            close(tuberia[0][1]);
            close(tuberia[1][0]);

            read(tuberia[0][0], cadena, strlen(cadena)+1);
            printf("Hijo %d [%d] recibio el char: %s\n", i, getpid(), cadena);

            printf("Hijo %d [%d] envio el char: %s\n", i, getpid(), cadena);
            write(tuberia[1][1], cadena, strlen(cadena)+1);

            wait(NULL);
        }
        //Nieto 0 (4)
        if(bandera == true){
            for(int t = 0; t < 2; t++){
                close(tuberia[t][0]);
                close(tuberia[t][1]);
            }
            close(tuberia[2][1]);
            close(tuberia[3][0]);

            read(tuberia[2][0], cadena, strlen(cadena)+1);
            printf("Nieto %d [%d] recibio el char: %s\n", i, getpid(), cadena);

            printf("Nieto %d [%d] envio el char: %s\n", i, getpid(), cadena);
            write(tuberia[3][1], cadena, strlen(cadena)+1);
        }
    } else if(i == 1){
        //Hijo 1 (3)
        if(bandera == false){
            for(int t = 0; t < 4; t++){
                if(t == 0 || t == 3){
                    close(tuberia[t][0]);
                    close(tuberia[t][1]);
                }
            }
            close(tuberia[1][1]);
            close(tuberia[2][0]);

            read(tuberia[1][0], cadena, strlen(cadena)+1);
            printf("Hijo %d [%d] recibio el char: %s\n", i, getpid(), cadena);

            printf("Hijo %d [%d] envio el char: %s\n", i, getpid(), cadena);
            write(tuberia[2][1], cadena, strlen(cadena)+1);

            wait(NULL);
        }
        //Nieto 1 (5)
        if(bandera == true){
            for(int t = 0; t < 3; t++){
                close(tuberia[t][0]);
                close(tuberia[t][1]);
            }
            close(tuberia[3][1]);

            read(tuberia[3][0], cadena, strlen(cadena)+1);
            printf("Nieto %d [%d] recibio el char: %s\n", i, getpid(), cadena);
        }
    }

    for(int t = 0; t < 4; t++){
        close(tuberia[t][0]);
        close(tuberia[t][1]);
    }

    return 0;
}
