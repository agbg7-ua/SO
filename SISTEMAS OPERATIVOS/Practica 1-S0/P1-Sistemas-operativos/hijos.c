#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>	
#include <string.h> 	
#include <unistd.h>
#include <signal.h>	
#include <sys/shm.h> //Libreria necesaria para usar shmat y shmget
int main(int argc,char *argv[]){
    //Variables para los argumentos recibidos
    int x,y; 
    //Variables para guardar pid y para mantener un contador para bucles
    int pid,cont; 
    //Variable como segundo contador para contabilizar la cantidad de hijos generados
    int hij = 0; 
    //Punteros para usar 'shmat'
    int *punt_X,*punt_Y,sh1,sh2; 
    //Comprobamos que se introduce la cantidad de argumentos necesaria
    if(argc != 3){
        printf("Error cantidad argumentos");
    }else{
        //Guardamos en variables los argumentos proporcionados
        x = atoi(argv[1]);
        y = atoi(argv[2]);
        //Comprobamos que estos argumentos no sean ni negativos ni menores de 0
        if(x <= 0 || y <= 0){
            printf("Error en parámetros, han de ser positivos y mayor que 0");
        }else{
            // IPC_PRIVATE indica que es memoria solo accesible por hijos.
            //Guardamos el identificador de memoria compartida asociado al segmento de memoria
            sh1 = shmget(IPC_PRIVATE,sizeof(int)*(x+1),IPC_CREAT|0666);
            sh2 = shmget(IPC_PRIVATE,sizeof(int)*y,IPC_CREAT|0666);
            //Asociamos la dirección de memoria compartida al puntero de cada variable
            punt_X = (int *)shmat(sh1,0,0);
            punt_Y = (int *)shmat(sh2,0,0);
            //Guardamos el PID del primer padre
            punt_X[0] = getpid();
            //Creamos el bucle para crear los hijos
            for(cont = 1;cont<= x;cont++){
                pid = fork(); //Creamos el hijo
				if(pid != 0)
					break;	//Hacemos que el padre de ese hijo se salga del bucle para que no cree más
				else{		//Imprimimos los procesos padre indicando sus respectivos hijos			
                    printf("Soy el subhijo %d mis padres son: ", getpid());
                    				
					for(hij = 0; hij < cont; hij++){
                        printf("%d", punt_X[hij]);	
                        if(hij != cont -1){
                            printf(","); //Imprimimos coma para separar cada hijo menos al final del último
                        }
					}
					printf("\n"); //Imprimimos salto de línea

					//Vamos guardando los PID de los hijos					
					punt_X[cont] = getpid(); 			
				}		
            }
            if(cont <= x){ 
								
				wait(NULL); //Esperamos a que se hayan creado todos los hijos y hayan acabado sus tareas

				if(cont == 1){ //Imprimimos el superpadre original y sus hijos
					printf("Soy el superpadre (%d) mis hijos finales son: ", getpid());
					//Empezamos con los hijos que viene determinados por x
					for(cont = 1; cont <= x; cont++){
						printf("%d", punt_X[cont]);
						printf(", ");					
					}
					
					//Seguimos con los hijos que viene determinados por y
					for(cont = 1; cont <= y; cont++){
						printf("%d", punt_Y[cont-1]);
						if(cont < y)
							printf(", "); //Se separan por comas menos el último
					} 

					printf("\n");  //Imprimimos salto de línea
				}
            }
            else{
                
                for(cont = 1; cont <= y; cont++){
                
                    pid = fork();
                
                    if(pid == 0){ //Si el hijo no tiene más hijos guardamos el PID
                        punt_Y[cont-1] = getpid();			
                        sleep(5); 
                        break; 
                    }
                }
                                
                if(cont == y + 1){ //El padre espera al crear los procesos para dar tiempo a que se creen bien
                    for(cont = 1; cont <= y; cont++)
                        wait(NULL);
                    }
                }
        }
    }
    return 0;
}
