//   lab1.cpp
//   CSE 325 Lab 1.  Cache performance and ASCII character set

//   This program uses access to the Time Stamp Counter register to
//   measure the effect of caching on memory access time, then uses
//   differences in timing to reveal a secret message.

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <x86intrin.h>	//	Linux dev
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
using namespace std;

#define PAGELEN  4096
#define NUMPAGES 256


// function to print message in case of invalid usage
void usage();

// function to trace execution
void trace(string s);

unsigned long long times[NUMPAGES];

ofstream tracestream;

//	main(int argc, char * argv[])
//	This is a 'C' standard format to handle command-line parameters
//		argc -	the number of command-line parameters.  This includes
//			the name of the executable, so argc will always be >= 1.
//		argv -	An array of pointers to 'C' strings, not instances of the 'C++'
//			String class.  This array of pointers is NULL terminated.
int main(int argc, char * argv[]) 
{

    // argc should be 2: executable name and one parameter, a filename
    if(argc != 2)
    {
        usage();
        return(-1);
    }

    int i = 0;                       // counter
    int num;                         // index of the byte to be probed
    char tmp;                        // character variable to hold result of probing the array
    unsigned long long start, stop;  // start and stop rdtsc "timer"
                                     // NOTE: any variables storing rdtsc-obtained values need to be unsigned long long
    string secret = "\nThe secret message is: "; // we will compose the secret codeword and append to this string

	tracestream.open ("trace.txt", ios::app);

	ifstream input_file(argv[1]);
	if (!input_file.is_open()) {
	  cerr << "ERROR: Could not open the file - '" << argv[1] << "'" << endl;
	  exit(0);
	}

	//allocate space for array
	char * largearray = (char*)malloc(NUMPAGES*PAGELEN);
	if (largearray == NULL)
	{
	  cout << "Memory allocation error!\n";
	  exit(0);
	}
	
	cout << "Input numbers:\n";

	// In a loop, read one number at a time from the input_file into variable num and check its validity, specifically,
	// 0 <= num < NUMPAGES*PAGELEN. Print out the number. If it is invalid, print an error message and call exit(0).
	// If it is valid, use it to access the largearray, specifically, read the byte at location num
	// of the array and store the value in tmp.
	while (input_file >> num) {
	  cout << "Probe location: " << num << "\n";
          trace("Probe location: " + to_string(num) + "\n");
	  

	  // check validity: num must be >= 0 and < NUMPAGES*PAGELEN
	  // if num is invalid, print an error message and exit
	  // else "probe" the largearray by reading the byte at location num into variable tmp

	  // [ENTER YOUR CODE HERE]
	  if ((num >= 0) && (num < (NUMPAGES * PAGELEN))){
		tmp = largearray.at(num);
	  }
	  else {
		cout << "Error!\n";
		exit(0);
	  }
	}
	cout << "\n";
	input_file.close();


	//  In a loop, read the first byte of each page of the largearray into the variable tmp.
	//  Use __rdtsc() to measure the cycles elapsed for each read operation using the start and
	//  stop variables, and record the number of cycles elapsed in the corresponding element of the times[] array.
	//  E.g., the time (number of cycles) needed to read the first byte of the ith page should be placed in times[i].
	//  NOTE: don't put any I/O in this loop!  It will affect the timing.

	//  [ENTER YOUR CODE HERE]
	for (int i = 0; i < NUMPAGES; ++i){
		start = __rdtsc();
		tmp = largearray[i * PAGE_LEN];
		stop = __rdtsc();
		times[i] = stop - start;
	}


	//  In a loop, for each of the NUMPAGES pages:
	//  1. Print a line that includes the page number and the number of cycles needed to read from that page (times[i] for page i).
	//  2. If the value of times[i] is < 200 cycles:
	//     a. print another line stating, "APPARENTLY CACHED!"
	//     b. IF (i > 0) then append the ASCII character corresponding to the integer i-10 to the secret message if such a
	//                   character exists (e.g., if i-10 is negative, no corresponding ASCII character exists) and is printable
	//        Note 1: access to page 0 is usually fast, so we ignore this case.
	//        Note 2: if the corresponding character is not printable (((i-10) < 32) or ((i-10) > 126)),
	//                then print an error and DO NOT append the character (this should not happen with the input files given
	//                but may help in debugging)

        // [ENTER YOUR CODE HERE]
		for (int i = 0; i < NUMPAGES; ++i){
			cout << "Page Number: " << i << endl; << "Number of cycles: " << times[i] << endl;
			if (times[i] < 200){
				cout << "APPARENTLY CACHED!";
				if (i > 0){
					int asciiValue = i - 10;
					if (((asciiValue) < 32) or ((asciiValue) > 126)){
						cout << "Error!\n";
					}
					else {
						secret += char(asciiValue);
					}
				}
			}
		}


	// we increment tmp simply in order to silence a compiler warning
	tmp++;

	// append a newline to the secret and output the string
	secret += '\n';
	cout << secret;
	trace(secret);
	return(0);
}


/* function informing user of improper usage */
void usage()
{
	printf("Usage: lab1 inputfile \n");
	printf("Where:\n");
	printf("lab1 - The name of the executable.\n");
	printf("input - File containing a list of integers 0 <= n <  1048576 (2^20)");
}


/* function for adding a trace string to a give trace file */
void trace(string s)
{
	tracestream << "Trace: " << s;
	usleep(10000);
	tracestream.flush();
}
