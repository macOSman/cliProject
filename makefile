CC=gcc

shell: src/shell.c src/minishell.h
	$(CC) -o shell src/shell.c src/minishell.h