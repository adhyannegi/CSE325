#include <stdio.h>
#include <signal.h>
#include <x86intrin.h>
#include <sys/time.h>
#include <cstdint>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;

// state of this process
pid_t mypid;
pid_t otherpid;

// function to print message in case of invalid usage
void usage();

void sigint_handler(int signo)
{
	  cout << "\nYes, may I help you?\n\n";

	  // ENTER YOUR CODE TO:
	  char choice;
	  // loop, prompting the user to enter a, c, p, or e
	  while (true){
		cout << "Enter a (to set alarm)" << std::endl << "c (to do nothing)" << std::endl << "p (to annoy the other process)" << std::endl << "e (to exit)" << std::endl;
		cin >> choice;
	  // 'a' entered: set alarm to fire in 2 seconds
		if (choice == 'a'){
			alarm(2);
			break;
		}
	  // 'c' entered: do nothing
		if (choice == 'c'){
			break;
		}
	  // 'p' entered:
		if (choice == 'p'){
	  		cout << "\nExcuse me, process " << to_string(otherpid) << ", may I interrupt?\n\n";
	  //     sleep 2 seconds
			sleep(2);
	  //     post SIGUSR1 to other process
			kill(otherpid, SIGUSR1);
	  //     sleep 2 seconds
			sleep(2);
	  		cout << "\nOh, never mind.\n\n";
	  //     sleep 1 second
			sleep(1);
			break;
		}
	  // 'e' entered
		if (choice == 'e'){
	  		cout << "\n OK, goodbye.\n";
	  //     sleep 1 second
			sleep(1);
	  		exit(0);
		}
	  //  if anything else is entered, print an error message and go to top of loop
		else{
			cout << "Invalid input. Please enter a c p or e."<< std::endl;
		}
	}
}

void sigalrm_handler(int signo)
{
  // ENTER YOUR CODE TO:
  // post SIGUSR2 to other process
  kill(otherpid, SIGUSR2);
  cout << "\n\n TIME TO GET UP!\n\n";
  // sleep 2 seconds
  sleep(2);
}

void sigusr2_handler(int signo)
{
  cout << "\n\n TIME TO GET UP!\n\n";
  sleep(2);
}

void sigusr1_handler(int signo)
{
  cout << "\n\nWhat is it now?\n\n";
  sleep(4);
}

// main function for lab3.cpp
// Usage: lab3
// Parameters: none

int main(int argc, char * argv[])
{

  // should be only the name of the executable on the command line
  if(argc != 1)
	{
	  usage();
	  return(-1);
	}

  // get my PID and store in mypid
  mypid = getpid();

  // output my PID, get other PID
  cout << "My PID is " + to_string(mypid) + "\n";
  cout << "Enter the PID of the other process: ";
  cin >> otherpid;
  cout << "other pid is " + to_string(otherpid) + "\n";


  // declare structures for catching signals
  struct sigaction sint;
  struct sigaction salrm;
  struct sigaction susr1;
  struct sigaction susr2;

  // register sigint_handler for SIGINT signal
  memset(&sint, 0, sizeof sint);
  sint.sa_handler = sigint_handler;
  if(sigaction(SIGINT, &sint, NULL) < 0)
	cout << "can't catch SINT\n";


  // register handlers for SIGUSR1, SIGUSR2, SIGALRM signals
  // ENTER YOUR CODE HERE
    memset(&susr1, 0, sizeof susr1);
    susr1.sa_handler = sigusr1_handler;
    if (sigaction(SIGUSR1, &susr1, NULL) < 0)
        cout << "can't catch SIGUSR1\n";

    memset(&susr2, 0, sizeof susr2);
    susr2.sa_handler = sigusr2_handler;
    if (sigaction(SIGUSR2, &susr2, NULL) < 0)
        cout << "can't catch SIGUSR2\n";

    memset(&salrm, 0, sizeof salrm);
    salrm.sa_handler = sigalrm_handler;
    if (sigaction(SIGALRM, &salrm, NULL) < 0)
        cout << "can't catch SIGALRM\n";

  // Loop, sleeping for 1 second at a time
  while(1) {
	sleep(1);
	cout << "Process " + to_string(mypid) + " resting...\n";
  }

  return(0);
}


void usage()
{
  cout << "Usage: lab3\n";
  cout << "Where:\n";
  cout << "lab3 - The name of the executable.\n";
}

