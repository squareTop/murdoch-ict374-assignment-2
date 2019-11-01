# makefile for shellder

shellder: command.c shellder.c
	gcc -Wall command.c shellder.c -o shellder

