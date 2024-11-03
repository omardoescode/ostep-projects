reverse: stack.o main.o
	gcc main.o stack.o -o reverse

stack.o: stack.c stack.h
	gcc -c stack.c -o stack.o

main.o: main.c
	gcc -c main.c -o main.o

clean:
	rm -f reverse stack.o main.o
