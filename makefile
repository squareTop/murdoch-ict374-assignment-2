# makefile for myshell

myshell: command.h myshell.h command.c myshell.c
	gcc -Wall command.c myshell.c -o myshell
