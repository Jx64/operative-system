#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig){ 
    printf("Señal enviada a: ");
}

int main(int argc, char **argcv){
    pid_t child[atoi(argcv[1])];
    int numhijos = atoi(argcv[1]), i;

    signal(SIGUSR1, handler);
    
    for(i = 0; i < numhijos ; i++){
        child[i] = fork();
        if(!child[i]) break;
    }

    if(i==numhijos){
        printf("Padre\n");
        sleep(2);
        kill(child[numhijos-1], SIGUSR1);
        for(int j = 0; j< numhijos ; j++) wait(NULL);
    }   
    else{
        pause();
        printf("Hijo %d\n", i);
        sleep(2);
        if(i>0)
            kill(child[i-1], SIGUSR1);
    }

    return EXIT_SUCCESS;
}