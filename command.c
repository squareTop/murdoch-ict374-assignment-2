#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEPARATOR_CONCURRENT "&"
#define SEPARATOR_INPUT      "<"
#define SEPARATOR_OUTPUT     ">"
#define SEPARATOR_PIPE       "|"
#define SEPARATOR_SEQUENTIAL ";"
#define MAX_COMMANDS  100
#define MAX_ARGUMENTS 1000

typedef struct CommandStruct {
  char * name;
  char * stdin;
  char * stdout;
  char * argv[MAX_ARGUMENTS];
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
 * Populates a Command struct with the found/required attributes.
 * @param {char *}    input
 * @param {int}       background
 * @param {int}       pipe
 * @param {Command *} command
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

  // calloc helps initialise all the attributes in Command
  command = calloc(1, sizeof(Command));

  if (stdout_result != NULL) {
    // output redirection found
    token = strtok(input, SEPARATOR_OUTPUT);

    // get output destination
    token = strtok(NULL, " ");

    // save the redirection
    command->stdout = strdup(token);
  } else if (stdin_result != NULL) {
    // input redirection found
    token = strtok(input, SEPARATOR_INPUT);

    // get input destination
    token = strtok(NULL, " ");

    // save the redirection
    command->stdin = strdup(token);
  }

  // save other attributes
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

/**
 * Processes a command line entry, extracts jobs and commands, and then saves
 * these commands into an array containing Command structs.
 * @param {char *}     input
 * @param {int}        background
 * @param {int}        pipe
 * @param {Command **} commands
 */
void handle_command_line(
  char * input,
  int background,
  int pipe,
  Command ** commands
) {
  printf("handle_command_line | [%s]\n", input);
  int is_background = 0;
  int is_pipe       = 0;
  char * separator  = NULL;
  char * token      = NULL;
  static int command_count;

  if ((separator = get_separator(input)) != NULL) {
    // separators found
    token = strtok(input, separator);
    token = strtok(NULL, "");

    if (strcmp(separator, SEPARATOR_CONCURRENT) == 0) {
      is_background = 1;
    }

    if (strcmp(separator, SEPARATOR_PIPE) == 0) {
      is_pipe = command_count + 1;
    }

    // process the input again to see if anymore separators are present
    handle_command_line(input, is_background, is_pipe, commands);
  } else {
    // no special characters found
    make_command(input, background, pipe, commands[command_count]);
    command_count++;
  }

  // run method again if input is still available
  if (token != NULL) {
    handle_command_line(token, background, pipe, commands);
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
  char sample[] = "ps | sort & sleep 10 & date; cat foo.txt > /tmp/foo";
  Command * commands[MAX_COMMANDS];

  printf("************************************************\n");
  handle_command_line(sample, 0, 0, commands);
}