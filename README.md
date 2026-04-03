# Interpretador de Comandos de Ficheiros

Um interpretador de comandos em C que permite realizar operações sobre ficheiros e diretorias diretamente no terminal, utilizando processos filho (`fork`) para executar cada comando.

---

## Compilação

Na raiz do projeto, executar:

```bash
# Compilar o projeto
cd src
gcc interpretador.c funcoes.c -o interpretador

# Executar o interpretador
./interpretador
```

---

## Utilização

Após executar o interpretador, é apresentada uma prompt `%` onde podes introduzir comandos.

```
% <comando> [argumentos]
```

Para sair do interpretador:

```
% termina
```

---

## Comandos Disponíveis

### `mostra <ficheiro>`
Apresenta o conteúdo de um ficheiro no terminal.

```
% mostra teste.txt
```

---

### `copia <ficheiro>`
Cria uma cópia do ficheiro com o sufixo `.copia`, na mesma diretoria.

```
% copia teste.txt
```

---

### `acrescenta <origem> <destino>`
Acrescenta o conteúdo do ficheiro de origem no final do ficheiro de destino.

```
% acrescenta origem.txt destino.txt
```

---

### `conta <ficheiro>`
Conta e apresenta o número de linhas de um ficheiro.

```
% conta teste.txt
```

---

### `apaga <ficheiro>`
Remove o ficheiro indicado do sistema de ficheiros.

```
% apaga teste.txt
```

---

### `informa <ficheiro>`
Apresenta informações detalhadas sobre um ficheiro, incluindo:
- Tipo de ficheiro (normal, diretoria, link simbólico)
- Número do i-node
- Nome do utilizador dono
- Datas de último acesso, modificação e alteração de metadados

```
% informa teste.txt
```

---

### `lista <diretoria>`
Lista o conteúdo de uma diretoria, identificando cada entrada como ficheiro, diretoria ou outro tipo.

```
% lista .
```
