#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEPARATOR_CONCURRENT "&"
#define SEPARATOR_INPUT      "<"
#define SEPARATOR_OUTPUT     ">"
#define SEPARATOR_PIPE       "|"
#define SEPARATOR_SEQUENTIAL ";"

typedef struct CommandStruct {
  char * name;
  char * stdin;
  char * stdout;
  char * argv[1000];
  int    argc;
  int    background;
  int    pipe;
} Command;

/**
 * Helper command that prints Command attributes.
 * @param {Command *} command
 */
void print_command(Command * command) {
  printf("-----------------------------------------------\n");
  printf("| %-24s | %-16s |\n", "Name", command->name);
  printf("| %-24s | %-16d |\n", "argc", command->argc);
  for (int i = 0; i < command->argc; i++) {
    printf("| argv[%d]                  | %-16s |\n", i, command->argv[i]);
  }
  printf("| %-24s | %-16d |\n", "Runs in background?", command->background);
  printf("| %-24s | %-16d |\n", "Pipes to command",    command->pipe);
  printf("| %-24s | %-16s |\n", "Redirect input",      command->stdin);
  printf("| %-24s | %-16s |\n", "Redirect output",     command->stdout);
  printf("-----------------------------------------------\n");
}

/**
 * Splits a string by whitespace to extract arguments (argv).
 * @param  {char *}    input
 * @param  {Command *} command
 * @return {int}
 */
int get_arguments(char * input, Command * command) {
  char * token = NULL;
  int argument_count = 0;

  // split input by whitespace to derive arguments
  token = strtok(input, " ");

  while (token != NULL) {
    // duplicate string and add to arguments array
    command->argv[argument_count] = strdup(token);

    // get the next argument
    token = strtok(NULL, " ");

    // increment count
    argument_count++;
  }

  return argument_count;
}

/**
 * @param {char *} input
 */
void make_command(
  char * input,
  int background,
  int pipe,
  Command * command
) {
  //printf("make_command | [%s]\n", input);
  char * token         = NULL;
  char * stdin_result  = index(input, * SEPARATOR_INPUT);
  char * stdout_result = index(input, * SEPARATOR_OUTPUT);

  command = calloc(1, sizeof(Command));

  if (stdout_result != NULL) {
    // we found output redirection
    token = strtok(input, SEPARATOR_OUTPUT);

    // get output destination
    token = strtok(NULL, " ");
    command->stdout = strdup(token);
    //printf("make_command | stdout: [%s]\n", token);
  } else if (stdin_result != NULL) {
    // we found input redirection
    token = strtok(input, SEPARATOR_INPUT);

    // get input destination
    token = strtok(NULL, " ");
    command->stdin = strdup(token);
    //printf("make_command | stdin: [%s]\n", token);
  }

  command->argc       = get_arguments(input, command);
  command->name       = command->argv[0];
  command->background = background;
  command->pipe       = pipe;

  print_command(command);
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

void handle_command_line_2(
  char * input,
  int background,
  int pipe,
  Command ** commands
) {
  printf("handle_command_line_2 | [%s]\n", input);
  //printf("handle_command_line_2 | background: %d\n", background);
  //printf("handle_command_line_2 | pipe: %d\n", pipe);
  int is_background = 0;
  int is_pipe       = 0;
  char * separator  = NULL;
  char * token      = NULL;
  static int command_count;

  if ((separator = get_separator(input)) != NULL) {
    token = strtok(input, separator);
    token = strtok(NULL, "");

    if (strcmp(separator, SEPARATOR_CONCURRENT) == 0) {
      is_background = 1;
    }

    if (strcmp(separator, SEPARATOR_PIPE) == 0) {
      is_pipe = command_count + 1;
    }

    handle_command_line_2(input, is_background, is_pipe, commands);
  } else {
    // no special characters found
    make_command(input, background, pipe, commands[command_count]);
    command_count++;
  }

  // run method again if input is still available
  if (token != NULL) {
    //printf("handle_command_line_2 | token not null: [%s]\n", token);
    handle_command_line_2(token, background, pipe, commands);
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
    //make_command(input);
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
  //char sample[] = "date & ps -ef | grep foo; ls -l -t; cat foo.txt > /tmp/foo; cat < junk";
  //char sample[] = "cat | cat | cat | cat > junk & cat | cat | cat | cat | grep line";
  char sample[] = "ps | sort & sleep 10";
  Command * commands[100];
  printf("************************************************\n");
  //handle_command_line(sample);
  handle_command_line_2(sample, 0, 0, commands);
}