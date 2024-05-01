// CSE 325 Lab 4.  lab4.cpp is the main program
// Parent process forks, child process exec's myrev process.
// Parent reads from standard input until line with a single '.' character, then closes
// pipe in order to allow myrev to read and process data from the pipe.

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
  if (pid < 0){
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

    // exec the myrev program; we'll give you the exact code:
    char *argv[] = {(char *)"./myrev", NULL};   // create argument vector for exec'd process (myrev)
    if (execv(argv[0], argv) < 0)               // exec myrev process; exec() shoud not return; exit if it does
       exit(0);      
  }


  // if we reach this point, we are the parent process


  // read end of the pipe is not used by the parent
  close(fds[0]);                 


  cout << "Enter lines of text; end with single . character:\n";

  // read lines of text into input[] array, until a single '.' character
  // forward each line to the pipe using dprintf().  e.g.,: dprintf(fds[1], "%s\n", input); 
  // ENTER YOUR CODE HERE
  while (true) {
	cin.getline(input, MAXLINE);
        if (input[0] == '.') {
            break;
        }
    dprintf(fds[1], "%s\n", input);
    }

  // close this end of the pipe; reads by the child (myrev) will block until this end of the pipe is closed
  close(fds[1]); 

  int status;
  wait(&status); // wait for child to finish before exiting
}
