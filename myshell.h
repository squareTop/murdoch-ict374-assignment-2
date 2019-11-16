#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wordexp.h>
#include "program.h"

#define DEBUG 0
#define BUF_SIZE 256
#define BUILTIN_CHANGE_DIR "cd"
#define BUILTIN_EXIT       "exit"
#define BUILTIN_PRINT_DIR  "pwd"
#define BUILTIN_PROMPT     "prompt"

/**
 * Global variables
 */
char * shell_name = "%";
Command * command_list[MAX_COMMANDS];

/**
 * Methods
 */
void collect_children();
void create_piped_processes(Command ** piped_commands, int count);
void create_process(Command * command);
void empty_commands(Command ** commands);
void execute_commands(Command ** commands);
void handle_signals();

int set_redirection(Command * command);
int setup_signals();
int toggle_signal_block(int how, int signal_number);

/**
 * Methods: built-in commands
 */
void change_directory(char * input);
int  print_working_directory();
int  prompt(char * input);

/**
 * Methods: tests
 * These tests help verify that we satisfy project requirements.
 */
void test_2_3();
void test_4();
void test_5();
void test_6();
void test_7();
void test_8_9();
void test_10_11();
void test_12();
