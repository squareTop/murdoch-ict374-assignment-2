#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "command.h"

#define BUF_SIZE 256

/**
 * Global variables
 */
Command * commands[MAX_COMMANDS];
char    * shell_name = "%";

/**
 * Methods
 */
void builtin_cd();
void builtin_exit();
void builtin_pwd();
void execute_commands();
void ignore_signals();
int prompt(char * input);
void test();