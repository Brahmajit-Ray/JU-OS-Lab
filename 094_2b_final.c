/*
 Name-Brahmajit Ray
 Roll-002211001094
 2nd Sep 11:30

*Question: Create two processes. Transfer 1GB file from process1 to process2 using a
                     FIFO. Now, transfer the same file from process2 to process1 using another
                     FIFO. Now, compare the two files to make sure that the same file has returned
                     back. Also, print the time required to do this double transfer. Attach this output
                     to the source file as a comment.
                     To create FIFO, you can either use shell command or system call.
                     To create a large file you can use the relevant command.
                     Use ls –l command to show the FIFO and the large file. Attach this output to
                     the source file as a comment

Compilation Command - gcc 094_2b_final.c
Execution Command   - ./a.out
 
Output:
>>> Opened the file initialFile.txt for reading
>>> Writing to FIFO fifo1
>>> Opened the file receivedFile.txt for writing
>>> Reading from FIFO fifo1
>>> Writing to FIFO fifo1 is completed
>>> File received from process 1
>>> Writing to FIFO fifo2 from the file receivedFile.txt
>>> Writing to fifo2 is completed
>>> Reading from fifo2 and writing to the file receivedBackFile.txt to compare received back file with the initial file sent by process1
>>> Both the files are same, there is no mismatch!
>>> The round-about time for transferring the file between process1 and process2 and back to process1 is 46 milliseconds




*/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

long getTimeElapsed()
{
    struct timeval timeElapsed;
    gettimeofday(&timeElapsed, NULL) ;
    return timeElapsed.tv_sec * 1000 + timeElapsed.tv_usec / 1000;
}

void compareFiles(FILE *firstFile, FILE *secondFile)
{
    char firstFileChar = getc(firstFile) ;
    char secondFileChar = getc(secondFile) ;

    int lineNumber = 1;
    int totalErrors = 0;

    while (firstFileChar != EOF && secondFileChar != EOF)
    {
        if (firstFileChar == '\n' && secondFileChar == '\n')
        {
            lineNumber++;
        }
        if (firstFileChar != secondFileChar)
        {
            totalErrors++;
            printf(">>> Mismatch occurred at line number : %d\n", lineNumber) ;
        }

        firstFileChar = getc(firstFile) ;
        secondFileChar = getc(secondFile) ;
    }

    if (totalErrors == 0)
    {
        printf(">>> Both the files are same, there is no mismatch!\n") ;
    }
}

int main()
{
    int startingTime = getTimeElapsed() ;

    char *fifo1 = "fifo1";
    mkfifo(fifo1,0666) ;
    char *fifo2 = "fifo2";
    mkfifo(fifo2, 0666) ;

    int process_id=fork() ;
    if(process_id==0){
	printf("Child Process\n");
	FILE* fd = fopen(fifo1, "r") ;
        char *receivedFile = "receivedFile.txt";
        FILE *receivedfp = fopen(receivedFile, "w") ;
        if (receivedfp == NULL)
            printf(">>> Failed to open the file %s for writing\n", receivedFile) ;
        else
            printf(">>> Opened the file %s for writing\n", receivedFile) ;
        if (fd != NULL)
        {
            printf(">>> Reading from FIFO fifo1\n") ;
            char filechar;
            while(fscanf(fd,"%c",&filechar)==1){
                fputc(filechar,receivedfp);
       	    }

            printf(">>> File received from process 1\n") ;
            fclose(fd) ;
        }
        else
        {
            printf(">>> Failed to open FIFO fifo1 for reading\n") ;
        }

        fclose(receivedfp) ;

	receivedfp = fopen(receivedFile, "r") ;
        fd = fopen(fifo2, "w") ;
        if (fd !=NULL)
        {
            printf(">>> Writing to FIFO fifo2 from the file %s\n", receivedFile) ;
	    char filechar=getc(receivedfp);
            while(filechar!=EOF){
                fputc(filechar,fd);
                filechar=getc(receivedfp);
            }

            printf(">>> Writing to fifo2 is completed\n") ;
        }
    }

    else{
	printf("Parent Process\n");
	const char *initialFile = "initial.txt";
        FILE *fp = fopen(initialFile, "r") ;
        if (fp == NULL)
            printf(">>> Failed to open the file %s for reading\n", initialFile) ;
        else
            printf(">>> Opened the file %s for reading\n", initialFile) ;
        FILE* fd = fopen(fifo1, "w") ;
        if (fd !=NULL)
        {
            printf(">>> Writing to FIFO fifo1\n") ;
            char filechar=getc(fp);
            while(filechar!=EOF){
                fputc(filechar,fd);
                filechar=getc(fp);
            }


            printf(">>> Writing to FIFO fifo1 is completed\n") ;
        }
        else
        {
            printf(">>> Failed to open FIFO fifo1 for writing\n") ;
        }
        fclose(fd) ;
        fclose(fp) ;



        fd = fopen(fifo2, "r") ;
        const char *receivedBackFile = "receivedBackFile.txt";
        FILE *receivedbackfp = fopen(receivedBackFile, "w") ;
        if (fd !=NULL)
        {
            printf(">>> Reading from fifo2 and writing to the file %s to compare received back file with the initial file sent by process1\n", receivedBackFile) ;
            char filechar;
            while(fscanf(fd,"%c",&filechar)==1){
                fputc(filechar,receivedbackfp);
            }

        }
        else
        {
            perror("read") ;
        }

        fclose(fd) ;
        fclose(receivedbackfp) ;

        int endingTime = getTimeElapsed() ;
	int totalTimeTaken = endingTime - startingTime;

	fp = fopen(initialFile, "r") ;
        receivedbackfp = fopen(receivedBackFile, "r") ;
        compareFiles(fp, receivedbackfp) ; 

        fclose(fp) ;
        fclose(receivedbackfp) ;
        printf(">>> The round-about time for transferring the file between process1 and process2 and back to process1 is %d milliseconds\n", totalTimeTaken) ;
    }

    return 0;
}











