#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <netdb.h>

#define BUFSIZE 1024
#define MAXFILENAME 64

char recvbuf[BUFSIZE];

using namespace std;

int main(int argc, char *argv[])
{
  int mysock;            // local socket for waiting
  int newsock;           // socket created by accept()
  sockaddr_in sockname;  // data structure identifying target endpoint
  socklen_t slen = sizeof(sockaddr_in);
  char filename[MAXFILENAME];	// target filename 
  int chunksize;         // max number of bytes in each transmission
  int len;               // number of bytes actually received from network
  
  if ( (argc != 1)) {
    cerr << "Usage: recvit (no parameters)\n";
    exit(EXIT_FAILURE);
  }

  // create a stream socket, mysock, using socket() system call; exit on return error
  // ENTER YOUR CODE HERE
  if ((mysock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    exit(EXIT_FAILURE);
  }  

  // use sockname data structure to bind() a name to the TCP socket; exit on return error
  // let the OS assign IP address and choose port number (see lab/course notes)
  // ENTER YOUR CODE HERE
  sockname.sin_family = AF_INET;
  sockname.sin_addr.s_addr = htonl(INADDR_ANY);
  sockname.sin_port = 0;

  if (bind(mysock, (struct sockaddr *)&sockname, sizeof(sockname)) == -1) {
      close(mysock);
      exit(EXIT_FAILURE);
  } 
  // get the port number of the socket from the OS and print it out
  getsockname(mysock, (struct sockaddr *) &sockname, &slen);
  cout<<"recvit: waiting on port " << sockname.sin_port << "\n";
	
  // configure the TCP socket using listen() and accept() a connection request from the base
  // assign return from accept() to newsock; exit on return error in either listen() and accept()
  // ENTER YOUR CODE HERE
  if (listen(mysock, 1) == -1) {
      close(mysock);
      exit(EXIT_FAILURE);
  }

  if ((newsock = accept(mysock, (struct sockaddr *)&sockname, &slen)) == -1) {
    close(mysock);
    exit(EXIT_FAILURE);
  }  
  cout << "Connected!\n";

  cout << "Enter name of file for output (use a DIFFERENT name than that of the file being sent!): \n";
  cin >> filename;

  // open target file for writing&creation; exit on failure
  // ENTER YOUR CODE HERE
  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    close(mysock);
    close(newsock);
    exit(EXIT_FAILURE);
  }

  cout << "Enter the receiving chunk size (0 < chunk size <= " << to_string(BUFSIZE) << ")\n";
  cin >> chunksize;
  if ((chunksize < 1) || (chunksize > BUFSIZE)) {
      cerr << "illegal chunk size; exiting\n";
      exit(EXIT_FAILURE);
  }
  

  // loop, receiving chunks of data into recvbuf, number of bytes received -> len
  // like this:  len = read(newsock,recvbuf,chunksize);
  // until read returns 0
  // ENTER YOUR CODE HERE
  while ((len = read(newsock, recvbuf, chunksize)) > 0) {
    fwrite(recvbuf, 1, len, file);
  }

  if (len == -1) {
    fclose(file);
    close(mysock);
    close(newsock);
    exit(EXIT_FAILURE);
  }

  cout << "recvit: Transfer complete.\n";
}
