#include "shellder.h"

/**
 * Test that the program can correct process commands with multiple arguments,
 * like "% ls -l -a -h"
 * Should satisfy:
 * - Issue #11
 * - Requirement #(N/A)
 * - Marking guide #4
 */
void test_multiple_command_args() {
  int index = 0;
  char command[BUF_SIZE];
  char * test_commands[4] = {
    "ls -l -a -h -i -e -g -m -n -p",
    "echo foo bar lorem ipsum dolor sit amet",
    NULL
  };

  while (test_commands[index] != NULL) {
    strcpy(command, test_commands[index]);
    handle_command_line(command, 0, 0, 0, commands);
    execute_commands();
    empty_commands();
    index++;
  }
}

/**
 * Test that the program can run commands sequentially. (e.g. % ps; ls)
 * Should satisfy:
 * - Issue #10
 * - Requirement #10
 * - Marking guide #6
 */
void test_sequential() {
  int index = 0;
  char command[BUF_SIZE];
  char * test_commands[4] = {
    "sleep 3 ; echo hello",
    "sleep 3 ; ls -l",
    "sleep 3 ; echo hello1 ; sleep 3 ; echo hello2",
    NULL
  };

  while (test_commands[index] != NULL) {
    strcpy(command, test_commands[index]);
    handle_command_line(command, 0, 0, 0, commands);
    execute_commands();
    empty_commands();
    index++;
  }
}

/**
 * Changes prompt name. Does nothing if argument is an empty string.
 * Returns 0 if changed, 1 if not.
 * Satisfies requirement #3 and marking guide #3.
 *
 * @param  {char *} input
 * @return {int}
 */
int prompt(char * input) {
  if (input != NULL && strlen(input) > 0) {
    shell_name = strdup(input);
    return 0;
  }

  return 1;
}

/**
 * Ignore interrupt, quit, and stop signals.
 * Satisfies marking guide #13.
 */
void ignore_signals() {
  sigset_t signal_set;
  sigemptyset(&signal_set);
  sigaddset(&signal_set, SIGINT);
  sigaddset(&signal_set, SIGQUIT);
  sigaddset(&signal_set, SIGTSTP);
  sigprocmask(SIG_SETMASK, &signal_set, NULL);
}

/**
 * Empties the command list so that it may be used for the next round of
 * command line processing.
 */
void empty_commands() {
  int index = 0;
  int argv_index = 0;

  while (commands[index] != NULL) {
    //printf("** freeing: %s, %p\n", commands[index]->name, commands[index]);

    // nullify arguments
    while (commands[index]->argv[argv_index] != NULL) {
      commands[index]->argv[argv_index] = NULL;
      argv_index++;
    }

    if (commands[index]->name != NULL) {
      free(commands[index]->name);
    }

    if (commands[index]->stdin != NULL) {
      free(commands[index]->stdin);
    }

    if (commands[index]->stdout != NULL) {
      free(commands[index]->stdout);
    }

    // free and nullify Command struct
    free(commands[index]);
    commands[index] = NULL;

    index++;
  }
}

/**
 * Executes commands found in list.
 * If any of the built-in commands are found, we'll simply run those methods.
 * Otherwise, we'll fork child processes to run the other shell commands.
 */
void execute_commands() {
  int index = 0;
  Command * command;

  while ((command = commands[index++]) != NULL) {
    if (strcmp(command->name, BUILTIN_CHANGE_DIR) == 0) {
      //
    } else if (strcmp(command->name, BUILTIN_EXIT) == 0) {
      //
    } else if (strcmp(command->name, BUILTIN_PRINT_DIR) == 0) {
      //
    } else if (strcmp(command->name, BUILTIN_PROMPT) == 0) {
      prompt(command->argv[1]);
    } else {
      create_process(command);
    }
  }

  free(command);
}

/**
 * Creates child process that runs command.
 * @param {Command *}
 */
void create_process(Command * command) {
  //printf("create_process | %d\n", getpid());
  pid_t pid;
  int status;

  pid = fork();

  if (pid == 0) {
    execvp(command->name, command->argv);

    // print error if invalid command
    fprintf(stderr, "%s: %s\n", command->name, strerror(errno));
    exit(1);
  } else if (pid < 0) {
    perror("Error forking");
    exit(1);
  }

  wait(&status);

  if (WIFEXITED(status)) {
    //printf("** Child process '%s' exited with status: %d.\n", command->name, WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    //printf("** Child process '%s' terminated by signal: %d.\n", command->name, WTERMSIG(status));
    perror("Process signal");
  }

  //printf("create_process | parent continues | %d\n", getpid());
}

/**
 * Driver program.
 * @param  {int}    argc
 * @param  {char *} argv[]
 * @return {int}
 */
int main(int argc, char * argv[]) {
  char input[BUF_SIZE];

  //test_multiple_command_args();

  // ignore keyboard signals
  //ignore_signals();

  // run infinite loop; prompt for input and execute commands
  while (1) {
    printf("%s ", shell_name);
    fgets(input, BUF_SIZE, stdin);

    // remove newline character
    input[strlen(input) - 1] = '\0';

    // only proceed if input is not empty; allows for empty returns (hitting enter with no input)
    if (strlen(input) > 0) {
      handle_command_line(input, 0, 0, 0, commands);
      execute_commands();
      empty_commands();
    }
  }

  exit(0);
}