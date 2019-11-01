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

/**
 * Methods
 */
void builtin_cd();
void builtin_exit();
void builtin_prompt();
void builtin_pwd();
void execute_commands();
void ignore_signals();
void test();