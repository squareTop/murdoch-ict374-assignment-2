#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEPARATOR_CONCURRENT "&"
#define SEPARATOR_INPUT      "<"
#define SEPARATOR_OUTPUT     ">"
#define SEPARATOR_PIPE       "|"
#define SEPARATOR_SEQUENTIAL ";"

typedef struct CommandStruct {
  char *  name;
  char *  stdin;
  char *  stdout;
  char ** argc;
  char ** argv;
  int     background;
  int     pipe;
} Command;

void get_arguments(char * input, int iteration) {
  printf("get_arguments | iteration: %d [%s]\n", iteration, input);
  int    argument_count = 0;
  char * arguments[100];
  char * token         = NULL;
  char * stdin_result  = index(input, * SEPARATOR_INPUT);
  char * stdout_result = index(input, * SEPARATOR_OUTPUT);

  if (stdout_result != NULL) {
    // we found output redirection
    token = strtok(input, SEPARATOR_OUTPUT);

    // get output destination
    token = strtok(NULL, " ");
    printf("get_arguments | stdout: [%s]\n", token);
  } else if (stdin_result != NULL) {
    // we found input redirection
    token = strtok(input, SEPARATOR_INPUT);

    // get input destination
    token = strtok(NULL, " ");
    printf("get_arguments | stdin: [%s]\n", token);
  }

  // split input by whitespace to derive arguments
  token = strtok(input, " ");

  while (token != NULL) {
    // duplicate string and add to arguments array
    arguments[argument_count] = strdup(token);

    printf(
      "get_arguments | arguments[%d] = [%s]\n",
      argument_count,
      arguments[argument_count]
    );

    // get the next argument
    token = strtok(NULL, " ");
    argument_count++;
  }

  printf("get_arguments | Total arguments: %d\n", argument_count);
}

/**
 * Searches for a separator (;&|) and returns the character if found.
 * @param  {char *} input
 * @return {char *}
 */
char * get_separator(char * input) {
  char * separator = NULL;

  if ((separator = index(input, * SEPARATOR_SEQUENTIAL)) != NULL) {
    // we found a ';' character
    separator = SEPARATOR_SEQUENTIAL;
  } else if ((separator = index(input, * SEPARATOR_CONCURRENT)) != NULL) {
    // we found a '&' character
    return SEPARATOR_CONCURRENT;
  } else if ((separator = index(input, * SEPARATOR_PIPE)) != NULL) {
    // we found a '|' character
    return SEPARATOR_PIPE;
  }

  return separator;
}

void handle_command_line_2(char * input, int iteration, int background, int pipe) {
  printf("handle_command_line_2 | [%s]\n", input);
  printf("handle_command_line_2 | background: %d\n", background);
  printf("handle_command_line_2 | pipe: %d\n", pipe);
  printf("handle_command_line_2 | iteration: %d\n", iteration);
  char * token = NULL;
  int is_background = 0;
  int is_pipe = 0;
  int current_iteration = iteration;

  char * separator = get_separator(input);

  if (separator != NULL) {
    token = strtok(input, separator);
    token = strtok(NULL, "");

    if (separator == SEPARATOR_CONCURRENT) {
      is_background = 1;
    }

    handle_command_line_2(input, current_iteration, is_background, is_pipe);
  } else {
    // no special characters found
    get_arguments(input, current_iteration);
    current_iteration++;
    printf("**** 1 current_iteration: %d\n", current_iteration);
  }
    //printf("**** 2 current_iteration: %d\n", current_iteration);

  // run method again if input is still available
  if (token != NULL) {
    printf("handle_command_line_2 | token not null: [%s]\n", token);
    printf("**** 3 current_iteration: %d\n", current_iteration);
    handle_command_line_2(token, current_iteration, is_background, is_pipe);
  }
}

/**
 * @param {char *} input
 */
void handle_command_line(char * input) {
  printf("handle_command_line | [%s]\n", input);
  char * token = NULL;

  char * separator = get_separator(input);

  if (separator != NULL) {
    token = strtok(input, separator);
    token = strtok(NULL, "");
    handle_command_line(input);
  } else {
    // no special characters found
    //get_arguments(input);
  }

  // run method again if input is still available
  if (token != NULL) {
    printf("handle_command_line | token not null: [%s]\n", token);
    handle_command_line(token);
  }
}

/**
 * Driver program.
 * @return {int}
 */
int main(void) {
  //char sample[] = "date -foo -bar & who & ps -ef | grep foo; ls -l -t -a; who & cat < junk; cat some.file > /tmp/foo & whoami";
  char sample[] = "date & who & ls -lt";
  //Command * commands[100];
  printf("--------------------------------------------\n");
  //handle_command_line(sample);
  handle_command_line_2(sample, 0, 0, 0);
}