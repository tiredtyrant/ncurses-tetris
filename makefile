tetris: main.c 
	gcc -g -Wall -Wextra -pedantic -o tetris main.c -lncurses

clean:
	rm tetris