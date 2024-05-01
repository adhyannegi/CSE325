#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <netdb.h>

using namespace std;

#define BUFSIZE 1024
#define MAXFILENAME 64

// buffer for holding chunks read from file prior to sending
char sendbuf[BUFSIZE];


int main(int argc, char *argv[])
{
  int mysock;          // local socket
  char dest_host[20];  // target hostname
  int dest_port;       // target port number
  sockaddr_in dest;    // data structure identifying target endpoint
  hostent *hp;         // DNS data structure
  char filename[MAXFILENAME];	// name of file to be sent
  int chunksize;       // granularity of file reads/transmissions
  int len;             // number of bytes read from file, sent to network
  
  // check usage
  if ( (argc != 3)) {
    cerr << "Usage: call hostname portnumber\n";
    exit(EXIT_FAILURE);
  }

  // copy parameters hostname and port
  strcpy (dest_host, argv[1]);
  dest_port = atoi(argv[2]);

  // create a stream socket, mysock, using socket() system call; exit on return error
  // ENTER YOUR CODE HERE
  if ((mysock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    cerr << "Error making socket\n";
    exit(EXIT_FAILURE);
  }

  /* invoke DNS to get IP address */
  hp = gethostbyname(dest_host);

  /* set up structure for name of destination socket */
  dest.sin_family = AF_INET;
  memcpy(&dest.sin_addr, hp->h_addr, hp->h_length);
  dest.sin_port = dest_port;

  // attempt to connect to dest; exit on failure (see lab/course notes) 
  // ENTER YOUR CODE HERE
  if (connect(mysock, (struct sockaddr *)&dest, sizeof(dest)) == -1) {
    close(mysock);
    exit(EXIT_FAILURE);
  }

  cout << "Connected!\n";

  cout << "Enter name of file to send: \n";
  cin >> filename;
  cout << filename << "\n";

  // open source file for reading; exit on failure
  // ENTER YOUR CODE HERE
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
      close(mysock);
      exit(EXIT_FAILURE);
  }

  cout << "Enter the sending chunk size (0 < chunk size <= " << to_string(BUFSIZE) << ")\n";
  cin >> chunksize;
  if ((chunksize < 1) || (chunksize > BUFSIZE)) {
      cerr << "illegal chunk size; exiting\n";
      exit(EXIT_FAILURE);
  }

  // loop, reading a chunk of the file into recvbuf, sending to network with write()
  // until entire file has been sent
  // ENTER YOUR CODE HERE
  while ((len = fread(sendbuf, 1, chunksize, file)) > 0) {
    ssize_t sent_bytes = write(mysock, sendbuf, len);
    if (sent_bytes == -1) {
        perror("write");
        fclose(file);
        close(mysock); 
        exit(EXIT_FAILURE);
      }
  }

  if (len == -1) {
    fclose(file); 
    close(mysock); 
    exit(EXIT_FAILURE);
  }

  fclose(file);

  cout << "sendit: Transfer complete.\n";
}
