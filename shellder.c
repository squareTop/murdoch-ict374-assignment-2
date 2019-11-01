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
  handle_command_line(sample, 0, 0, commands);
}

/**
 * Driver program.
 * @param  {int}    argc
 * @param  {char *} argv[]
 * @return {int}
 */
int main(int argc, char * argv[]) {
  char input[BUF_SIZE];
  //char shell_name[BUF_SIZE] = "%";
  char * shell_name = "%";

  while (1) {
    printf("%s ", shell_name);
    fgets(input, BUF_SIZE, stdin);

    // remove newline character
    input[strlen(input) - 1] = '\0';

    //shell_name = strdup(input);

    handle_command_line(input, 0, 0, commands);
  }

  exit(0);
}