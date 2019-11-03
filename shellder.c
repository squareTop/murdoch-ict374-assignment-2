#include "shellder.h"

/**
 * Run tests.
 */
void test() {
  //char sample[] = "date -foo -bar & who & ps -ef | grep foo; ls -l -t -a; who & cat < junk; cat some.file > /tmp/foo & whoami";
  //char sample[] = "date & ps -ef | grep foo; ls -l -t; cat foo.txt > /tmp/foo; cat < junk";
  //char sample[] = "cat | cat | cat | cat > junk & cat | cat | cat | cat | grep line";
  //char sample[] = "ps | sort & sleep 10 & date; cat foo.txt > /tmp/foo";
  char sample[] = "ls -l > junk ; cat < junk ; /bin/ls -lt /dev/tty* | grep tty | sort | head > junk2 & sleep 10 ; cat < junk2";
  printf("************************************************\n");
  handle_command_line(sample, 0, 0, 0, commands);
}

/**
 * Changes prompt name. Does nothing if argument is an empty string.
 * We are not checking for and trimming trailing whitespace.
 * Returns 0 if changed, 1 if not.
 * Satisfies requirement #3 and marking guide #3.
 *
 * @param  {char *} input
 * @return {int}
 */
int prompt(char * input) {
  if (strlen(input) > 0) {
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

  while (commands[index] != NULL) {
    printf("** freeing: %s, %p\n", commands[index]->name, commands[index]);

    // nullify arguments
    while (commands[index]->argv[arg_index] != NULL) {
      commands[index]->argv[arg_index] = NULL;
      arg_index++;
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
 */
void execute_commands() {
  int index = 0;
  Command * command;

  while ((command = commands[index++]) != NULL) {
    printf("execute_commands | %s, %p\n", command->name, command);
  }

  free(command);
}

/**
 * Driver program.
 * @param  {int}    argc
 * @param  {char *} argv[]
 * @return {int}
 */
int main(int argc, char * argv[]) {
  char input[BUF_SIZE];

  // ignore keyboard signals
  //ignore_signals();

  // run infinite loop; prompt for input and execute commands
  while (1) {
    printf("%s ", shell_name);
    fgets(input, BUF_SIZE, stdin);

    // remove newline character
    input[strlen(input) - 1] = '\0';

    handle_command_line(input, 0, 0, 0, commands);
    execute_commands();
    empty_commands();
  }

  exit(0);
}