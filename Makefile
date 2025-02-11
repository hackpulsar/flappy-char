sources := src/main.c src/object.c src/core.c src/screen.c
executable = flappy_char

compile:
	gcc $(sources) -o $(executable) -Wall

run:
	./$(executable)

all:
	compile
