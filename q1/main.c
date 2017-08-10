#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    pid_t pid_filho = 0;
    int i;
    if(argc < 2) {
        printf("Parâmetro não informado\n");
        return 0;
    }

    char *p;
    int valor = atoi(argv[1]);
    printf("Recebido o número %d.\n", valor);
    if(valor > 1) {
        i = 1;
        while((pid_filho == 0) & (i < valor)) {
            printf("PID_PAI: %d \t| PID: %d \t| Criando filho %d\n", getppid(), getpid(), i);
            pid_filho = fork();
            wait(NULL);
            i++;
        }
    }

    return 0;
}