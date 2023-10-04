# operative-system

signals
-
El programa recibe una cantidad N de hijos que tendra el padre. Posterior, cuando el proceso padre termine su ejecución, enviará una señal al proceso hijo N, este a su vez cuando termine su proceso enviará una señal al hijo N-1 hasta llegar al primer hijo.

taller_fork
-
Taller en clase donde se pide un programa donde se envie el numero de procesos hijo como primer argumentos y un archivo de lectura que contenga nomeros enteros.
El programa debe imprimir la suma de todos los numeros leidos del archivo, el proceso de dividira entre el numero de hijos y cada uno hara una lectura de datos, al final se retornara una suma parcial en un archivo de salida y el proceso padre abrira ese archivo para sumar los resultados de cada hijo y devolver la suma total.

taller_1
-
El programa lee un archivo que contiene numero enteros, hace una suma total de estos e imprime el resultado; ademas el programa debe enviar los 2 numeros que mas se repiten, indicando el numero y la cantidad de veces.
El programa hace un uso de HashMap para agilizar el proceso de numeros repetidos.