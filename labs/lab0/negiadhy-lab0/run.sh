#!/bin/bash         
# first line of any bash script file should be "#!/bin/bash"
# btw bash is an acronym for "Bourne Again Shell"

num_param=1

# command-line parameters are designated by a '$' followed immediately by 
# a number indicating the parameter's relative position.
#
# $1 --> MSU netid of user
if [ $# -ne $num_param ]
then      
	#    Let user know what is acceptable usage
	echo ""
	echo ""
	echo "Usage:"
	echo " run.sh <your-MSU-netid>"
	echo " "
else
	# run the program
	rm -rf trace.txt     # If a copy of the output file exists, delete it.
	echo COURSE: CSE 325, Spring 2024 >> trace.txt
	echo DATE: `date` >> trace.txt
	echo " " >> trace.txt
	echo USER: $USER >> trace.txt
	echo HOST: $HOST >> trace.txt
	echo -n 'Number of processors: ' >> trace.txt
	grep ^processor /proc/cpuinfo | wc -l >> trace.txt
	echo -n 'Size of main memory: ' >> trace.txt
	size=`grep MemTotal /proc/meminfo | awk '{print $2}'`
	echo -n $size >> trace.txt
	echo ' kilobytes' >> trace.txt
	echo " " >> trace.txt
	./lab0 $1 trace.txt 
fi   # end if-else statement

exit 0

