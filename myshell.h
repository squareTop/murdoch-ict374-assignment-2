/*
*
* Filename: myshell.h
* Author: Seet Ting Peng & Andrew Loone
* Date: 18 Nov 2019
*
*/


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


#define BUF_SIZE 256
#define CHANGE_DIR "cd"
#define PRINT_DIR  "pwd"
#define PROMPT     "prompt"
#define EXIT       "exit"


// Global variables
char * prompt_name = "%";
Command * command_array[MAX_COMMANDS];


/*
*
*
* Helper functions
*
*
*/
int chpr(char * input);
//void chdirect(char * input);
//int print_working_directory();
int set_redirection(Command * command);
void collect_children();
void handle_signals();
int setup_signals();
int toggle_signal_block(int how, int signal_number);
void empty_commands(Command ** commands);
void execute_commands(Command ** commands);
void create_piped_processes(Command ** piped_commands, int count);
void create_process(Command * command);


/*
*
*
* Test methods
*
*
*/

//Marking guide #2 & #3 - Test that the program can correctly process basic and built-in commands.
void test_2_3();

// Marking guide #4 - Test that the program can correctly process commands with multiple arguments.
void test_4();

// Marking guide #5 - Test that the program can correctly process commands with Wildcards.
void test_5();

// Marking guide #6 - Test that the program can run commands sequentially.
void test_6();

// Marking guide #7 - Test that the program can run processes in the background.
void test_7();

// Marking guide #8 & #9 - Test that the program can perform redirection (input or output).
void test_8_9();

// Marking guide #10 & #11 - Test that the program can execute simple and long pipelines.
void test_10_11();

// Marking guide #12 - Test that the program can run combinations of commands.
void test_12();
