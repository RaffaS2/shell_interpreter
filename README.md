# File Command Interpreter

A command interpreter written in C that allows you to perform operations on files and directories directly in the terminal. It utilizes child processes (fork) to execute each command independently.

---

## Compilation

From the project root, run the following:

```bash
# Compile the project
cd src
gcc interpretador.c funcoes.c -o interpretador

# Run the Interpreter
./interpretador
```

---

## Usage

After launching the interpreter, a % prompt will appear where you can enter commands.

```
% <comando> [argumentos]
```

To exit the interpreter:
```
% termina
```

---

## Available Commands

### `mostra <file>`
Displays the content of a file in the terminal.

```
% mostra teste.txt
```

---

### `copia <file>`
Creates a copy of the file with the .copia suffix in the same directory.

```
% copia teste.txt
```

---

### `acrescenta <source> <destination>`
Appends the content of the source file to the end of the destination file.

```
% acrescenta source.txt destination.txt
```

---

### `conta <file>`
Counts and displays the number of lines in a file.

```
% conta teste.txt
```

---

### `apaga <file>`
Removes the specified file from the file system.
```
% apaga teste.txt
```

---

### `informa <file>`
Displays detailed information about a file, including:
- File type (regular, directory, symbolic link)
- I-node number
- NOwner username
- Timestamps (last access, modification, and metadata change)

```
% informa teste.txt
```

---

### `lista <directory>`
Lists the contents of a directory, identifying each entry as a file, directory, or other type.
```
% lista .
```
