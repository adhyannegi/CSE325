// CSE 325 Lab 4.  myrev.cpp is the exec'd program
// Parent (myrev) creates a pipe, forks, child process exec's /usr/bin/sort process.
// Parent reads from standard input until line with a single '.' character, reverses the order
// of the characters in each line, forwards the result to the write end of the pipe, then closes the pipe.

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;

#define MAXLINE 256
char input[MAXLINE];
char output[MAXLINE];

int  main(int argc, char *argv[]) {
  int fds[2];                      // an array that will hold two file descriptors returned by pipe()
  pid_t pid;                       // holds return value from fork()

  // create pipe using pipe() to populate fds with two file descriptors
  // fds[0] is the read end, fds[1] is the write end
  // if pipe() fails, exit(0)
  // ENTER YOUR CODE HERE
  if (pipe(fds) < 0){
    exit(0);
  }
  // create child process with
  // pid = fork();                    
  // if fork() fails, exit(0)
  // ENTER YOUR CODE HERE
  pid = fork();
  if (pid < 0) {
    exit(0);
  }

  if (pid == 0) // this is the child process
  {
    // use dup2 to duplicate the read end of pipe (fds[0]) to standard input (STDIN_FILENO) -- see handout
    // close both ends of the pipe since not needed in child
    // ENTER YOUR CODE HERE
    dup2(fds[0], STDIN_FILENO);
    close(fds[0]);
    close(fds[1]);

    //  and the sort program; we'll give you the exact code:
    char *argv[] = {(char *)"/usr/bin/sort", NULL};   // create argument vector for exec'd process (sort)
    if (execv(argv[0], argv) < 0)                    // exec sort process; exec() should not return; exit if it does
      exit(0);  
  }


  // if we reach here, we are in parent process

  // read end of the pipe is not used by the parent
  close(fds[0]);                 


  // read lines of text into input[] array, until a single '.' character
  // reverse the characters in each line and place the result in the output[] array
  // note: be sure to put a 0 character at the end of the the line in output[]
  // forward each line from output[] to the pipe using dprintf().  e.g.,: dprintf(fds[1], "%s\n", output; 
  // ENTER YOUR CODE HERE
  while (fgets(input, MAXLINE, stdin) != NULL && input[0] != '.'){
  for (size_t i = 0; i < strlen(input); i++){
        output[i] = input[strlen(input) - i - 2];
    }
    output[strlen(input) - 1] = '\0';
    dprintf(fds[1], "%s\n", output);
  }
  // close this end of the pipe; reads by the child (sort) will block until this end of the pipe is closed
  close(fds[1]); 

  int status;
  wait(&status); // wait for child to finish before exiting
}
