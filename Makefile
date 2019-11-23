all:
	gcc -c util.c -o util.o
	gcc util.o main.c -lpthread -o microonibus
run:
	./microonibus 2