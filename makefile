# makefile for shellder

shellder: command.h jobs.h shellder.h command.c jobs.c shellder.c
	gcc -Wall command.c jobs.c shellder.c -o shellder

