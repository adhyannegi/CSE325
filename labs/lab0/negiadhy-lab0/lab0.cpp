#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;

// for tracing execution
string tracefile;
ofstream tracestream;

// for reading from the /proc file system
ifstream commandstream;

// function to print message in case of invalid usage
void usage();

// function to trace execution
void trace(string s);

// main(int argc, char * argv[])
// This is a 'C' standard format to handle command-line parameters
//   argc: the number of command-line parameters.  This includes
//         the name of the executable, so argc will always be >= 1.
//   argv: An array of pointers to 'C' strings, not instances of the 'C++'
//         String class.  This array of pointers is NULL terminated.

// main function for lab0.cpp
// Usage: lab0 <netid> <tracefile>
// Parameters:
//   netid - MSU netid of user
//   tracefile - file used by the trace function.

int main(int argc, char * argv[])
{
  string netid;
  string myname = "Adhyan Negi";
  string commandpath;
  string command;
  pid_t pid;
  uid_t uid;

  // should be one command line parameters, netid
  if(argc != 3)
	{
	  usage();
	  return(-1);
	}

  // get command line parameters
  netid = argv[1];
  tracefile = argv[2];

  // get process id
  pid = getpid();

  // open the trace file for writing
  tracestream.open (tracefile, ios::app);
  if (tracestream.fail()) {
    cout<< "cannot open file " + tracefile + "; exiting.";
    exit(0);
  }
  trace("lab0: process " + to_string(pid) + " starting.\n\n");
  trace("lab0: command-line parameters are " + netid + ", " + tracefile + "\n\n");

  trace("lab0: the user is " + myname + "\n\n");

  // get user id
  uid = getuid();
  trace("lab0: the user id is " + to_string(uid) + "\n\n");

  // get the name of the executable (command) from /proc file system
  commandpath = "/proc/" + to_string(pid) + "/comm";
  commandstream.open (commandpath, std::ifstream::in);
  if (commandstream.fail()) {
    cout<< "cannot open file " + commandpath + "; exiting.";
    exit(0);
  }

  getline(commandstream, command);
  
  trace("lab0: the command name found in " + commandpath + " is " + command + "\n\n");

  // sleep for half a second (500000 microseconds)
  usleep(500000);

  trace("lab0: exiting \n\n");
}

void usage()
{
  cout << "Usage: lab0 netid tracefile\n";
  cout << "Where:\n";
  cout << "lab0 - The name of the executable.\n";
  cout << "netid - MSU netid of the user.\n";
  cout << "tracefile - file used by the trace function.\n";
}

void trace(string s)
{
  struct timeval t;
  gettimeofday(&t, NULL);
  tracestream << "Time: " << to_string(t.tv_sec % 10000) << "." << to_string((int) t.tv_usec/100) << ": " << s;
  tracestream.flush();
}











































