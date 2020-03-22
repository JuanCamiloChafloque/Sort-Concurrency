#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define MAXLON 70
#define MAX 9
#define MAXARC 100000

struct Dato{
    char usuario[MAXLON];
    char comando[MAXLON];
    char numProc[MAXLON];
    int tiempoEjec;
    char anio[MAXLON];
    char hora[MAXLON];
};

struct Archivo{
    char nombre[MAXLON];
    struct Dato *procesos;
    int numProcesos;
};

//Lee un archivo que llega por parámetro y lo guarda en una estructura de tipo Archivo.
void leerArchivo(struct Archivo *arc, char *nombreArc){

    struct Dato datos;
    int cont;
    FILE *stream;
    char cadena[MAXLON];

    cont = 0;
    stream = fopen(nombreArc, "r");
    strcpy(arc->nombre, nombreArc);

    if(stream == NULL){
        perror("Error. No se pudo arbir el archivo.");
        exit(1);
    }

    while( !feof( stream ) ){

        fscanf(stream, "%s", cadena);
        if(cont == 0){
            strcpy(datos.usuario, cadena);
            cont++;  
        }
        else if(cont == 1){
            strcpy(datos.comando, cadena);
            cont++;
        }
        else if(cont == 2){
            strcpy(datos.numProc, cadena);
            cont++;
        }
        else if(cont == 3){
            datos.tiempoEjec = atoi(cadena);
            cont++;
        }
        else if(cont == 4){
            strcpy(datos.anio, cadena);
            cont++;
        }
        else if(cont == 5){
            strcpy(datos.hora, cadena);
            cont = 0;
        }

        if(cont == 0){
            *(arc->procesos + arc->numProcesos) = datos;
            arc->numProcesos++;
        }
    }
    fclose(stream);
}

//Escribe un archivo con nombre que llega por parámetro lo procesos que se encuentran en una estructura 
//de tipo archivo. 
void escribirArchivo(struct Archivo *arc, char *salida, int flag){

    char cadena[MAXLON];
    FILE *stream;

    strcpy(cadena, salida);
    strcat(cadena, "Temp.txt");

    if(flag == 1){
        stream = fopen(salida, "w");
    }else{
        stream =  fopen(cadena, "w");
    }

    if(stream == NULL){
        perror("Error. No se pudo abrir el archivo");
        exit(1);
    }

    for(int i = 0; i < arc->numProcesos; i++){
        fprintf(stream, "%s %s %s %d %s %s \n", arc->procesos[i].usuario, arc->procesos[i].comando,
                                                arc->procesos[i].numProc, arc->procesos[i].tiempoEjec,
                                                arc->procesos[i].anio, arc->procesos[i].hora);
    }
    fclose(stream);
}

//Método para intercambiar dos indices del arreglo
void swap(struct Archivo *arc, int i, int j){

    struct Dato aux;
    aux = arc->procesos[i];
    arc->procesos[i] = arc->procesos[j];
    arc->procesos[j] = aux;
}

//Método auxiliar del algoritmo quicksort para hallar la partición del arreglo
int partition(struct Archivo *arc, int low, int high){
    struct Dato pivot = arc->procesos[high];
    int i = low - 1;

    for(int j = low; j <= high - 1; j++){

        if(arc->procesos[j].tiempoEjec < pivot.tiempoEjec){
            i++;
            swap(arc, i, j);
        }
        else if(arc->procesos[j].tiempoEjec == pivot.tiempoEjec){
        
            if(strcmp(arc->procesos[j].anio, pivot.anio) < 0){
                i++;
                swap(arc, i, j);
            }
            else if(strcmp(arc->procesos[j].anio, pivot.anio) == 0){

                if(strcmp(arc->procesos[j].hora, pivot.hora) < 0){
                    i++;
                    swap(arc, i, j);
                }
            }
        }
    }
    swap(arc, i + 1, high);
    return (i + 1);
}

//Método auxiliar del algoritmo quicksort para hallar la partición del arreglo
int partitionReversed(struct Archivo *arc, int low, int high){
    struct Dato pivot = arc->procesos[high];
    int i = low - 1;

    for(int j = low; j <= high - 1; j++){

        if(arc->procesos[j].tiempoEjec > pivot.tiempoEjec){
            i++;
            swap(arc, i, j);
        }
        else if(arc->procesos[j].tiempoEjec == pivot.tiempoEjec){
        
            if(strcmp(arc->procesos[j].anio, pivot.anio) > 0){
                i++;
                swap(arc, i, j);
            }
            else if(strcmp(arc->procesos[j].anio, pivot.anio) == 0){

                if(strcmp(arc->procesos[j].hora, pivot.hora) > 0){
                    i++;
                    swap(arc, i, j);
                }
            }
        }
    }
    swap(arc, i + 1, high);
    return (i + 1);
}

//Ordena los procesos de un archivo utilizando el método Quicksort. Se utilizó una base del codigo 
//encontrado en: https://www.geeksforgeeks.org/quick-sort/ sin embargo se realizaron modificaciones
//al código para poder ordenar la estructura como se debía
void quicksort(struct Archivo *arc, int low, int high){

    if(low < high){
        int part = partition(arc, low, high);
        quicksort(arc, low, part - 1);
        quicksort(arc, part + 1, high);
    }
}

//Ordena los procesos de un archivo utilizando el método Quicksort. Se utilizó una base del codigo 
//encontrado en: https://www.geeksforgeeks.org/quick-sort/ sin embargo se realizaron modificaciones
//al código para poder ordenar la estructura como se debía
void quicksortReversed(struct Archivo *arc, int low, int high){

    if(low < high){
        int part = partitionReversed(arc, low, high);
        quicksortReversed(arc, low, part - 1);
        quicksortReversed(arc, part + 1, high);
    }
}

//Función auxiliar del método MergeSort
void merge(struct Archivo *arc, int l, int m, int r){

    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    struct Dato *aux1 = (struct Dato *) calloc(n1, sizeof(struct Dato));
    struct Dato *aux2 = (struct Dato *) calloc(n2, sizeof(struct Dato));

    for(i = 0; i < n1; i++){
        aux1[i] = arc->procesos[l + i];
    }
    for(j = 0; j < n2; j++){
        aux2[j] = arc->procesos[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;

    while(i < n1 && j < n2){

        if(aux1[i].tiempoEjec < aux2[j].tiempoEjec){
            arc->procesos[k] = aux1[i];
            i++; 
        }
        else if(aux1[i].tiempoEjec == aux2[j].tiempoEjec){
        
            if(strcmp(aux1[i].anio, aux2[j].anio) < 0){
                arc->procesos[k] = aux1[i];
                i++; 
            }
            else if(strcmp(aux1[i].anio, aux2[j].anio) == 0){

                if(strcmp(aux1[i].hora, aux2[j].hora) < 0){
                    arc->procesos[k] = aux1[i];
                    i++; 
                }
                else{
                    arc->procesos[k] = aux2[j];
                    j++;
                }
            }
            else{
                arc->procesos[k] = aux2[j];
                j++;
            }
        }
        else{
            arc->procesos[k] = aux2[j];
            j++;
        }
        k++;
    }

    while(i < n1){
        arc->procesos[k] = aux1[i];
        i++;
        k++;
    }

    while(j < n2){
        arc->procesos[k] = aux2[j];
        j++;
        k++;
    }
}

//Función auxiliar del método MergeSort
void mergeReversed(struct Archivo *arc, int l, int m, int r){

    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    struct Dato *aux1 = (struct Dato *) calloc(n1, sizeof(struct Dato));
    struct Dato *aux2 = (struct Dato *) calloc(n2, sizeof(struct Dato));

    for(i = 0; i < n1; i++){
        aux1[i] = arc->procesos[l + i];
    }
    for(j = 0; j < n2; j++){
        aux2[j] = arc->procesos[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;

    while(i < n1 && j < n2){

        if(aux1[i].tiempoEjec > aux2[j].tiempoEjec){
            arc->procesos[k] = aux1[i];
            i++; 
        }
        else if(aux1[i].tiempoEjec == aux2[j].tiempoEjec){
        
            if(strcmp(aux1[i].anio, aux2[j].anio) > 0){
                arc->procesos[k] = aux1[i];
                i++; 
            }
            else if(strcmp(aux1[i].anio, aux2[j].anio) == 0){

                if(strcmp(aux1[i].hora, aux2[j].hora) > 0){
                    arc->procesos[k] = aux1[i];
                    i++; 
                }
                else{
                    arc->procesos[k] = aux2[j];
                    j++;
                }
            }
            else{
                arc->procesos[k] = aux2[j];
                j++;
            }
        }
        else{
            arc->procesos[k] = aux2[j];
            j++;
        }
        k++;
    }

    while(i < n1){
        arc->procesos[k] = aux1[i];
        i++;
        k++;
    }

    while(j < n2){
        arc->procesos[k] = aux2[j];
        j++;
        k++;
    }
}

//Ordena los procesos de un archivo utilizando el método MergeSort. Se utilizó una base del codigo 
//encontrado en: https://www.geeksforgeeks.org/merge-sort/ sin embargo se realizaron modificaciones
//al código para poder ordenar la estructura como se debía
void mergesort(struct Archivo *arc, int l, int r){

    if(l < r){
        int m = l + (r - l) / 2;
        mergesort(arc, l, m); 
        mergesort(arc, m + 1, r);
        merge(arc, l, m, r);
    }
}

//Ordena los procesos de un archivo utilizando el método MergeSort. Se utilizó una base del codigo 
//encontrado en: https://www.geeksforgeeks.org/merge-sort/ sin embargo se realizaron modificaciones
//al código para poder ordenar la estructura como se debía
void mergesortReversed(struct Archivo *arc, int l, int r){

    if(l < r){
        int m = l + (r - l) / 2;
        mergesortReversed(arc, l, m); 
        mergesortReversed(arc, m + 1, r);
        mergeReversed(arc, l, m, r);
    }
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------MAIN--------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]){

    if(argc < 3){
        perror("Error. Cantidad de argumentos invalidos.");
        exit(1);
    }

    int rev = 0;

    if(strcmp(argv[1], "-r") == 0){
        rev = 1;
    }

    int numProcesos = argc - 2 - rev;
    pid_t proc;
    void *status;
    struct Archivo archivosHijos[MAX];
    struct Archivo archivoFinal;

    //Se asigna memoria y se inicializan las estructras de tipo Archivo
    for(int i = 0; i < numProcesos; i++){
        archivosHijos[i].procesos = (struct Dato *) calloc(MAXARC, sizeof(struct Dato));
        archivosHijos[i].numProcesos = 0;
    }
    archivoFinal.procesos = (struct Dato *) calloc(MAXARC, sizeof(struct Dato));
    archivoFinal.numProcesos = 0;
    
    //Se leen los archivos de entrada para llenar las estructuras con los datos de los procesos
    for (int i = 0; i < numProcesos; i++){
        leerArchivo(&archivosHijos[i], argv[i + 1 + rev]);
    }

    //Se crean los procesos hijos para que ordenen los datos que contienen las estructuras.
    for(int i = 0; i < numProcesos; i++){
        if((proc = fork()) == 0){
            if(rev == 0){
                quicksort(&archivosHijos[i], 0, archivosHijos[i].numProcesos - 1);
                escribirArchivo(&archivosHijos[i], archivosHijos[i].nombre, 0);
            }else{
                quicksortReversed(&archivosHijos[i], 0, archivosHijos[i].numProcesos - 1);
                escribirArchivo(&archivosHijos[i], archivosHijos[i].nombre, 0);
            }
            exit(0);
        }
    }

    //El padre espera a que todos los hijos terminen
    for(int i = 0; i < numProcesos; i++){
        wait(&status);
    }

    //El padre lee los archivos temporales y llena la estructura Archivofinal para ordenarla con el 
    //método de mergesort y finalmente generar el archivo de salida con todos los procesos ordenados
    for(int i = 0; i < numProcesos; i++){
        char actual[MAXLON];
        strcpy(actual, argv[i + 1 + rev]);
        strcat(actual, "Temp.txt");
        leerArchivo(&archivoFinal, actual);
    }
    if(rev == 0)
        mergesort(&archivoFinal, 0, archivoFinal.numProcesos - 1);
    else
        mergesortReversed(&archivoFinal, 0, archivoFinal.numProcesos - 1);
    escribirArchivo(&archivoFinal, argv[argc - 1], 1);

    //Se libera la memoria pedida
    for(int i = 0; i < numProcesos; i++){
        free(archivosHijos[i].procesos);
    }
    free(archivoFinal.procesos);

    return 0;
}