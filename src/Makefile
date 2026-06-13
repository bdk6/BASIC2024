# Makefile for basic2024
# wrc 20240808

basic2024:  basic2024.o scanner.o parser.o
	gcc -o basic2024 basic2024.o scanner.o parser.o

basic2024.o:  basic2024.c
	gcc -c basic2024.c

scanner.o:  scanner.c scanner.h
	gcc -c scanner.c

parser.o:  parser.c parser.h
	gcc -c parser.c

	