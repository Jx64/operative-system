#include <stdio.h>
#include <stdlib.h>

void duplicate_HashTable(int *, int, int);

int main(int argc, char *argv[]){

  FILE *file;

  file = fopen(argv[1], "r");
  if(!file){
    printf("Archivo no encontrado\n");
    exit(EXIT_FAILURE);
  }

  int size;
  fscanf(file, "%d", &size);
 
  int *array = (int *) malloc(size * sizeof(int));
  if(!array){
    printf("MRE");
    exit(EXIT_FAILURE);
  }

  long int sum = 0;
  for(int i=0; i<size;i++){
    fscanf(file, "%d",&array[i]);
    sum += array[i];
  }

  //maximum element
  int max = 0;
  for(int i = 0; i<size; i++){
    fscanf(file, "%d",&array[i]);
    if(array[i]>max){
      max = array[i];
    }
  }

  printf("Archivo encontrado [%s] \n", argv[1]);
  printf("sum = %ld\n", sum);
  duplicate_HashTable(array, max, size);
  fclose(file);
  free(array);
  return EXIT_SUCCESS;
}

void duplicate_HashTable(int *array, int max, int size){
 
  int H[max+1];
 
  for(int i = 0; i <= max; i++){ H[i] = 0; }
 
  for(int i = 0; i < size; i++){ H[array[i]]++; }

  int repeated = 0, count=0, srepeated=0, scount=0;

  for(int i = 0; i<=max;i++){
    if(H[i]>count){
      scount=count;
      srepeated = repeated;
      count=H[i];
      repeated=i;
    }else if(H[i]>scount && H[i]<=count){
      scount = H[i];
      srepeated=i;
    }
  }

  printf("most repeated elements: (%d, %d) (%d, %d)\n", repeated, count, srepeated, scount);
}