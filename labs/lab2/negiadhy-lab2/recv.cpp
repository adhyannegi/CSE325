/* CSE 325 Lab 2 receiver program using shared memory and semaphore */
#include <iostream>
#include <unistd.h>
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_post(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
#include <string.h> 

#define MAXLINE 256

#define semName "negiadhySem"  //call it netidSem, where netid is your MSU netid

using namespace std;

int main(int argc, char * argv[]) 
{
    key_t shm_key;                 /*      shared memory key       */
    int shm_id;                    /*      shared memory id        */
    sem_t *mysem;                   /*      synch semaphore         *//*shared */
    char *myaddr;                       /*      shared variable         *//*shared */
    
    // call ftok() to create a shared memory key using the path to your home directory (e.g., /user/your-netid) and any integer > 0.
    // assign return value to shm_key; print out the key value in hex (see demo)

    // ENTER YOUR CODE HERE (same as for send.cpp)
    shm_key = ftok("/user/negiadhy", 1);
    cout << "shm_key is 0x" << hex << shm_key << dec << endl;

    // with the above key, call shmget() to create a shared memory region of size MAXLINE
    // third parameter should be: 0600 
    // return value assigned to shm_id; check for errors and exit if error

    // ENTER YOUR CODE HERE (almost same as for send.cpp, only difference is third param)
    shm_id = shmget(shm_key, MAXLINE, 0600);
    if (shm_id < 0) {
        exit(0);
    }

    // call shmat(shm_id, NULL, 0) to attach shared memory segment; return value goes in myaddr
    // print out the address (see demo)

    // ENTER YOUR CODE HERE (same as for send.cpp)
    myaddr = (char *) shmat(shm_id, NULL, 0);
    cout << "Shared memory attatched at address: " << (void *) myaddr << endl;

    // open the semaphore created by send.cpp by calling: sem_open (semName, 0)
    // return value assigned to mysem; check for errors; exit if error

    // ENTER YOUR CODE HERE
    mysem = sem_open (semName, 0);
    if (mysem == SEM_FAILED){
      exit(0);
    }

    cout << "semaphore " << semName << " opened.\n\n";

    // loop, waiting on mysem, printing the string starting at myaddr, until the line is a single "."

    // ENTER YOUR CODE HERE
    while (true){
      sem_wait(mysem);
      cout << myaddr << endl;
      if (myaddr[0] == '.' && myaddr[1] == '\0'){
	break;
	}
    }

    // clean up by closing mysem and detaching from shared segment (code is given below)
    sem_close(mysem);
    if (shmdt(myaddr) < 0){
      cout << "shmdt() failed!\n";
    }
}
