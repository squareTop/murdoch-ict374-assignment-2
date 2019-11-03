#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "command.h"

#define BUF_SIZE 256
#define BUILTIN_CHANGE_DIR "cd"
#define BUILTIN_EXIT      "exit"
#define BUILTIN_PRINT_DIR "pwd"
#define BUILTIN_PROMPT    "prompt"

/**
 * Global variables
 */
Command * commands[MAX_COMMANDS];
char    * shell_name = "%";

/**
 * Methods
 */
void create_process(Command * command);
void empty_commands();
void execute_commands();
void ignore_signals();

/**
 * Methods: built-in commands
 */
int change_directory();
int exit_program();
int sprompt(char * input);
int print_working_directory();

/**
 * Methods: tests
 * These tests help verify that we satisfy project requirements.
 */
void test_multiple_command_args();
void test_sequential();