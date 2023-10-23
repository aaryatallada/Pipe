## UID: 805819572

## Pipe Up

Used a loop through all arguments and used child and parent process to manipulate file descriptors of a pipe and STDIN and STDOUT to connect the processes to one another. 

## Building

to run my program, you run this command: gcc pipe.c -o pipe for the initial build. Now there is an executable pipe file which you can type:
./pipe program1 program2 etc... to run. 


## Running

./pipe ls cat wc 
returns:
6       6      56

## Cleaning up

to clean up run this command: make clean
