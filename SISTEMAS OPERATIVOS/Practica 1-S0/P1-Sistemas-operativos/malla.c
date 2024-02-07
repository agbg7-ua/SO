#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>

int main (int argc, char *argv[]) 
{   
    pid_t pid;
    //Comprobamos que se introduce la cantidad de argumentos necesaria
    if(argc != 3){
        printf("Error cantidad argumentos");
    }else{
        //primer parámetro
        int x = atoi(argv[1]);
        //segundo parametro
        int y = atoi(argv[2]);
        pid = fork();
        
        //controla los sucesos que conforman las columnas
        for(int i = 1; i<y; i++){
            if(pid>0){
                pid = fork();
            }
        }

        //controla los sucesos que conforman las filas
        for(int j = 1; j<x; j++){
            if(pid==0){
                pid = fork();
            }
        }
        
    sleep(10);
    }
   exit(0);
} 