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

void get_arguments(char * input) {
  printf("get_arguments | [%s]\n", input);
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
 * @param {char *} input
 */
void handle_command_line(char * input) {
  printf("handle_command_line | [%s]\n", input);
  char * index_result;
  char * token = NULL;

  if ((index_result = index(input, * SEPARATOR_SEQUENTIAL)) != NULL) {
    // we found a ';' character
    token = strtok(input, SEPARATOR_SEQUENTIAL);
    token = strtok(NULL, "");
    handle_command_line(input);
  } else if ((index_result = index(input, * SEPARATOR_CONCURRENT)) != NULL) {
    // we found a '&' character
    token = strtok(input, SEPARATOR_CONCURRENT);
    token = strtok(NULL, "");
    handle_command_line(input);
  } else if ((index_result = index(input, * SEPARATOR_PIPE)) != NULL) {
    // we found a '|' character
    token = strtok(input, SEPARATOR_PIPE);
    token = strtok(NULL, "");
    handle_command_line(input);
  } else {
    // no special characters found
    get_arguments(input);
  }

  // run method again if input is still available
  if (token != NULL) {
    handle_command_line(token);
  }
}

/**
 * Driver program.
 * @return {int}
 */
int main(void) {
  char sample[] = "date -foo -bar & who & ps -ef | grep foo; ls -l -t -a; who & cat < junk; cat some.file > /tmp/foo & whoami";
  //Command * commands[100];
  printf("--------------------------------------------\n");
  handle_command_line(sample);
}