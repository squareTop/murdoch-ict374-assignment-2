#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEPARATOR_CONCURRENT "&"
#define SEPARATOR_INPUT      "<"
#define SEPARATOR_OUTPUT     ">"
#define SEPARATOR_PIPE       "|"
#define SEPARATOR_SEQUENTIAL ";"
#define WILDCARD_ALL         '*'
#define WILDCARD_EXACT       '?'
#define MAX_COMMANDS  100
#define MAX_ARGUMENTS 1000

static const char white_space[2] = {(char) 0x20, (char) 0x09};

typedef struct CommandStruct {
  char * name;
  char * stdin;
  char * stdout;
  char * argv[MAX_ARGUMENTS];
  int    argc;
  int    background;
  int    pipe;
  int    redirection;
} Command;

// Populates a Command struct with the found/required attributes and returns a pointer to that instance.
Command * make_command(char * input, int background, int pipe);

// Splits a string by whitespace to extract arguments (argv) and saves them into the Command instance provided.
int    get_arguments(char * input, Command * command);

// Searches for a separator (;&|) and returns the character if found.
char * get_separator(char * input);

//void   print_command(Command * command);

// Processes a command line entry, extracts jobs and commands, and then saves these commands into an array containing Command structs.
void   handle_command_line(char * input, int background, int pipe, int iteration, Command ** commands);
