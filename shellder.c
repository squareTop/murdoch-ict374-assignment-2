#include "shellder.h"

/**
 * Test that the program can run processes in the background, like "sleep 10 &".
 * Satisfies:
 * - Issue #8
 * - Requirement #9
 * - Marking guide #7
 */
void test_background() {
  int index = 0;
  char command[BUF_SIZE];
  char * test_commands[6] = {
    "echo hello & echo world",
    "sleep 10 & echo hello",
    "ps & ls",
    "echo ps-command & ps & echo ls-command & ls -l",
    "sleep 10 &",
    NULL
  };

  while (test_commands[index] != NULL) {
    strcpy(command, test_commands[index]);
    handle_command_line(command, 0, 0, 0, command_list);
    execute_commands(command_list);
    empty_commands(command_list);
    index++;
  }
}

/**
 * Test that the program can run a combination of commands.
 * Satisfies:
 * - Issue #14
 * - Requirement #8, 9, 10
 * - Marking guide #12
 */
void test_combination() {
  int index = 0;
  char command[BUF_SIZE];
  char * test_commands[2] = {
    "ls -lt | cat > junk ; ps | sort & echo ps-output ; sleep 10 & echo wait-for-10seconds ; cat junk | cat | grep a | sort -r",
    NULL
  };

  while (test_commands[index] != NULL) {
    strcpy(command, test_commands[index]);
    handle_command_line(command, 0, 0, 0, command_list);
    execute_commands(command_list);
    empty_commands(command_list);
    index++;
  }
}

/**
 * Test that the program can correctly process commands with multiple arguments,
 * like "% ls -l -a -h"
 * Satisfies:
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
    handle_command_line(command, 0, 0, 0, command_list);
    execute_commands(command_list);
    empty_commands(command_list);
    index++;
  }
}

/**
 * Test that the program can execute piped commands.
 * Example: "ps -ef | grep usr | head".
 * Satisfies:
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
    handle_command_line(command, 0, 0, 0, command_list);
    execute_commands(command_list);
    empty_commands(command_list);
    index++;
  }
}

/**
 * Test that the program can perform redirection (input or output).
 * Example: "cat foo > bar", "cat < foo".
 * Satisfies:
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
    handle_command_line(command, 0, 0, 0, command_list);
    execute_commands(command_list);
    empty_commands(command_list);
    index++;
  }
}

/**
 * Test that the program can run commands sequentially. (e.g. % ps; ls)
 * Satisfies:
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
    handle_command_line(command, 0, 0, 0, command_list);
    execute_commands(command_list);
    empty_commands(command_list);
    index++;
  }
}

/**
 * Changes prompt name. Does nothing if argument is an empty string.
 * Returns 0 if changed, 1 if not.
 * Satisfies:
 * - Issue #1
 * - Requirement #3
 * - Marking guide #3
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
 * Changes working directory, which will be set to the user's home directory
 * if the argument "input" is empty.
 * Satisfies:
 * - Issue #3
 * - Requirement #5
 * - Marking guide #3
 *
 * @param {char *} input
 */
void change_directory(char * input) {
  if (input != NULL) {
    wordexp_t word_expansion;
    wordexp(input, &word_expansion, 0);

    if (chdir(word_expansion.we_wordv[0]) < 0) {
      printf("cd: %s: Not a directory.\n", word_expansion.we_wordv[0]);
    }

    wordfree(&word_expansion);
  } else {
    input = getenv("HOME");
    chdir(input);
  }
}

/**
 * Prints current working directory.
 * Satisfies:
 * - Issue #2
 * - Requirement #4
 * - Marking guide #3
 *
 * @return {int}
 */
int print_working_directory() {
  char current_dir[1024];
  getcwd(current_dir, 1024);
  printf("%s\n", current_dir);
  return 0;
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
 * Recursively collect background processes (zombies).
 * Satisfies:
 * - Issue #16
 * - Marking guide #14
 */
void collect_children() {
  int collect = 1;
  int status;
  pid_t pid;

  while (collect) {
    pid = waitpid(-1, &status, WNOHANG);

    if (pid <= 0) {
      collect = 0;
    }
  }
}

/**
 * Signal handler; we're only handling SIGCHLD.
 * @param {int} signal_number
 */
void handle_signals(int signal_number) {
  if (signal_number == SIGCHLD) {
    collect_children();
  }
}

/**
 * Ignore interrupt, quit, and stop signals, catch child termination signal.
 * Returns 0 if successful with setup.
 * Should satisfy marking guide #13.
 *
 * @return {int}
 */
int setup_signals() {
  struct sigaction act;
  sigset_t signal_set;

  act.sa_flags = 0;
  act.sa_handler = handle_signals;

  // catch SIGCHLD
  if (sigaction(SIGCHLD, &act, NULL) != 0) {
    perror("sigaction");
    exit(1);
  }

  // if we're debugging, don't ignore the other signals to allow easy exit.
  if (DEBUG == 1) {
    return 0;
  }

  // add signals we want to ignore
  sigemptyset(&signal_set);
  sigaddset(&signal_set, SIGINT);
  sigaddset(&signal_set, SIGQUIT);
  sigaddset(&signal_set, SIGTSTP);

  if (sigprocmask(SIG_SETMASK, &signal_set, NULL) < 0) {
    perror("Error with SEG_SETMASK");
    return -1;
  }

  return 0;
}

/**
 * Blocks or unblocks a signal.
 * Primarily used to block the SIGCHLD signal while the shell is still
 * waiting for a process to complete, so that the signal doesn't interrupt the
 * main while loop and prematurely show the prompt.
 *
 * @param  {int} how           | Values like SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK.
 * @param  {int} signal_number | Values like SIGCHLD, SIGINT, etc.
 * @return {int}
 */
int toggle_signal_block(int how, int signal_number) {
  sigset_t signal_set;
  sigemptyset(&signal_set);
  sigaddset(&signal_set, signal_number);

  if (sigprocmask(how, &signal_set, NULL) < 0) {
    perror("Error trying to (un)block signal");
    return -1;
  } else {
    return 0;
  }
}

/**
 * Empties the command list so that it may be used for the next round of
 * command line processing.
 */
void empty_commands(Command ** commands) {
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
void execute_commands(Command ** commands) {
  int index = 0;
  int pipe_count = 0;
  Command * command;
  Command * piped_commands[MAX_COMMANDS];

  while ((command = commands[index++]) != NULL) {
    if (strcmp(command->name, BUILTIN_CHANGE_DIR) == 0) {
      change_directory(command->argv[1]);
    } else if (strcmp(command->name, BUILTIN_EXIT) == 0) {
      exit(0);
    } else if (strcmp(command->name, BUILTIN_PRINT_DIR) == 0) {
      print_working_directory();
    } else if (strcmp(command->name, BUILTIN_PROMPT) == 0) {
      prompt(command->argv[1]);
    } else {
      if (command->pipe > 0) {
        piped_commands[pipe_count] = command;
        pipe_count++;
      } else {
        if (pipe_count > 0) {
          piped_commands[pipe_count++] = command;
          create_piped_processes(piped_commands, pipe_count);

          // "empty" the array so that we may use it again later
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
  pid_t pid;
  int i, j;
  int status;
  int pids[count];

  int is_background = 0;
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
    pid = fork();

    // if any of the processes have to run in the background, ALL of them must run in the background
    if (command->background == 1) {
      is_background = 1;
    }

    if (pid == 0) {
      //printf("create_process | %s | %d\n", command->name, getpid());
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

      //
      if (command->stdin != NULL || command->stdout != NULL) {
        set_redirection(command);
      }

      // we need to close the pipes else the process "hangs"; can't write/read if the other end is open.
      for (j = 0; j < num_of_pipes; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
      }

      // print error if we're unable to execute the command
      if (execvp(command->name, command->argv) < 0) {
        fprintf(stderr, "%s: %s\n", command->name, strerror(errno));
        exit(1);
      }
    }

    // save the child process ID
    pids[i] = pid;
  }

  // we need to close the pipes else the process "hangs"; can't write/read if the other end is open.
  for (i = 0; i < num_of_pipes; i++) {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }

  /*
   * Wait for each child process created previously.
   * This approach seems more 'air-tight' than the previous "wait(&status)" one,
   * which seemed to cause problems with background processes and multiple pipes.
   * Example: "sleep 10 & ps -ef | grep usr | head | sort"
   * ... would seem to wait for "sleep 10" to complete even though it's a
   * background process. Using "waitpid" seems to fix this.
   */
  if (is_background == 0) {
    for (i = 0; i < count; i++) {
      waitpid(pids[i], &status, 0);
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
    //printf("create_process | %s | %d\n", command->name, getpid());
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

  /**
   * Parent only waits if command is meant to run in the background.
   * Satisfies:
   * - Issue #8
   * - Requirement #9
   * - Marking guide #7
   */
  if (command->background == 0) {
    waitpid(pid, &status, 0);
  }
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

  //test_sequential();
  setup_signals();

  // run infinite loop; prompt for input and execute commands
  while (1) {
    printf("%s ", shell_name);
    input_pointer = fgets(input, BUF_SIZE, stdin);

    /**
     * Force a re-read if a signal interrupts our fgets.
     * Satisfies:
     * - Issue #17
     * - Marking guide #15
     */
    while (input_pointer == NULL && errno == EINTR) {
      input_pointer = fgets(input, BUF_SIZE, stdin);
    }

    // remove newline character
    input[strlen(input) - 1] = '\0';

    // only proceed if input is not empty; allows for empty returns (hitting enter with no input)
    if (strlen(input) > 0) {
      /**
       * Block SIGCHLD so that the signal doesn't interrupt our loop if our
       * shell still waiting for a process to complete.
       */
      toggle_signal_block(SIG_BLOCK, SIGCHLD);

      handle_command_line(input, 0, 0, 0, command_list);
      execute_commands(command_list);
      empty_commands(command_list);

      // unblock SIGCHLD so that we may catch completed background processes
      toggle_signal_block(SIG_UNBLOCK, SIGCHLD);
    }
  }

  exit(0);
}
