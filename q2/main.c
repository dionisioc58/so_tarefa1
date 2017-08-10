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
    int valor = atoi(argv[1]);
    int num;
    printf("Recebido o número %d.\n", valor);

    int vetor_pipe[2], vetor_pipe_volta[2];
    char buffer[10];

    pipe(vetor_pipe);
    pipe(vetor_pipe_volta);
    pid_filho = fork();
    if(pid_filho == 0) {// Processo filho
        close(vetor_pipe[1]); //Fecha o caminho de escrita pois não será utilizado pelo filho
        while (read(vetor_pipe[0], &buffer, 10) > 0) // leia enquanto nao for o fim (EOF)
        
        close(vetor_pipe[0]); // fecha o caminho de leitura do pipe
	    printf("FILHO: O filho leu: %s\n", buffer);

        sscanf(buffer, "%d", &num); //Converte em inteiro
        num++;                      //Incrementa
        sprintf(buffer, "%d", num); //Converte em string
        close(vetor_pipe_volta[0]); //Fecha o caminho de leitura pois nao será utilizado
        write(vetor_pipe_volta[1],  buffer, 10); // envia o conteúdo do parametro passado pelo pipe
        close(vetor_pipe_volta[1]); // fecha o caminho de escrita do pipe

        printf("FILHO: O filho escreveu: %d\n", num);
        exit(EXIT_SUCCESS); // termina o processo filho
    }else {
        char str[10];
        sprintf(str, "%d", valor+1);
        close(vetor_pipe[0]); //Fecha o caminho de leitura pois nao será utilizado
        write(vetor_pipe[1], str, strlen(str)); // envia o conteúdo do parametro passado pelo pipe
        close(vetor_pipe[1]); // fecha o caminho de escrita caracterizando o fim (EOF)
        printf("PAI: O pai enviou o valor %d para o filho\n", valor+1);

        close(vetor_pipe_volta[1]); // fecha o caminho de escrita do pipe
	    while (read(vetor_pipe_volta[0], buffer, 10) > 0) // leia enquanto nao for o fim (EOF)
        close(vetor_pipe_volta[0]); // fecha o caminho de leitura do pipe
        sscanf(buffer, "%d", &num);	
        printf("PAI: O pai leu: %d\n", num);
            
        wait(NULL); // espera até que o filho termine
        printf("PAI: O filho terminou\n");
        printf("PAI: O pai terminou\n");
    }

    return 0;
}