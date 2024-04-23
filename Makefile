sources := main.c object.c core.c screen.c
executable = flappy_char

compile:
	gcc $(sources) -o $(executable) -Wall

run:
	./$(executable)

all:
	compile
