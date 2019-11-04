#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "command.h"

#define DEBUG 0
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
void collect_zombies();
void create_process(Command * command);
void empty_commands();
void execute_commands();
void handle_signals();
void setup_signals();
int  set_redirection(Command * command);
void create_piped_processes(Command ** piped_commands, int count);

/**
 * Methods: built-in commands
 */
int change_directory();
int exit_program();
int prompt(char * input);
int print_working_directory();

/**
 * Methods: tests
 * These tests help verify that we satisfy project requirements.
 */
void test_multiple_command_args();
void test_pipes();
void test_redirection();
void test_sequential();