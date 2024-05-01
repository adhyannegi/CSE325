// CSE 325 Lab 5: pthreads

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

#define NUM_THREADS 3

// data structure for passing parameters to threads
struct thread_data {
  int  thread_id;
  string message;
};

// declare the structures that will hold the thread parameters
struct thread_data td[NUM_THREADS];

// mutex for critical sections when threads print
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

// semaphores for synchronizing printing of keys, one for each thread
sem_t sem[NUM_THREADS];

// two sets of keys, one simple, one less so...
string key[2][NUM_THREADS] = {
  {"444444",
   "555555",
   "666666"
   },
  {"P/hr#|kr\rhwokvhhdvIvqgsqvw#oj*#|ru#o>hd#hlvhodhWjhxiwvwhVjxohrDdduqw#dh##PP#VVV#XX1",
   "V##yw#dzZqzlwlq#o/okjh#grosl\rhl#yhko\rqww#q#*#wurl#udkru/l#ur#uo#w\rgkslv#PPP#VV##XX\r",
   "Xzohkvgvk#lj#ofio\rxl#hd#i|dqRuyfhgdvEhkkshzojk\r#yr#l##x\rqr#yi#pPhD#|uv/#PP##VV#XXX\r"}
};

// print message if usage is incorrect
void usage();

// ascii to integer conversion
int atoi();


// function to implement thread; coordinate with other threads to print characters
// from keys in round-robin order
void *printMsg(void *threadarg) {

  // local variable to point to parameters
  struct thread_data *my_data = (struct thread_data *) threadarg;

  // thread identifier: e.g. 0, 1, 2
  int myid = my_data->thread_id ;  

  // string holding key used by this thread
  string s = (string) my_data->message;

  // counter for printing one character at a time
  int k;  

  // print a message identifying the thread with  myid (e.g., 0, 1, 2)
  // surround any I/O with the mutex lock/unlock so the messages does not get scrambled
  // NOTE: the order of these prints might NOT be 0, 1, 2, since we do not control the
  // order, but rather only prevent interleaving of prints

  // [INSERT YOUR CODE HERE]
  pthread_mutex_lock(&print_mutex);
  cout << "Thread " << myid << " is printing." << endl;
  pthread_mutex_unlock(&print_mutex);

  // step through the string s, one character at a time
  // you can use a C++ iterator or just step through the string with counting variable k
  for (k=0; k < s.length(); k++) {

	// wait on my semaphore, sem[myid]
	// [INSERT YOUR CODE HERE]
	sem_wait(&sem[myid]);

	// use usleep to add a little delay between each character printed, e.g. 110 milliseconds (not microseconds)
	// [INSERT YOUR CODE HERE]
	usleep(110000);

	// print out the ascii character that is 3 characters preceding s[k].
	// so, if s[k] == 'r', you should print 'o'; if s[k] == 'e', you should print 'b'

	// [INSERT YOUR CODE HERE]
	char correct_char = s[k] - 3;
    	cout << correct_char;

        // flush standard output to print character by character, instead of line by line
	cout << std::flush;

        // signal (post) the semaphore of the next thread

	// [INSERT YOUR CODE HERE]
	int next_thread = (myid + 1) % NUM_THREADS;
    	sem_post(&sem[next_thread]);
	
  }
  pthread_exit(NULL);
}

// main function for msu-msg program
// takes one integer command-line argument (0 or 1), identifying key set to translate.
int main(int argc, char * argv[]) {

  pthread_t threads[NUM_THREADS]; // for creating threads, will hold their internal IDs
  int keyindex; // identify which key set to use
  int rc; // return value
  int i;  // loop counter

  // there should be one command-line parameters, 0 or 1
  if(argc != 2)
	{
	  usage();
	  return(-1);
	}

  keyindex = atoi(argv[1]);

  if ((keyindex < 0) || (keyindex > 1))
	{
	  usage();
	  return(-1);
	}
  

  // for each thread
  for( i = 0; i < NUM_THREADS; i++ ) {

	// initialize its semaphore to be 0
	// [INSERT YOUR CODE HERE]
	sem_init(&sem[i], 0, 0);

	// print a message about creating it
	cout <<"main() : creating thread, " << i << endl;

	// set up the parameters for the thread
	td[i].thread_id = i;
	td[i].message = key[keyindex][i];

	// create the thread i with parameters *pointed to* by td[i], that is, last parameter is (void *)&td[i];
	// check for errors in return value.  if error, print message and exit(-1).

	// [INSERT YOUR CODE HERE]
	int thread = pthread_create(&threads[i], NULL, printMsg, (void *)&td[i]);
	if (thread) {
		cout << "Error: Unable to create thread. " << thread << endl;
		exit(-1);
	}
  }
  
  // post sem[0] to get things started
  // [INSERT YOUR CODE HERE]
  sem_post(&sem[0]);

  // wait for other threads to finish by using pthread_join()
  for (i = 0; i <  NUM_THREADS; i++) {

	// [INSERT YOUR CODE HERE]
	pthread_join(threads[i], NULL);
  }
  
  // call pthread_exit to finish cleanly, though not absolutely necessary after joins
  pthread_exit(NULL);
}

void usage()
{
  cout << "Usage: lab3 keyindex\n";
  cout << "Where:\n";
  cout << "lab3 - name of the executable.\n";
  cout << "keyindex - 0 or 1\n";
}


