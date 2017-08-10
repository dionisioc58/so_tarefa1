#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[]) {
    pid_t pid_filho = 0;
    int i;
    if(argc < 2) {
        printf("Parâmetro não informado\n");
        return 0;
    }
    int qtde = atoi(argv[1]);
    int valor = 1;
    printf("Realizar a soma dos %d primeiros números.\n", qtde);

    int vetor_pipe[2], vetor_pipe_volta[2];
    char buffer[10];
    pipe(vetor_pipe);
    pipe(vetor_pipe_volta);
    int num = 1;
    i = 1;
    
    while((pid_filho == 0) & (i < qtde)) {
        //printf("Criando filho %d\n", i);
        pid_filho = fork();
        if(pid_filho == 0) {// Processo filho
            close(vetor_pipe[1]); //Fecha o caminho de escrita pois não será utilizado pelo filho
            while (read(vetor_pipe[0], &buffer, 10) > 0) // leia enquanto nao for o fim (EOF)
            close(vetor_pipe[0]); // fecha o caminho de leitura do pipe
            sscanf(buffer, "%d", &num); //Converte em inteiro
            num = num + (i+1);      //Incrementa
            valor = num;
            sprintf(buffer, "%d", num); //Converte em string
            close(vetor_pipe_volta[0]); //Fecha o caminho de leitura pois nao será utilizado
            write(vetor_pipe_volta[1],  buffer, 10); // envia o conteúdo do parametro passado pelo pipe
            close(vetor_pipe_volta[1]); // fecha o caminho de escrita do pipe
            printf("PID_PAI: %d \t| PID: %d \t| Filho devolve: %d\n", getppid(), getpid(), valor);
        } else {
            sprintf(buffer, "%d", valor);
            close(vetor_pipe[0]); //Fecha o caminho de leitura pois nao será utilizado
            write(vetor_pipe[1], buffer, strlen(buffer)); // envia o conteúdo do parametro passado pelo pipe
            close(vetor_pipe[1]); // fecha o caminho de escrita caracterizando o fim (EOF)
        }
        i++;
    }
    for(i = 0; i < qtde; i++)
        wait(NULL);
        
    return 0;
}