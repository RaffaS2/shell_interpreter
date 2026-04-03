#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "funcoes.h"

#define MAX_INPUT 50
#define MAX_ARGS 10

int interpretador() {
    char input[MAX_INPUT]; // Guarda a linha lida do terminal.
    char *args[MAX_ARGS];

    while (1) {
        printf("%% ");
        fflush(stdout); // Garante que o %% apareça

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0'; // Remove o \n do final da linha

        if (strcmp(input, "termina") == 0) {
            break;
        }
        // Separar os argumentos
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL) {
            continue;
        }

        pid_t pid = fork();

        if (pid == 0) {
            // filho - executa o comando
            int ret = 0;

            if (strcmp(args[0], "apaga") == 0) {
                if (args[1] == NULL) {
                    fprintf(stderr, "Erro: tem de indicar o nome do ficheiro a apagar.\n");
                    exit(1);
                }
                ret = apaga_ficheiro(args[1]);

            } else if (strcmp(args[0], "lista") == 0) {
                if (args[1] == NULL) {
                    fprintf(stderr, "Erro: tem de indicar a diretoria a listar.\n");
                    exit(1);
                }
                ret = lista_diretorio(args[1]);

            } else if (strcmp(args[0], "informa") == 0) {
                if (args[1] == NULL) {
                    fprintf(stderr, "Erro: tem de indicar o ficheiro.\n");
                    exit(1);
                }
                ret = mostra_info(args[1]);

            } else if (strcmp(args[0], "conta") == 0) {
                if (args[1] == NULL) {
                    fprintf(stderr, "Erro: tem de indicar o ficheiro.\n");
                    exit(1);
                }
                ret = conta(args[1]);

            } else if (strcmp(args[0], "mostra") == 0) {
                if (args[1] == NULL) {
                    fprintf(stderr, "Erro: tem de indicar o ficheiro.\n");
                    exit(1);
                }
                ret = mostra_ficheiro(args[1]);

            } else if (strcmp(args[0], "acrescenta") == 0) {
                if (args[1] == NULL) {
                    fprintf(stderr, "Erro: tem de indicar o ficheiro origem.\n");
                    exit(1);
                }
                if (args[2] == NULL) {
                    fprintf(stderr, "Erro: tem de indicar o ficheiro de destino.\n");
                    exit(1);
                }
                ret = acrescenta(args[1], args[2]);

            } else if (strcmp(args[0], "copia") == 0) {
                if (args[1] == NULL) {
                    fprintf(stderr, "Erro: tem de indicar o ficheiro.\n");
                    exit(1);
                }
                ret = copia_ficheiro(args[1]);

            } else {
                fprintf(stderr, "Comando desconhecido: %s\n", args[0]);
                exit(127);
            }

            exit(ret); // propaga 0 (sucesso) ou 1 (erro) ao processo pai

        } else if (pid > 0) {
            // PAI: espera pelo filho
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                int code = WEXITSTATUS(status);
                if (code == 0) {
                    printf("\nTerminou comando '%s' com código %d\n", args[0], code);
                }
                // se code != 0, o filho já imprimiu a mensagem de erro
            } else {
                fprintf(stderr, "Erro na execução do comando '%s'\n", args[0]);
            }

        } else {
            perror("Erro ao criar processo");
        }
    }

    return 0;
}

int main() {
    return interpretador();
}