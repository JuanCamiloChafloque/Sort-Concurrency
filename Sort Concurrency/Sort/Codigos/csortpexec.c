#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define MAXLON 70


int main(int argc, char* argv[]){
//revision de parametros
    if(argc < 3 || argc > 12){
        perror("Error. Cantidad de argumentos invalidos.");
        exit(1);
    }

    int rev = 0;
    int fd;
    //revision de parametro opcional
    if(strcmp(argv[1], "-r") == 0){
        rev = 1;
    }

    //determinar numero de procesos
    int numProcesos = argc - 2 - rev;
    pid_t proc;
    void *status;

    //declarar variables
    char input[MAXLON] ;
    char *output = argv[argc-1];
    //vectores del comando sort, normal y reverso
    char * argv1[] = { "sort" ,input ,"-s", "-nk", "4,4","-k", "5,5", "-k", "6,6", NULL };
    char * argv2[] = { "sort" ,input ,"-s", "-nrk", "4,4","-rk", "5,5", "-rk", "6,6", NULL };
    
    
    //arreglo para el comando cat
    //arreglo dinamico 
    char** argv3 = malloc(sizeof(char*)*numProcesos+2);
    //se le agregan todos los procesos
    for(int i = 0; i< numProcesos+2; i++){
        argv3[i] = malloc(MAXLON*sizeof(char));

    }
    //comando cat
    argv3[0]="cat";
    //nombre de los archivos es archivoOriginalTemp y esos son los que se van a concatenar
     for(int i = 1; i< numProcesos+1; i++){
        strcpy(argv3[i], argv[i+rev]);
        strcat(argv3[i], "Temp");
    }
    //ultimo parametro null obligatorio
    argv3[numProcesos+1]=NULL;
   

    //Se crean los procesos hijos para que ordenen los archivos usando el sort del sistema.
    //codigo basado en el siguiente link: https://stackoverflow.com/questions/33323449/how-to-use-execvp-in-c-to-sort-a-file-and-write-into-another-file
    for(int i = 0; i < numProcesos; i++){
       
        //se crea el proceso
        if((proc = fork()) == 0){
            //se define el archivo que se va a ordenar
            strcpy(input,  argv[i + 1 + rev]);
            //se define como se va a llamar el archivo resultante
            strcpy(output, input);
            strcat(output, "Temp");
            //si no llego -r
            if(rev == 0){
                //el archivo de entrada va a ser input. 
                argv1[1]= input;
                //si no existe, se va a crear
                fd = open( output, O_WRONLY | O_CREAT, 0777 );
                if( fd == -1 ) {
                    perror("Error.");
                    exit(1);
                }
                fclose(stdout);
                //la entrada estandard va a ser fd (el archivo creado)
                dup2( fd, STDOUT_FILENO);
                close(fd);
                //se ordena el archivo
                execvp(argv1[0],argv1);
                }
                //llego el -r. Lo mismo pero usando el arreglo reverso. 
            else{
                argv2[1]= input;
                fd = open( output, O_WRONLY | O_CREAT, 0777 );
                if( fd == -1 ) {
                    perror("Error.");
                    exit(1);
                }
                fclose(stdout);
                dup2( fd, STDOUT_FILENO);
                close(fd);
                execvp(argv2[0],argv2);
                
            }
            exit(0);
        }
    }

    //El padre espera a que todos los hijos terminen
    for(int i = 0; i < numProcesos+1; i++){
        wait(&status);
    }


    //copia el nuevo archivo a salida usando cat
    for (int i = 0; i < numProcesos+1; i++)
    {        
        //nombre del archivo destino
        strcpy(output, argv[argc-1]);
        //si el archivo destino no existe, se crea. 
        fd = open( output, O_WRONLY | O_CREAT, 0777 );
        if( fd == -1 ) {
            perror("Error.");
            exit(1);
        }
        fclose(stdout);
        //redireccionamiento de la salida estandard
        dup2( fd, STDOUT_FILENO);
        close(fd);
        execvp(argv3[0],argv3);

    }

    //por ultimo se ordena el archivo de salida
        
    strcpy(output, argv[argc-1]);
    strcpy(input, output);
    
    if(rev == 0){
        fd = open( output, O_WRONLY | O_CREAT, 0777 );
        if( fd == -1 ) {
            perror("Error.");
            exit(1);
        }
        fclose(stdout);
        dup2( fd, STDOUT_FILENO);
        close(fd);
        execvp(argv1[0],argv1);
    }
    //llego el -r
    else{
        fd = open( output, O_WRONLY | O_CREAT, 0777 );
        if( fd == -1 ) {
            perror("Error.");
            exit(1);
        }
        fclose(stdout);
        dup2( fd, STDOUT_FILENO);
        close(fd);
        execvp(argv2[0],argv2);
        
    }
    
    return 0;
}