#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h> //para saber el tamaño del archivo inicial
#include <sys/wait.h>

int main(int argc, char *argv[]){
    pid_t pid;
    int tuberia[2], fichero_entrada, fichero_salida, leidos;
    if(argc != 3){
        printf("Error cantidad argumentos");
    }else{
        //primer parámetro: nombre del archivo
        char archivo[50];
        strcpy(archivo,argv[1]);
        //segundo parámetro: tamaño
        int tam = atoi(argv[2]);
        struct stat st;
        stat(archivo, &st);
        off_t tam_padre = st.st_size;

        int num_hijos = tam_padre/tam;
        for(int i = 0; i<num_hijos; i++){
            pid = fork();
            //solo lo ejecutan los hijos
            if(pid == 0){
                sleep(3);
                char c;
                close(tuberia[1]);
                
                char archivo_hijo[50];
                strcat(archivo_hijo, archivo);
                strcat(archivo_hijo, ".h0");
                char value[10];
                sprintf(value, "%c", i);
                strcat(archivo_hijo, value);

                //creamos el archivo
                fichero_salida = creat(archivo_hijo, tam);
                read(tuberia[0], &c,sizeof(char));

                while(c != '\0'){
                    write(fichero_salida, &c, sizeof(char));
                    read(tuberia[0], &c, sizeof(char));
                }
                close(fichero_salida);

            }else{
                char caracter;
                close(tuberia[0]);
                fichero_entrada = open(archivo, O_RDONLY);

                do{
                    leidos = read(fichero_entrada, &caracter, sizeof(char));
                    write(tuberia[1], &caracter, sizeof(char));
                }while(leidos != 0);
                
                close(fichero_entrada);
            }
            
        }
    }
}