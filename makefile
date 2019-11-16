# makefile for myshell

myshell: program.h myshell.h program.c myshell.c
	gcc -Wall program.c myshell.c -o myshell
