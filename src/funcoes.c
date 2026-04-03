#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <dirent.h>
#include "funcoes.h"

#define TAMANHO_BUFFER 1024

/**
 * @brief Apresenta todo o conteúdo que o ficheiro contém.
 *
 * @param nome_ficheiro Nome do ficheiro a obter o conteúdo.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int mostra_ficheiro(const char *nome_ficheiro) {
    int fd = open(nome_ficheiro, O_RDONLY);

    if (fd == -1) {
        dprintf(STDERR_FILENO, "Erro: Não foi possível abrir o ficheiro '%s'\n", nome_ficheiro);
        return 1;
    }

    char buffer[TAMANHO_BUFFER];
    ssize_t bytes_lidos;

    while ((bytes_lidos = read(fd, buffer, TAMANHO_BUFFER)) > 0) {
        write(STDOUT_FILENO, buffer, bytes_lidos);
    }

    if (bytes_lidos == -1) {
        dprintf(STDERR_FILENO, "Erro: Ocorreu um erro durante a leitura do ficheiro '%s'\n", nome_ficheiro);
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

/**
 * @brief Cria um novo ficheiro cópia com o conteúdo do ficheiro passado como argumento.
 *
 * @param nome_ficheiro Nome do ficheiro a copiar.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int copia_ficheiro(const char *nome_ficheiro) {
    int fd_in = open(nome_ficheiro, O_RDONLY);
    if (fd_in == -1) {
        dprintf(STDERR_FILENO, "Erro: ficheiro '%s' não existe ou não pode ser aberto\n", nome_ficheiro);
        return 1;
    }

    char nome_copia[1024];
    snprintf(nome_copia, sizeof(nome_copia), "%s.copia", nome_ficheiro);

    int fd_out = open(nome_copia, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        dprintf(STDERR_FILENO, "Erro: não foi possível criar '%s'\n", nome_copia);
        close(fd_in);
        return 1;
    }

    char buffer[TAMANHO_BUFFER];
    ssize_t bytes_lidos;

    while ((bytes_lidos = read(fd_in, buffer, TAMANHO_BUFFER)) > 0) {
        ssize_t bytes_escritos = write(fd_out, buffer, bytes_lidos);
        if (bytes_escritos != bytes_lidos) {
            dprintf(STDERR_FILENO, "Erro ao escrever em '%s'\n", nome_copia);
            close(fd_in);
            close(fd_out);
            return 1;
        }
    }

    if (bytes_lidos == -1) {
        dprintf(STDERR_FILENO, "Erro ao ler o ficheiro '%s'\n", nome_ficheiro);
        close(fd_in);
        close(fd_out);
        return 1;
    }

    close(fd_in);
    close(fd_out);
    return 0;
}

/**
 * @brief Acrescenta o conteúdo do ficheiro de origem no ficheiro de destino.
 *
 * @param origem  Ficheiro de origem do conteúdo.
 * @param destino Ficheiro de destino do conteúdo.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int acrescenta(const char *origem, const char *destino) {
    int fd_origem = open(origem, O_RDONLY);
    if (fd_origem == -1) {
        dprintf(STDERR_FILENO, "Erro: Não foi possível abrir o ficheiro de origem '%s'\n", origem);
        return 1;
    }

    int fd_destino = open(destino, O_WRONLY | O_APPEND);
    if (fd_destino == -1) {
        dprintf(STDERR_FILENO, "Erro: Não foi possível abrir o ficheiro de destino '%s'\n", destino);
        close(fd_origem);
        return 1;
    }

    char buffer[TAMANHO_BUFFER];
    ssize_t bytes_lidos;

    while ((bytes_lidos = read(fd_origem, buffer, TAMANHO_BUFFER)) > 0) {
        ssize_t bytes_escritos = write(fd_destino, buffer, bytes_lidos);
        if (bytes_escritos != bytes_lidos) {
            dprintf(STDERR_FILENO, "Erro: Falha ao escrever no ficheiro de destino '%s'\n", destino);
            close(fd_origem);
            close(fd_destino);
            return 1;
        }
    }

    if (bytes_lidos == -1) {
        dprintf(STDERR_FILENO, "Erro: Falha ao ler o ficheiro de origem '%s'\n", origem);
        close(fd_origem);
        close(fd_destino);
        return 1;
    }

    close(fd_origem);
    close(fd_destino);
    return 0;
}

/**
 * @brief Conta o número de linhas existentes num ficheiro.
 *
 * @param nome_ficheiro Nome do ficheiro a contar as linhas.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int conta(const char *nome_ficheiro) {
    int fd = open(nome_ficheiro, O_RDONLY);

    if (fd == -1) {
        dprintf(STDERR_FILENO, "Erro: Não foi possível abrir o ficheiro '%s'\n", nome_ficheiro);
        return 1;
    }

    char buffer[TAMANHO_BUFFER];
    int linhas = 1;
    ssize_t bytes_lidos;

    while ((bytes_lidos = read(fd, buffer, TAMANHO_BUFFER)) > 0) {
        for (ssize_t i = 0; i < bytes_lidos; i++) {
            if (buffer[i] == '\n') {
                linhas++;
            }
        }
    }

    if (bytes_lidos == -1) {
        dprintf(STDERR_FILENO, "Erro: Ocorreu um erro durante a leitura do ficheiro '%s'\n", nome_ficheiro);
        close(fd);
        return 1;
    }

    dprintf(STDOUT_FILENO, "Número de linhas: %d\n", linhas);
    close(fd);
    return 0;
}

/**
 * @brief Apaga um ficheiro do sistema de ficheiros.
 *
 * @param nome Nome (ou caminho) do ficheiro a remover.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int apaga_ficheiro(const char *nome) {
    if (unlink(nome) == 0) {
        printf("Ficheiro '%s' apagado com sucesso.\n", nome);
        return 0;
    } else {
        fprintf(stderr, "Erro ao apagar ficheiro '%s': %s\n", nome, strerror(errno));
        return 1;
    }
}

/**
 * @brief Mostra informações detalhadas sobre um ficheiro.
 *
 * @param ficheiro Nome (ou caminho) do ficheiro a analisar.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int mostra_info(const char *ficheiro) {
    struct stat st;

    if (stat(ficheiro, &st) == -1) {
        fprintf(stderr, "Erro ao obter informação do ficheiro '%s': %s\n", ficheiro, strerror(errno));
        return 1;
    }

    printf("Tipo de ficheiro: ");
    if (S_ISREG(st.st_mode)) {
        printf("Ficheiro normal\n");
    } else if (S_ISDIR(st.st_mode)) {
        printf("Diretoria\n");
    } else if (S_ISLNK(st.st_mode)) {
        printf("Link simbólico\n");
    } else {
        printf("Outro tipo\n");
    }

    printf("i-node: %lu\n", st.st_ino);

    struct passwd *pw = getpwuid(st.st_uid);
    if (pw != NULL) {
        printf("Dono: %s\n", pw->pw_name);
    } else {
        printf("Dono (UID): %d\n", st.st_uid);
    }

    char buffer[100];
    struct tm *tm_info;

    tm_info = localtime(&st.st_atime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Último acesso: %s\n", buffer);

    tm_info = localtime(&st.st_mtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Última modificação: %s\n", buffer);

    tm_info = localtime(&st.st_ctime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Última alteração de metadados: %s\n", buffer);

    return 0;
}

/**
 * @brief Lista o conteúdo de uma diretoria, identificando ficheiros e pastas.
 *
 * @param path Caminho da diretoria a listar.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int lista_diretorio(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char full_path[1024];

    dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "Erro ao abrir diretoria '%s': %s\n", path, strerror(errno));
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &st) == -1) {
            fprintf(stderr, "Erro ao obter informação de '%s': %s\n", full_path, strerror(errno));
            continue;
        }

        printf("%s ", entry->d_name);

        if (S_ISREG(st.st_mode)) {
            printf("(ficheiro)\n");
        } else if (S_ISDIR(st.st_mode)) {
            printf("(diretoria)\n");
        } else {
            printf("(outro)\n");
        }
    }

    closedir(dir);
    return 0;
}