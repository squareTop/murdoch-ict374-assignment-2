# Project 1: A Simple Unix Shell

## Table of contents

1. Description
1. Requirements
1. Definition of Command Line Syntax
1. A Note on Implementing the Command Line Parser
1. A Note on Claiming Zombie Processes as You Go
1. A Note on Interruption of Slow System Calls by Signals

## Description

Design and implement a simple UNIX shell program using the grammar specified in the "Definition of Command Line Syntax" section of this readme.

## Requirements

1. **At least 100 commands in a command line**
1. **At least 1000 arguments in each command**
1. **Reconfigurable shell prompt (default %)**

    The shell must have a shell built-in command prompt for changing the current prompt. For example, typing the following command:  
    `% prompt john$`  
    ... should change the shell prompt to `john$`, i.e., the second token of the command.
  
1. **The shell built-in command pwd**

    This command prints the current directory (also known as working directory) of the shell process.

1. **Directory walk**

    This command is similar to that provided by the Bash built-in command `cd`. In particular, typing the command without a path should set the current directory of the shell to the home directory of the user.

1. **Wildcard characters**

    If a token contains wildcard characters `*` or `?` the token is treated as a filename. The wildcard characters in such a token indicate to the shell that the filename must be expanded. For example the command:  
    `% ls *.c`  
    ... may be expanded to ls ex1.c ex2.c ex3.c if there are three matching files ex1.c ex2.c ex3.c in the current directory. You may implement this feature using the C function `glob`.

1. **Standard input and output redirections > and <**

    For example:  
    `% ls -lt >  foo`  
    would redirect the standard output of process `ls -lt` to file `foo`. Similarly in the following command:  
    `% cat <  foo`  
    the standard input of process `cat` is redirected to file `foo`.

1. **Shell pipeline |**

    For example:  
    `% ls -lt | more`  
    the standard output of process `ls -lt` is connected to the standard input of process `more` via a pipe.

1. **Background job execution**

    For example:  
    `% xterm &`  
    The commannd line starts command `xterm` in the background (i.e., the shell will not wait for the process to terminate and you can type in the next command immediately). The following command line:  
    `% sleep 20 &  ps -l`  
    ... starts command `sleep 20` and immediately execute command `ps -l` without waiting for command `sleep 20` to finish first.

1. **Sequential job execution**

    For example the command line:  
    `%  sleep 20 ; ps -l`  
    ... starts command `sleep 20` first, and wait for it to finish, then execute command `ps -l`.

1. **The shell environment**

    The shell should inherit its environment from its parent process.

1. **The shell built-in command exit**

    Use the built-in command exit to terminate the shell program.
  
The behaviour of the above commands (except `prompt`) should be as close to those of the Bash shell as possible. In addition, your shell should not be terminated by CTRL-C, CTRL-\, or CTRL-Z.

Finally you must not use any existing shell program to implement your shell (for example by calling a shell through the function `system`). That would defeat the purpose of this project.

In the above, commands such as `ls`, `cat`, `grep`, `sleep`, `ps` and `xterm` are used as examples to illustrate the use of your shell program. However your shell must be able to handle any command or executable program. Note the commands `prompt`, `pwd`, `cd` and `exit` should be implemented as shell builtins, not as external commands.

The syntax and behaviour of the built-in commands `pwd`, `cd` and `exit` should be similar to the corresponding commands under Bash shell.

A major part of this shell is a command line parser. Please read the this note for suggestions on implementing the parser.

## Definition of Command Line Syntax

The following is the formal definition of the command line syntax for the shell, defined in Extended BNF:

```
   < command line >      ::=   < job >
                             | < job > '&'
                             | < job > '&' < command line >
                             | < job > ';'
                             | < job > ';' < command line >

   < job >               ::=   < command >
                             | < job > '|' < command >

   < command >           ::=   < simple command >
                             | < simple command > '<' < filename >
                             | < simple command > '>' < filename >

   < simple command >    ::=   < pathname >
                             | < simple command >  < token >
```
An informal definition plus additional explanations of the syntax is given below:

1. A **command line** consists of one or several **jobs** separated by the special character "&" and/or ";". The last **job** may be followed by the character "&" or ";". If a **job** is followed by the character "&", then it should be executed in the background.

1. A **job** consists of one or more **commands** separated by pipeline characters "|";

1. A **command** is either a simple **command** or a **simple command** followed by an input redirection (< filename) or an output redirection (> filename);

1. A simple **command** consists of a single **pathname** followed by zero or more **tokens**;

1. The following five characters are the **special characters**: &, ;, |, < , > ;

1. The **white space characters** are defined to be the space character and the tab character;

1. A **token** is either a **special character** or a string that does not contain space characters or **special characters**. In this project we do not consider quoted strings. Therefore if single quote or double quote characters appear in a string, they are treated just like any other **non-special characters** without its usually special meaning;

1. **Tokens** must be separated by one or more white spaces;

1. A **pathname** is either a file name, or an absolute **pathname**, or a relative **pathname**. Examples of **pathnames** are `grep`, `/usr/bin/grep`, `bin/grep` and `./grep`;

1. A **command** line must end with a newline character.

## A Note on Implementing the Command Line Parser

A major part of Project 1 is a command line parser. You need to break a command line into an array of commands and each array element contains the detail of one command. I suggest that you use a structure similiar to the one given below to represent a command.

```
struct Command_struct { 
     char *com_pathname;     // what is the path name of the command
     int argc;               // the number of arguments to the command
                             // including the command itself
     char *argv[MAX_ARGS];   // pointers to strings, each string
                             // is an argument for the command, including
                             // argument "0". The last pointer should
                             // be set to NULL. 
     char *redirect_in;      // if this is not NULL, then the standard input
                             // is redirected to the given file name
     char *redirect_out;     // if this is not NULL, then the standard output
                             // is redirected to the given file name
     char com_suffix;        // ' ' - no command suffix (last command);
                             // '&' - the command is followed by '&'
                             // ';' - the command is followed by ';';
                             // '|' - the command is followed by '|'.
};
```

[The sample parser here](https://www.it.murdoch.edu.au/~S900432D/oli1l1hsu_x3X64dg72kf7Th973yihbkys9M10in0x/assignments/a2/Sample_Parser.tar) uses the above structure for commands. You may use this sample parser as the starting point for your new parser for the shell. Please note the sample parser provided here does not fully meet the requirements of this project. It is highly likely that it also contains errors. It is meant to provide a (good) foundation for developing the required parser. If you choose to use it in this project, then it is your responsibility to improve it so that it meets all requirements of the project.

You may want to use the fucntion `printComStruct` given below to test the parser:

```
void printComStruct(struct Command_struct *com)
{ 
    int i;

    fprintf(stderr,"com_pathname=%s\n", com->com_pathname); 
    fprintf(stderr,"argc=%d\n", com->argc); 
    for(i=0;  com->argv[i]!=NULL; i++) 
        fprintf(stderr,"argv[%d]=%s\n", i, com->argv[i]); 
        fprintf(stderr,"#######\n"); 
        if (com->redirect_in == NULL)
            fprintf(stderr,"redirect_in=NULL\n"); 
        else
            fprintf(stderr,"redirect_in=%s\n", com->redirect_in); 
        if (com->redirect_out == NULL)
            fprintf(stderr,"redirect_out=NULL\n"); 
        else
            fprintf(stderr,"redirect_out=%s\n", com->redirect_out); 
        fprintf(stderr,"com_suffix=%c\n\n", com->com_suffix); 
} 
```

For example, if the input line is: `/bin/sleep 10 & ls -lt xyz > junk`, calling the above function should result in a "dump" of the raw contents of two structures (because there are two commands) shown below:

```
*** ICT310 comstruct dump begin ***
command[0]:
com_pathname=/bin/sleep
argc=2
argv[0]=/bin/sleep
argv[1]=10
#######
redirect_in=NULL
redirect_out=NULL
com_suffix=&

command[1]:
com_pathname=ls
argc=3
argv[0]=ls
argv[1]=-lt
argv[2]=xyz
#######
redirect_in=NULL
redirect_out=junk
com_suffix= 

*** ICT310 comstruct dump end ***
```

## A Note on Claiming Zombie Processes as You Go

The background commands, i.e., those with a postfix `&`, are not waited for by your shell process in the main loop. As the result once they die they become zombie processes. Each zombie process would still take one entry in the Process Table. If your shell process does not clean them up they would accumulate over time. In theory this could exhaust the Process Table, thus frozen the machine. Therefore it is necessary for your shell process to claim (i.e., clean up) those zombie processes as soon as possible.

The best place to claim the zombies would be in the signal handler for `SIGCHLD`. When a process terminates, it sends a `SIGCHLD` to its parent process. However since signals are not queued, when a `SIGCHLD` arrives, it is possible that there may be more than one zombie of yours in the system. This means your signal handler should try to claim as many zombies as you can. This would involve with a loop similar to the following one:

```
int more = 1;        // more zombies to claim
pid_t pid;           // pid of the zombie
int status;          // termination status of the zombie

while (more) {
    pid = waitpid(-1, &status, WNOHANG);
    if (pid≤0) 
        more = 0;
}
```

The option, `WNOHANG`, is necessary so that the handler is not blocked when there are no more zombies.

Note if you are waiting for a child using one of the wait system calls, and you have also installed signal handler for `SIGCHLD`, when `SIGCHLD` arrives, not only wait will return, but the signal handler will also be called.

## A Note on Interruption of Slow System Calls by Signals

Some I/O related system calls are known as slow system calls, due to the fact that they take a long time to complete. For example, read from a keyboard using system call read is a slow system call, as it would normally block the calling process until the user has typed something on the keyboard.

Many slow system calls can be interrupted by an incoming signal. If a signal arrives while the system call is still waiting for data, the system call would return prematurely with `errno = EINTR`.

Such things can also happen to many standard input/output functions such as `fgets`, because these functions are ultimately implemented with slow system calls such as read. When they happen you will see some unexpected behaviour from your program. Let's look at the example below.

It is very likely your shell would have a loop to print the prompt and wait for the next command line, something similar to the following algorithm:

```
while (1) {
     1) print the shell prompt
     2) read a line using, eg, fgets
     3) analyse the command line
     4) for each job in the command line
        4.1) if the job is the exit command, then terminate the program
        4.2) create child processes (and pipes, redirections etc) to execute that job
        4.3) if the job is a background job (ie ended with &)
              continue    // go back to for loop
        4.4) wait for the job to finish
}
```

Note step 2) involves with a slow system call (inside the function `fgets`). Imaging that you type a background job such as `sleep 10` & at the shell prompt. As soon as you hit `ENTER` key, the new command line would be analysed at step 3), and executed at step 4.2. Since it is a background job, your shell would not wait for its completion. It simply goes back to the beginning of the for loop. Since there is only one job in this command line, the for loop would end, hence the program goes back to the beginning of the while loop. It will then print the next prompt, waiting for the user to type the next command line at step 2) again.

Note that at this point your shell is blocked at the slow read system call inside `fgets` function waiting for keyboard input.

At about 10 seconds later, the child process created to execute the command `sleep 10` would terminate. It would send a `SIGCHLD` to your shell process. Note when the signal arrives, your shell process is blocked at the slow system call read. This signal would interrupt the slow system call, causing `fgets` to return prematurely with a `NULL` pointer and without anything read into the line buffer). Your shell would quickly go through steps 3) and 4) and comes back to step 1) to print yet another prompt at your previous shell prompt (double shell prompt). If your code relies on the return value from `fgets`, you process may even crash with a segmentation fault.

To handle the above problem, I suggest that you modify step 2) as follows:

```
int again = 1;
char *linept;        // pointer to the line buffer

while (again) {
      again = 0;
      linept = fgets(line, LENGTH_OF_LINE, stdin);
      if (linept == NULL) 
          if (errno == EINTR)
              again = 1;        // signal interruption, read again
}
```
