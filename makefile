# makefile for shellder

shellder: command.h shellder.h command.c shellder.c
	gcc -Wall command.c shellder.c -o shellder

