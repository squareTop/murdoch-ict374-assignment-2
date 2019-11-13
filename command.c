#include "command.h"

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
  printf("| %-24s | %-16d |\n", "Redirection",         command->redirection);
  printf("| %-24s | %-16s |\n", "Redirect input",      command->stdin);
  printf("| %-24s | %-16s |\n", "Redirect output",     command->stdout);
  printf("-----------------------------------------------\n");
}

/**
 * Splits a string by whitespace to extract arguments (argv) and saves them
 * into the Command instance provided.
 * @param  {char *}    input
 * @param  {Command *} command
 * @return {int}
 */
int get_arguments(char * input, Command * command) {
  char * token = NULL;
  int argument_count = 0;

  // split input by whitespace to derive arguments
  token = strtok(input, white_space);

  while (token != NULL) {
    /**
     * Searches for all pathnames matching pattern.
     * Satisfies:
     * - Issue #9
     * - Requirement #6
     * - Marking guide #5
     */
    if (index(token, WILDCARD_ALL) != NULL || index(token, WILDCARD_EXACT) != NULL) {
      glob_t result;

      glob(token, 0, NULL, &result);

      // add expanded patterns to argv
      for (int i = 0; i < result.gl_pathc; i++) {
        command->argv[argument_count] = strdup(result.gl_pathv[i]);
        argument_count++;
      }

      globfree(&result);
    } else {
      // duplicate string and add to arguments array
      command->argv[argument_count] = strdup(token);

      // increment count
      argument_count++;
    }

    // get the next argument
    token = strtok(NULL, white_space);
  }

  return argument_count;
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
 * Populates a Command struct with the found/required attributes and returns
 * a pointer to that instance.
 * @param  {char *}    input
 * @param  {int}       background
 * @param  {int}       pipe
 * @return {Command *}
 */
Command * make_command(char * input, int background, int pipe) {
  char    * token         = NULL;
  char    * stdin_result  = index(input, * SEPARATOR_INPUT);
  char    * stdout_result = index(input, * SEPARATOR_OUTPUT);
  Command * command       = calloc(1, sizeof(Command));

  if (stdout_result != NULL) {
    // output redirection found
    token = strtok(input, SEPARATOR_OUTPUT);

    // get output destination
    token = strtok(NULL, " ");

    // save the redirection; 1 for standard input (fd[1])
    command->redirection = 1;
    command->stdout = strdup(token);
  } else if (stdin_result != NULL) {
    // input redirection found
    token = strtok(input, SEPARATOR_INPUT);

    // get input destination
    token = strtok(NULL, " ");

    // save the redirection; 0 for standard input (fd[0])
    command->redirection = 0;
    command->stdin = strdup(token);
  }

  // save other attributes
  command->argc       = get_arguments(input, command);
  command->name       = command->argv[0];
  command->background = background;
  command->pipe       = pipe;

  //print_command(command);

  // only return the struct if there're valid arguments
  if (command->argc > 0) {
    return command;
  } else {
    free(command);
    return NULL;
  }
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
  int iteration,
  Command ** commands
) {
  int is_background = 0;
  int is_pipe       = 0;
  char * separator  = NULL;
  char * token      = NULL;
  static int command_count;

  // reset command count if we're running this method as a fresh run
  if (iteration == 0) {
    command_count = 0;
  }

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
    handle_command_line(input, is_background, is_pipe, 1, commands);
  } else {
    // no special characters found
    commands[command_count] = make_command(input, background, pipe);
    command_count++;
  }

  // run method again if input is still available
  if (token != NULL) {
    handle_command_line(token, background, pipe, 1, commands);
  }
}