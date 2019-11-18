/*
*
* Filename: command.c
* Authors: Seet Ting Peng and Andrew Loone
* Date: 18 Nov 2019
*
*/

#include "command.h"

// Splits a string by whitespace to extract arguments (argv) and saves them into the Command instance provided.
int get_arguments(char * input, Command * command) {
  char * token = NULL;
  int argument_count = 0;

  // split input by whitespace to derive arguments
  token = strtok(input, white_space);

  while (token != NULL) {

    if (index(token, WILDCARD_ALL) != NULL || index(token, WILDCARD_EXACT) != NULL) {
      glob_t result;

      glob(token, 0, NULL, &result);

      for (int i = 0; i < result.gl_pathc; i++) {
        command->argv[argument_count] = strdup(result.gl_pathv[i]);
        argument_count++;
      }

      globfree(&result);
    } else {
      command->argv[argument_count] = strdup(token);

      argument_count++;
    }

    // get the next argument
    token = strtok(NULL, white_space);
  }

  return argument_count;
}


// Searches for a separator and returns the character if found.
char * get_separator(char * input) {
  char * separator = NULL;

  if ((separator = index(input, * SEPARATOR_SEQUENTIAL)) != NULL) {
    separator = SEPARATOR_SEQUENTIAL;
  }
  else if ((separator = index(input, * SEPARATOR_CONCURRENT)) != NULL) {
    return SEPARATOR_CONCURRENT;
  }
  else if ((separator = index(input, * SEPARATOR_PIPE)) != NULL) {
    return SEPARATOR_PIPE;
  }

  return separator;
}


// Populates a Command struct with the found/required attributes and returns a pointer to that instance.
Command * make_command(char * input, int background, int pipe) {
  char    * token         = NULL;
  char    * stdin_result  = index(input, * SEPARATOR_INPUT);
  char    * stdout_result = index(input, * SEPARATOR_OUTPUT);
  Command * command       = calloc(1, sizeof(Command));

  if (stdout_result != NULL) {
    token = strtok(input, SEPARATOR_OUTPUT);
    token = strtok(NULL, " ");
    command->redirection = 1;
    command->stdout = strdup(token);
  }
  else if (stdin_result != NULL) {
    token = strtok(input, SEPARATOR_INPUT);
    token = strtok(NULL, " ");
    command->redirection = 0;
    command->stdin = strdup(token);
  }

  command->argc       = get_arguments(input, command);
  command->name       = command->argv[0];
  command->background = background;
  command->pipe       = pipe;

  if (command->argc > 0) {
    return command;
  } else {
    free(command);
    return NULL;
  }
}


// Processes a command line entry, extracts jobs and commands, and then saves these commands into an array containing Command structs.
void handle_command_line(char * input, int background, int pipe, int iteration, Command ** commands) {
  int is_background = 0;
  int is_pipe       = 0;
  char * separator  = NULL;
  char * token      = NULL;
  static int command_count;

  if (iteration == 0) {
    command_count = 0;
  }

  if ((separator = get_separator(input)) != NULL) {
    token = strtok(input, separator);
    token = strtok(NULL, "");

    if (strcmp(separator, SEPARATOR_CONCURRENT) == 0) {
      is_background = 1;
    }

    if (strcmp(separator, SEPARATOR_PIPE) == 0) {
      is_pipe = command_count + 1;
    }

    handle_command_line(input, is_background, is_pipe, 1, commands);
  }
  else {
    commands[command_count] = make_command(input, background, pipe);
    command_count++;
  }

  // run method again if input is still available
  if (token != NULL) {
    handle_command_line(token, background, pipe, 1, commands);
  }
}
