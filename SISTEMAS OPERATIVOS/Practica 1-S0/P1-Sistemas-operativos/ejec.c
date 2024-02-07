#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>	
#include <string.h> 	
#include <unistd.h>
#include <signal.h>	
int seguir = 1;
void final(){
}	

void pstree(){
    int pid,estado;
    pid = fork();

    if(pid == 0){ 
		execlp("pstree", "pstree", "-p", NULL);
	}else{
        wait(&estado);
    }

}
int main (int argc, char *argv[]) {
    pid_t pid,arbol,pA,pB,pX,pY,pZ;
    int i,estado;

    arbol = getpid();
    printf("Soy el proceso ejec: mi pid es %d\n", arbol);
    pid=fork();
    if(pid != 0){
        wait(&estado); 
		printf("Soy ejec(%d) y muero\n", arbol); 
    }else{
        pA = getpid();
		printf("Soy el proceso A: mi pid es %d. Mi padre es %d\n", pA, arbol); 
		pid = fork(); 
        if(pid != 0){	
            signal(SIGUSR1,final);
            wait(&estado); 	
			printf("Soy A(%d) y muero\n", pA);
        }else{	  
            signal(SIGUSR1,final);
            pB=getpid();

            printf("Soy el proceso B: mi pid es %d. Mi padre es %d\n. Mi Abuelo es %d\n", pB, pA, arbol); 
            for(i= 1;i<=3;i++){
                pid = fork(); 
                if(pid != 0){
                    if(i==1) pX = pid; 
                    if(i==2) pY = pid;
                }else{
                    switch(i){
                        case 1:
                        printf("Soy el proceso X. mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n", pX, pB, pA, arbol); 					
                        signal(SIGUSR1,final);
                        pause();

                        printf("Soy X(%d) y muero\n", getpid());
                        break;
                        case 2:
                        pY = getpid();
                        printf("Soy el proceso Y. mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n", pY, pB, pA, arbol); 
						signal(SIGUSR1,final);					
                        pause();
                        printf("Soy Y(%d) y muero\n", getpid());
                        break;
                        case 3:
                        pZ = getpid();
                        printf("Soy el proceso Z. mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n", pZ, pB, pA, arbol); 
                        signal(SIGALRM,final);
                        alarm(atoi(argv[1]));
                        pause();
                        pstree();
                        final();
                      
                        kill(pX, SIGUSR1);
                        kill(pY, SIGUSR1);
                        kill(getpid(),SIGKILL);
                        printf("Soy Z(%d) y muero\n", getpid());
                        exit(0);
                        break;
                    }					  						
   
                }
            }
            printf("Soy B(%d) y muero\n", pB);  
        }
    }
}