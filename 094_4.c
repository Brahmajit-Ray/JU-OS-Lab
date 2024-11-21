/* 

Name: Brahmajit Ray
9th Septenmber 2024 1:32pm

Running Command:gcc 094_4.c
		./a.out initalFile.txt


Output:

The Value Written is : 2874999   
The Value Read is : 2874999   
The value that is read and written are the same!
---------------------------------------------------------The Value Written is : 6184637   
The Value Read is : 6184637   
The value that is read and written are the same!
---------------------------------------------------------The Value Written is : 8132556   
The Value Read is : 8132556   
The value that is read and written are the same!
---------------------------------------------------------The Value Written is : 340379    
The Value Read is : 340379    
The value that is read and written are the same!
---------------------------------------------------------The Value Written is : 8193634   
The Value Read is : 8193634   
The value that is read and written are the same!
---------------------------------------------------------The Value Written is : 4125987   
The Value Read is : 4125987   
The value that is read and written are the same!
---------------------------------------------------------The Value Written is : 2678174   
The Value Read is : 2678174   
The value that is read and written are the same!



Command:sar -B 1 1000

Output:

Linux 3.10.0-514.21.1.el7.x86_64 (localhost.localdomain)        Monday 09 September 2024        _x86_64_        (16 CPU)

02:44:22  IST  pgpgin/s pgpgout/s   fault/s  majflt/s  pgfree/s pgscank/s pgscand/s pgsteal/s    %vmeff
02:44:23  IST      0.00     16.00    539.00      0.00    214.00      0.00      0.00      0.00      0.00
02:44:24  IST      0.00      0.00    539.00      0.00    213.00      0.00      0.00      0.00      0.00
02:44:25  IST      0.00      0.00    529.00      0.00    226.00      0.00      0.00      0.00      0.00
02:44:26  IST      0.00    406.00    529.00      0.00    219.00      0.00      0.00      0.00      0.00
02:44:27  IST      0.00      0.00    558.00      0.00    342.00      0.00      0.00      0.00      0.00
02:44:28  IST      0.00     20.00    528.00      0.00    216.00      0.00      0.00      0.00      0.00
02:44:29  IST      0.00      0.00    529.00      0.00    218.00      0.00      0.00      0.00      0.00
02:44:30  IST      0.00      0.00    529.00      0.00    217.00      0.00      0.00      0.00      0.00
02:44:31  IST      0.00      0.00    544.00      0.00    230.00      0.00      0.00      0.00      0.00
02:44:32  IST      0.00      0.00    528.00      0.00    239.00      0.00      0.00      0.00      0.00
02:44:33  IST      0.00      4.00    528.00      0.00    213.00      0.00      0.00      0.00      0.00
^C

02:44:33  IST      0.00      0.00    577.17      0.00    233.70      0.00      0.00      0.00      0.00
Average:         0.00     37.42    537.84      0.00    231.71      0.00      0.00      0.00      0.00

*/


#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define lower 0
#define upper 8388608

char* addressOfMap;

void printLine(){
	printf("---------------------------------------------------------");
}

int randomIntegerGenerator(){
	return rand()%(upper-1);
}

int readInteger(unsigned long offset)
{
    unsigned long readValue;
    sscanf(addressOfMap + offset, "%lu", &readValue);
    printf("The Value Read is : %-10lu\n", readValue);
    return readValue;
}

int writeInteger(unsigned long offset, unsigned long value)
{
    printf("The Value Written is : %-10lu\n", value);
    sprintf(addressOfMap + offset, "%lu", value);
}


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Sufficient number of arguments are not provided!\n");
        printf("The expected execution command is : ./a.out <file_name>\n");
        exit(1);
    }

    int FILE = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
    struct stat sb;

    if (fstat(FILE, &sb) == -1)
    {
        perror("Some ERROR occured while getting the file size!");
        exit(1);
    }
 
    if ((addressOfMap = (char *)mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, FILE, 0)) == MAP_FAILED)
    {
        perror("Some ERROR occured while mapping the file!");
        exit(2);
    }

    srand(time(NULL));
    unsigned long offset, writtenValue, readValue;

    while (1)
    {
        writtenValue = randomIntegerGenerator();
	offset = randomIntegerGenerator();
        writeInteger(offset, writtenValue);
	readValue = readInteger(offset);

	if (writtenValue == readValue)
        {
            printf("The value that is read and written are the same!\n");
            printLine();
        }
        else
        {
            printf("The value that is read and written are NOT the same!\n");
            printLine();
        }

	sleep(1);
    }

    return 0;
}











