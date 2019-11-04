#include "shellder.h"

/**
 * Test that the program can correctly process commands with multiple arguments,
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
 * Test that the program can execute piped commands.
 * Example: "ps -ef | grep usr | head".
 * Should satisfy:
 * - Issue #7, #13
 * - Requirement #8
 * - Marking guide #10, #11
 */
void test_pipes() {
  int index = 0;
  char command[BUF_SIZE];
  char * test_commands[5] = {
    "ps -ef | head",
    "ps -ef | grep usr | head",
    "ps -ef | grep usr | head -6 | tail -3",
    "ps -ef | grep usr | head -4; date",
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
 * Test that the program can perform redirection (input or output).
 * Example: "cat foo > bar", "cat < foo".
 * Should satisfy:
 * - Issue #6
 * - Requirement #7
 * - Marking guide #8, 9
 */
void test_redirection() {
  int index = 0;
  char command[BUF_SIZE];
  char * test_commands[5] = {
    "cat foo > bar",
    "cat foo2 > bar",
    "cat < bar",
    "grep 99 < bar",
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
 * Opens a file for standard input or output.
 * Returns 0 if successfully done, 1 if not.
 * @param  {Command *} command
 * @return {int}
 */
int set_redirection(Command * command) {
  char * file_name = NULL;
  int descriptor   = command->redirection;
  int file         = 0;

  // 0 = standard input (e.g. keyboard), 1 = standard output (e.g. display)
  if (descriptor == 0) {
    file_name = command->stdin;
    file      = open(file_name, O_RDONLY);
  } else if (descriptor == 1) {
    file_name = command->stdout;
    file      = open(file_name, O_WRONLY | O_CREAT, 0766);
  } else {
    return 1;
  }

  if (file < 0) {
    perror("Error opening file");
    exit(1);
  } else {
    dup2(file, descriptor);
    close(file);
  }

  if (file_name != NULL) {
    free(file_name);
  }

  return 0;
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
  int pipe_count = 0;
  Command * command;
  Command * piped_commands[MAX_COMMANDS];

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
      if (command->pipe > 0) {
        //create_process(command);
        piped_commands[pipe_count] = command;
        pipe_count++;
      } else {
        //printf("execute_commands | %d\n", pipe_count);
        if (pipe_count > 0) {
          piped_commands[pipe_count++] = command;
          create_piped_processes(piped_commands, pipe_count);
          for (int i = 0; i < pipe_count; i++) {
            piped_commands[i] = NULL;
          }
          pipe_count = 0;
        } else {
          create_process(command);
        }
      }
    }
  }

  free(command);
}

/**
 * Creates child processes and pipes between them.
 * @param {Command **} piped_commands
 * @param {int}        count
 */
void create_piped_processes(Command ** piped_commands, int count) {
  int i;
  int status;
  int num_of_pipes = count - 1;
  int pipes[num_of_pipes][2];
  Command * command = NULL;

  // create pipes
  for (i = 0; i < num_of_pipes; i++) {
    if (pipe(pipes[i]) < 0) {
      perror("Error in pipe call");
      exit(1);
    }
  }

  // create child processes and tie their stdin and stdout to the appropriate ends of the pipes
  for (i = 0; i < count; i++) {
    command = piped_commands[i];

    if (fork() == 0) {
      /**
       * Connect appropriate descriptors to relevant pipe ends.
       *
       * Visualisation:
       * [Process 1]───────────────┐ (writes into pipe[0][1]])
       * |------------((R) Pipe 1 (W))
       * [Process 2]────┴──────────┐ (writes into pipe[1][1], reads from pipe[0][0])
       * |------------((R) Pipe 2 (W))
       * [Process 3]────┘            (reads from pipe[1][0])
       */
      if (i == 0) {
        dup2(pipes[i][1], 1);     // write into [1] of first pipe
      } else if (i == num_of_pipes) {
        dup2(pipes[i - 1][0], 0); // read from [0] of previous pipe
      } else {
        dup2(pipes[i - 1][0], 0); // read from [0] of previous pipe
        dup2(pipes[i][1], 1);     // write into [1] of next pipe
      }

      // we need to close the pipes else the process "hangs"; can't write/read if the other end is open.
      for (int j = 0; j < num_of_pipes; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
      }

      // print error if we're unable to execute the command
      if (execvp(command->name, command->argv) < 0) {
        fprintf(stderr, "%s: %s\n", command->name, strerror(errno));
        exit(1);
      }
    }
  }

  // we need to close the pipes else the process "hangs"; can't write/read if the other end is open.
  for (i = 0; i < num_of_pipes; i++) {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }

  // wait for processes to terminate if they're not supposed to run in the background
  for (i = 0; i < count; i++) {
    wait(&status);

    if (WIFEXITED(status)) {
      //printf("** Child process '%d' exited with status: %d.\n", wpid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
      //printf("** Child process '%d' terminated by signal: %d.\n", wpid, WTERMSIG(status));
    }
  }
}

/**
 * Creates child process that runs command.
 * @param {Command *}
 */
void create_process(Command * command) {
  //printf("create_process | %s | %d\n", command->name, getpid());
  pid_t pid;
  int status;

  pid = fork();

  if (pid == 0) {
    if (command->stdin != NULL || command->stdout != NULL) {
      set_redirection(command);
    }

    // print error if we're unable to execute the command
    if (execvp(command->name, command->argv) < 0) {
      fprintf(stderr, "%s: %s\n", command->name, strerror(errno));
      exit(1);
    }
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
  char * input_pointer = NULL;

  //test_pipes();

  // ignore keyboard signals
  //ignore_signals();

  // run infinite loop; prompt for input and execute commands
  while (1) {
    printf("%s ", shell_name);
    fgets(input, BUF_SIZE, stdin);

    /**
     * Force a re-read if a signal interrupts our fgets.
     * Satisfies:
     * - Issue #17
     * - Marking guide #15
     */
    if (input_pointer == NULL && errno == EINTR) {
      input_pointer = fgets(input, BUF_SIZE, stdin);
    }

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