/*
*
* Filename: myshell.c
* Author: Seet Ting Peng & Andrew Loone
* Date: 18 Nov 2019
*
*/


#include "myshell.h"


/*
*
*
* Test methods
*
*
*/


//Requirement 01, 02 & 03 - Test that the program can correctly process basic and built-in commands and reconfigurable shell prompt.
void test_1_2_3() {
  int index = 0;
  char command[BUF_SIZE];
  char * test_commands[11] = {
    "ls",
    "ps",
    "prompt myshell",
    "cd tmp",
    "pwd",
    "cd",
    "pwd",
    "cd ..",
    "pwd",
    "echo a bb ccc dddd 1 22 333 4444 test for marking guide #2 and #3",
    NULL
  };

  while (test_commands[index] != NULL) {
    strcpy(command, test_commands[index]);
    handle_command_line(command, 0, 0, 0, command_array);
    execute_commands(command_array);
    empty_commands(command_array);
    index++;
  }
}



// Requirement 04 - Test that the program can correctly process commands with Wildcards.
void test_4() {
  int index = 0;
  char command[BUF_SIZE];
  char * test_commands[6] = {
    "ls -l tmp/*.c",
    "ls -l tmp/a*c",
    "ls -l tmp/abc.?",
    "ls -l tmp/abc*.?",
    "ls -l dev/*",
    NULL
  };

  while (test_commands[index] != NULL) {
    strcpy(command, test_commands[index]);
    handle_command_line(command, 0, 0, 0, command_array);
    execute_commands(command_array);
    empty_commands(command_array);
    index++;
  }
}


// Requirement 05 - Test that the program can perform redirection (input or output).
void test_5() {
  int index = 0;
  char command[BUF_SIZE];
  char * test_commands[6] = {
    "cat < tmp/foo",
    "grep line < tmp/foo",
    "ls -l > tmp/junk",
    "cat tmp/foo > tmp/junk2",
    "cat tmp/junk2",
    NULL
  };

  while (test_commands[index] != NULL) {
    strcpy(command, test_commands[index]);
    handle_command_line(command, 0, 0, 0, command_array);
    execute_commands(command_array);
    empty_commands(command_array);
    index++;
  }
}


// Requirement 06 - Test that the program can execute simple and long pipelines.
void test_6() {
  int index = 0;
  char command[BUF_SIZE];
  char * test_commands[10] = {
    "ps -ef | head",
    "ps -ef | grep usr | head",
    "ps -ef | grep usr | head -6 | tail -3",
    "ps -ef | grep usr | head -4; date",
    "cat tmp/foo | cat",
    "cat tmp/foo | grep line",
    "cat tmp/foo | sort",
    "cat tmp/foo | sort -r",
    "cat tmp/foo | sort | sort -r | grep line",
    NULL
  };

  while (test_commands[index] != NULL) {
    strcpy(command, test_commands[index]);
    handle_command_line(command, 0, 0, 0, command_array);
    execute_commands(command_array);
    empty_commands(command_array);
    index++;
  }
}

// Requirement 07 - Test that the program can run processes in the background.
void test_7() {
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
    handle_command_line(command, 0, 0, 0, command_array);
    execute_commands(command_array);
    empty_commands(command_array);
    index++;
  }
}

// Requirement 08 - Test that the program can run commands sequentially.
void test_8() {
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
    handle_command_line(command, 0, 0, 0, command_array);
    execute_commands(command_array);
    empty_commands(command_array);
    index++;
  }
}

// Requirement 09 - Ensuring shell inherits environment from parent 
void test_9(char *envp[])
{
    for (int i = 0; envp[i] !=NULL; i++)
	printf("\n%s", envp[i]);
}


// Test that the program can run combinations of commands.
void test_10() {
  int index = 0;
  char command[BUF_SIZE];
  char * test_commands[3] = {
    "ls -l > junk ; cat < junk ; /bin/ls -lt /dev/tty* | grep tty | sort | head > junk2 & sleep 10 ; cat < junk2",
    "ls -lt | cat > junk ; ps | sort & echo ps-output ; sleep 10 & echo wait-for-10seconds ; cat junk | cat | grep a | sort -r",
    NULL
  };

  while (test_commands[index] != NULL) {
    strcpy(command, test_commands[index]);
    handle_command_line(command, 0, 0, 0, command_array);
    execute_commands(command_array);
    empty_commands(command_array);
    index++;
  }
}



// Test that the program can correctly process commands with multiple arguments.
void test_11() {
  int index = 0;
  char command[BUF_SIZE];
  char * test_commands[3] = {
    "ls -l -a -h -i -g -m -n -p",
    "echo a bb ccc dddd 1 22 333 4444 this is test for marking guide #4",
    NULL
  };

  while (test_commands[index] != NULL) {
    strcpy(command, test_commands[index]);
    handle_command_line(command, 0, 0, 0, command_array);
    execute_commands(command_array);
    empty_commands(command_array);
    index++;
  }
}



/*
*
*
* Helper functions
*
*
*/


// Opens a file for standard input or output.
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


// Recursively collect zombies.
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


// Handle SIGCHLD.
void handle_signals(int signal_number) {
  if (signal_number == SIGCHLD) {
    collect_children();
  }
}


// Ignore interrupt, quit, and stop signals, catch child termination signal.
int setup_signals() {
  struct sigaction act;
  sigset_t signal_set;

  act.sa_flags = 0;
  act.sa_handler = handle_signals;


  if (sigaction(SIGCHLD, &act, NULL) != 0) {
    perror("sigaction");
    exit(1);
  }


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


// Blocks or unblocks a signal.
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


// Empties the command list so that it may be used for the next round of command line processing.
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


// Create child processes to run the other shell commands found in list.
void execute_commands(Command ** commands) {
  int index = 0;
  int pipe_count = 0;
  Command * command;
  Command * piped_commands[MAX_COMMANDS];

  while ((command = commands[index++]) != NULL) {
    if (strcmp(command->name, CHANGE_DIR) == 0) {
      input = command->argv[1];
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
    else if (strcmp(command->name, EXIT) == 0) {
        exit(0);
    }
    else if (strcmp(command->name, PRINT_DIR) == 0) {
        char current_dir[1024];
        getcwd(current_dir, 1024);
        printf("%s\n", current_dir);
    }
    else if (strcmp(command->name, PROMPT) == 0) {
        input = command->argv[1];
        if (input != NULL && strlen(input) > 0) {
          prompt_name = strdup(input);
        }
    }
    else {
      if (command->pipe > 0) {
        piped_commands[pipe_count] = command;
        pipe_count++;
      }
      else {
        if (pipe_count > 0) {
          piped_commands[pipe_count++] = command;
          create_piped_processes(piped_commands, pipe_count);

          // "empty" the array so that we may use it again later
          for (int i = 0; i < pipe_count; i++) {
            piped_commands[i] = NULL;
          }

          pipe_count = 0;
        }
        else {
          create_process(command);
        }
      }
    }
  }

  free(command);
}


// Creates child processes.
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

      if (i == 0) {
        dup2(pipes[i][1], 1);     // write into [1] of first pipe
      } else if (i == num_of_pipes) {
        dup2(pipes[i - 1][0], 0); // read from [0] of previous pipe
      } else {
        dup2(pipes[i - 1][0], 0); // read from [0] of previous pipe
        dup2(pipes[i][1], 1);     // write into [1] of next pipe
      }

      if (command->stdin != NULL || command->stdout != NULL) {
        set_redirection(command);
      }

      for (j = 0; j < num_of_pipes; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
      }

      if (execvp(command->name, command->argv) < 0) {
        fprintf(stderr, "%s: %s\n", command->name, strerror(errno));
        exit(1);
      }
    }

    pids[i] = pid;
  }

  // we need to close the pipes else the process "hangs"; can't write/read if the other end is open.
  for (i = 0; i < num_of_pipes; i++) {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }

  /*
   * Wait for each child process created previously.
   */
  if (is_background == 0) {
    for (i = 0; i < count; i++) {
      waitpid(pids[i], &status, 0);
    }
  }
}


// Creates child process that runs command.
void create_process(Command * command) {
  pid_t pid;
  int status;

  pid = fork();

  if (pid == 0) {
    if (command->stdin != NULL || command->stdout != NULL) {
      set_redirection(command);
    }

    if (execvp(command->name, command->argv) < 0) {
      fprintf(stderr, "%s: %s\n", command->name, strerror(errno));
      exit(1);
    }
  } else if (pid < 0) {
    perror("Error forking");
    exit(1);
  }

  /**
   * Parent waits if command is running in the background.
   */
  if (command->background == 0) {
    waitpid(pid, &status, 0);
  }
}


// Main program
int main(int argc, char * argv[], char * envp[]) {
  char input[BUF_SIZE];
  char * input_pointer = NULL;

  // Tests;
//  printf("Running tests 1, 2 and 3..\n");
  test_1_2_3();
//  printf("Running test 4..\n");
//  test_4();
//  printf("Running test 5..\n");
//  test_5();
//  printf("Running test 6..\n");
//  test_6();
//  printf("Running test 7..\n");
//  test_7();
//  printf("Running tests 8..\n");
//  test_8();
//  printf("Running tests 9..\n");
//  test_9(envp);
//  printf("Running tests 10..\n");
//  test_10();
//  printf("Running tests 11..\n");
//  test_11();




  setup_signals();

  while (1) {
    printf("%s ", prompt_name);
    input_pointer = fgets(input, BUF_SIZE, stdin);

    while (input_pointer == NULL && errno == EINTR) {
      input_pointer = fgets(input, BUF_SIZE, stdin);
    }


    input[strlen(input) - 1] = '\0';


    if (strlen(input) > 0) {

      toggle_signal_block(SIG_BLOCK, SIGCHLD);

      handle_command_line(input, 0, 0, 0, command_array);
      execute_commands(command_array);
      empty_commands(command_array);

      toggle_signal_block(SIG_UNBLOCK, SIGCHLD);
    }
  }

  exit(0);
}
