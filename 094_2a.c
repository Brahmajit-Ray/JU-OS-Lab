/*
*Name-Brahmajit Ray
 Roll-002211001094
 5th Aug 12:00

*Question: Catch the signal "SIGINT" and display "Ha Ha, Not Stopping". Use "signal" system call. Always use "perror" to check the return status of a library/system call

*Input Description-Ctrl+C

*Output Description-Ha Ha, not stopping

*Compilation Command: gcc 094_2a.c
*Execution Command  : ./a.out

*Sample Output:

Loop Running
Loop Running
Loop Running
Loop Running
Loop Running
^C
Ha Ha,not stopping
Loop Running
Loop Running
Loop Running
^C
Ha Ha,not stopping
Loop Running
Loop Running
Loop Running
^C
Ha Ha,not stopping
Loop Running
Loop Running
Loop Running
^Z
[2]+  Stopped                 ./a.out

*/


#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void handle_sigint(int sig){
	if(sig==SIGINT){
		printf("\nHa Ha,not stopping\n");
	}
}

int main(){
	if(signal(SIGINT,handle_sigint)==SIG_ERR){
		perror("Error handling signal\n"); //perror used
	}

	while(1){                                  //Infinite Loop
		printf("Loop Running\n");
		sleep(1);
	}
	
	return 0;
}
