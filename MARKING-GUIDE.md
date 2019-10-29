# Marking guide

1. **Compilation (10 marks)**

    * Check to see whether executable is provided
    * Check to see whether the program can be built using the makefile and the source code
    
    Marks:

    * No makefile, no more than 5 marks.
    * No executable, no more than 8 marks.
    * Does not compile, no more than 5 marks.

1. **Basics (10 marks)**

    Test the following commands:

    * `% ls`
    * `% ps`
    * `% ./show a b c`
    * whether you can type and run simple command lines repeatedly

    Note the command "show" is available from the Tutors’ page (tests.tar) but you may need to recompile it.

1. **Built-in commands (5 marks)**

    Check command "prompt", “pwd”, "cd" and "exit". Suggested commands:

    * `% prompt myshell`
    * `% cd /tmp`
    * `% pwd`
    * `% cd` (check whether it goes to the home directory)
    * `% pwd`
    * `% cd ..`
    * `% pwd`
    * `% exit`
    
    Suggestion: 1 mark for each of “prompt”, “pwd” and “exit”. 2 marks for “cd”.

1. **Long command (5 marks)**

    Check whether the shell can take a command with many command line arguments:

    * `% ls –l  -t   a b c`  
    * `% show a  bb  ccc  dddd 1  22  333  4444  555555`  
    * `% show  a b c d e f g h I j k l m n o p q r s t u v w x y z 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 20 abc123xyz`

1. **Wildcards (5 marks)**

    Check:

    * `% ls -l /tmp/*.c`
    * `% ls -l /tmp/a*c`
    * `% ls -l /tmp/abc.?`
    * `% ls -l /tmp/abc*.?`
    * `% ls -l /dev/*`

    Suggestion: create a few files with different name patterns in a fixed directory such as `/tmp`.

1. **Sequential execution ";" (5 marks)**

    Check:

    * `%  sleep 10  ;  echo hello`
    * `%  sleep 10  ;  ls –l`
    * `%  sleep 10  ;  echo hello1  ;   sleep 10 ;  echo hello2`

    There should be 10 seconds delay before each command output.

1. **Concurrent execution "&" (5 marks)**

    Check:

    * `%  echo hello  &   echo  world`
    * `%  sleep 10  &  echo hello`
    * `%  ps  &   ls`
    * `%  echo ps-command &  ps   &   echo ls-command  &  ls –l`
    * `%  sleep 10  &`

    The output from all commands should be generated pretty much simultaneously.

1. **Standard input redirection "<" (5 marks)**

    Check:

    * `% cat  <  /tmp/foo`
    * `% grep  line  <  /tmp/foo`

1. **Standard output redirection ">" (5 marks)**

    Check:

    * `% ls -l  >  /tmp/junk`
    * `% cat  /tmp/foo >  /tmp/junk2`

1. **Simple shell pipeline (5 marks)**

    Check:

    * `% cat /tmp/foo |  cat`
    * `% cat  /tmp/foo |  grep  line `
    * `% cat  /tmp/foo  |  sort`
    * `% cat  /tmp/foo  |  sort  -r`

1. **Long shell pipeline (5 marks)**

    Check:

    * `% cat /tmp/foo |  sort  |  sort –r  |  grep  line `
    * `% cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat `
        
        In the above command line, you should be able to type a line from the keyboard and see the echoed line as well as the output line. The two lines should be the same. You should be able to repeatedly type lines. End the "file" by typing Ctrl-D.

    * `% cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat > junk`

        You should compare the echoed lines on screen and the content of the file "junk".

    * `% cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | grep line`

        You should see each echoed line and those lines containing word "line".

1. **Combinations (10 marks)**
    
    Check:

    * `%  ls -l   > junk  ;  cat < junk ;  /bin/ls  -lt  /dev/tty*  |  grep tty  |  sort  |  head  > junk2  &  sleep 10  ;  cat  < junk2`
    * `% ls –lt | cat > junk ;  ps | sort  &  echo ps-output ; sleep 10 & echo wait-for-10seconds ; cat junk | cat | grep a | sort -r`

1. **Ignore Ctrl-C, Ctrl-\ and Ctrl-Z (5 marks)**

    Check whether the shell ignores `SIGINT`, `SIGQUIT` and `SIGTSTP` by typing Ctrl-C, Ctrl-\ and Ctrl-Z from shell prompt.

1. **Claim of zombies (5 marks)**

    You should check whether the shell has claimed zombie processes. One way to check this is: while running the shell in one terminal, open up another terminal. In the shell prompt, type the command "sleep 1 &" several times. In the other terminal, use the command "ps –elfH | grep your-userid" to check whether there are zombies (defunct children of the shell). 

1. **Handling of slow system calls (5 marks)**

    In the shell command loop where it reads a command line, a standard function such as fgets will be used. Ultimately these standard input functions would have to be implemented using the read system call. The blocking read can return prematurely when a signal is received (rather than when it receives an input). If such kind of event is not handled, the shell may behave erratically, such as suddenly displaying a shell prompt. It is a bit hard to check this feature, but I suggest that you can use one of the two ways:
    
    1. checking the code to see whether it is handled. I have released a note in the Project Page which documented this problem and its solution (See Notes on Project Implementation under Project Page)
    1. Send a signal such as SIGCHLD to the shell to see whether it would change its behaviour. 

1. **Compliance to Documentation Requirements and Submission Guidelines (10 marks)**
    
    1. The completed Project Declaration Form
    1. List of files
    1. Project title and description of the project
    1. Filenames used in submission
    1. Overall quality of presentation

    Suggestion: award 2 marks for each of the above
