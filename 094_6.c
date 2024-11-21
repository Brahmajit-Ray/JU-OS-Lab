/*
The OS is a program that uses various data structures. Like all programs in execution, you can determine the performance and other behaviour of the OS by 
inspecting its state - the values stored in its data structures. In this part of the assignment, we study some aspects of the organization and behaviour of a Linux system by observing values of kernel data structures exposed through the /proc virtual file system.


The /proc virtual file system:

Linux uses the /proc file system to collect information from kernel data structures. The /proc implementation provided with Linux can read many different kernel data structures. If you cd to /proc on a Linux machine, you will see a number of files and directories at that location. Files in this directory subtree each correspond to some kernel data structure. The subdirectories with numeric names contain virtual files with information about the process whose process ID is the same as the directory name. Files in /proc can be read like ordinary ASCII files. You can open each file and read it using library routines such as fgets() or fscanf(). The proc (5) manual page explains the virtual files and their content available through the /proc file system.


Requirements in detail:

You are supposed to write a C program which should print the following values on terminal

?	Processor Informations (Vendor ID, Model Name and Cache Size in MB)
?	Kernel Name, Kernel Release and Operating System Name.
?	The amount of memory configured into this computer and Free Memory
?	Amount of time since the system was last booted (In Hours, Minutes, Seconds)

Date:21 Oct 12:30pm

Run:gcc 094_6.c / ./a.out

Output:

Displaying ... 

Time spent in user mode : 0.090475 %, idle mode : 99.779658 % , system mode : 0.105975 % 

Available Memory : 64032608 kb
The Percentage Available Memory : 97.686169

Average Read Rate of the disk (sectors per second) : 0.000000
Average Write Rate of the disk (sectors per second) : 0.000000
Rate of Context Switches (number per sec) : 691.000000
Rate of Process Creations (number per sec) : 0.000000

Kernel Information: 
Kernel Name: CPU0
Kernel Release: CPU2

Time since PC was switched on: 7652 seconds

Operating System: Linux

*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_BUFFER_LENGTH 10000

void printLine()
{
    printf("------------------------------------------------------------------------------------------\n\n");
}

double toDouble(const char *str) // Method to convert string to double
{

    int n = strlen(str);
    int i;
    double ans = 0;
    for (i = 0; i < n; ++i)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            ans = ans * 10 + (str[i] - '0');
        }
    }
    return ans;
}

int toInt(const char *str) // Method to convert string to integer
{
    int n = strlen(str);
    int i, ans = 0;
    for (i = 0; i < n; ++i)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            ans = ans * 10 + (str[i] - '0');
        }
    }
    return ans;
}

void kernelInfo()
{
    FILE *fp;
    fp = fopen("/proc/interrupts", "rb");
    char *kernelName = (char *)malloc(1024);
    char *release = (char *)malloc(1024);
    char *temp = (char *)malloc(1024);
    fscanf(fp, "%s %s %s", kernelName, temp, release);
    printf("\nKernel Information: \n");
    printf("Kernel Name: %s\n", kernelName);
    printf("Kernel Release: %s\n", release);
}

void uptime(){
    FILE *file;
    double uptime;
    // Open /proc/uptime to read system uptime
    file = fopen("/proc/uptime", "r");
    if (file == NULL) {
        perror("fopen");
       // return EXIT_FAILURE;
    }
    // Read the uptime value
    fscanf(file, "%lf", &uptime);
    fclose(file);
    // Print uptime in seconds
    printf("Time since PC was switched on: %.0f seconds\n", uptime);
    //return 0;
}

void os_name(){
	#ifdef _WIN32
    printf("Operating System: Windows\n");
#elif __linux__
    printf("Operating System: Linux\n");
#elif __APPLE__
    printf("Operating System: macOS\n");
#elif __FreeBSD__
    printf("Operating System: FreeBSD\n");
#else
    printf("Operating System: Unknown\n");
#endif
}

int main(int argc, char const *argv[])
{
    char stream[MAX_BUFFER_LENGTH];

    double total = 0, idle, user, temp, system, mem_total, ctxt_prev, process_prev, sector_write_prev, sector_read_prev;
    double user_total = 0, idle_total = 0, system_total = 0, disk_read = 0, disk_write = 0;
    unsigned long long int available_mem, available_mem_total = 0;
    double ctxt_rate = 0, process_rate = 0;
    int observations = 0, disks = 0;
    int read_time = 1, print_time = 1;

    // Reading /proc/stat to save the initial values for the context switches and processess running in the system
    FILE *kernel_st_file = fopen("/proc/stat", "r");
    if (kernel_st_file == NULL)
    {
        perror("Error in opening file to read kernel/system statistics\n");
        exit(EXIT_FAILURE);
    }

    // Taking the initial values
    while (fgets(stream, MAX_BUFFER_LENGTH, kernel_st_file))
    {
        char *tk = NULL;
        tk = strtok(stream, " ");
        int i = 1, flag = 0;

        while (tk != NULL)
        {

            if (strcmp(tk, "ctxt") == 0)
            {
                flag = 1;
            }
            if (strcmp(tk, "processes") == 0)
            {
                flag = 2;
            }
            if (i == 2 && flag == 1)
            {
                flag = 0;
                ctxt_prev = toDouble(tk);
            }
            if (i == 2 && flag == 2)
            {
                flag = 0;
                process_prev = toDouble(tk);
            }

            i++;
            tk = strtok(NULL, " ");
        }
        free(tk);
    }
    fclose(kernel_st_file);

    FILE *diskstat_file = fopen("/proc/diskstats", "r");
    if (diskstat_file == NULL)
    {
        perror("Error in opening the file to read the disk statistics \n");
        exit(EXIT_FAILURE);
    }

    while (fgets(stream, MAX_BUFFER_LENGTH, diskstat_file))
    {

        char *token = NULL;
        int i = 1;
        int flag = 0;

        token = strtok(stream, " ");

        while (token != NULL)
        {
            // Calculating only for sda
            if (strcmp(token, "sda") == 0)
            {
                flag = 1;
            }
            // If the data is related to sda , then only consider it
            if (flag == 1)
            {
                // Record the number of sectors read sucessfully
                if (i == 6)
                {
                    sector_read_prev = toDouble(token);
                }
                // Record the number of sectors to which write was successfull
                if (i == 10)
                {
                    sector_write_prev = toDouble(token);
                }
            }
            token = strtok(NULL, " ");
            i++;
        }
        free(token);
    }
    fclose(diskstat_file);

    // Starting the read and write clocks for maintaining the time bound for reading files and printing the statistics
    clock_t initial_read = clock();
    clock_t initial_write = clock();

    while (1)
    {

        // Taking the reading of the current time
        clock_t final_time = clock();

        // Calculating the amount of time paassed since the last read and write operations
        int diff_read = (int)(final_time - initial_read) / CLOCKS_PER_SEC;
        int diff_print = (int)(final_time - initial_write) / CLOCKS_PER_SEC;

        // If the time elapsed is equal to the read_rate provided then record the data
        if (diff_read == read_time)
        {

            // Declaring and initializing variables
            total = 0;
            temp = 0;
            int i = 1;
            observations++;
            double read_rate = 0, write_rate = 0;
            double ctxt_new, process_new;

            // Processor time in different modes found in the /proc/stat file
            FILE *stat_file = fopen("/proc/stat", "r");
            if (stat_file == NULL)
            {
                perror("Error in opening file for reading processor time statistics \n");
                exit(EXIT_FAILURE);
            }

            fscanf(stat_file, "%s", stream);
            while (i <= 10)
            {

                fscanf(stat_file, "%lf", &temp);
                total += temp;

                // Record user mode data
                if (i == 1)
                {
                    user = temp;
                }
                // Record system mode data
                else if (i == 3)
                {
                    system = temp;
                }
                // Record idle mode data
                else if (i == 4)
                {
                    idle = temp;
                }

                i++;
            }

            idle = (idle * 100.0) / total;
            user = (user * 100.0) / total;
            system = (system * 100.0) / total;
            idle_total += idle;
            user_total += user;
            system_total += system;

            fclose(stat_file);

            // Amount of memory available
            FILE *mem_file = fopen("/proc/meminfo", "r");
            if (mem_file == NULL)
            {
                perror(" Error in opening file for reading memory details \n");
                exit(EXIT_FAILURE);
            }
            // Reading the total memory
            fscanf(mem_file, "%s", stream);
            fscanf(mem_file, "%lf", &mem_total);
            fscanf(mem_file, "%s\n", stream);
            fgets(stream, MAX_BUFFER_LENGTH, mem_file);
            // Reading the free(or available memory )
            fscanf(mem_file, "%s", stream);
            fscanf(mem_file, "%lld", &available_mem);
            fscanf(mem_file, "%s", stream);
            available_mem_total += available_mem;

            fclose(mem_file);

            // Disk read write
            FILE *disk_file = fopen("/proc/diskstats", "r");
            if (disk_file == NULL)
            {
                perror("Error in opening the file to read the disk statistics \n");
                exit(EXIT_FAILURE);
            }
            double sector_read, sector_write;
            while (fgets(stream, MAX_BUFFER_LENGTH, disk_file))
            {

                char *token = NULL;
                int i = 1;
                int flag = 0;

                token = strtok(stream, " ");

                while (token != NULL)
                {
                    // Calculating only for sda
                    if (strcmp(token, "sda") == 0)
                    {
                        flag = 1;
                    }
                    // If the data is related to sda , then only consider it
                    if (flag == 1)
                    {
                        // Record the number of sectors read sucessfully
                        if (i == 6)
                        {
                            sector_read = toDouble(token);
                        }
                        // Record the number of sectors to which write was successfull
                        if (i == 10)
                        {
                            sector_write = toDouble(token);
                        }
                    }
                    token = strtok(NULL, " ");
                    i++;
                }

                free(token);
            }
            read_rate = (sector_read - sector_read_prev) / read_time;
            write_rate = (sector_write - sector_write_prev) / read_time;

            sector_write_prev = sector_write;
            sector_read_prev = sector_read;

            disk_read += read_rate;
            disk_write += write_rate;

            fclose(disk_file);

            // Reading from /proc/stat file for recording the context switch rate and the process creation rate
            FILE *kernel_stat_file = fopen("/proc/stat", "r");
            if (kernel_stat_file == NULL)
            {
                perror("Error in opening file to read kernel/system statistics\n");
                exit(EXIT_FAILURE);
            }

            while (fgets(stream, MAX_BUFFER_LENGTH, kernel_stat_file))
            {

                char *tk = NULL;
                tk = strtok(stream, " ");
                int i = 1, flag = 0;

                while (tk != NULL)
                {
                    if (strcmp(tk, "ctxt") == 0)
                    {
                        flag = 1;
                    }
                    if (strcmp(tk, "processes") == 0)
                    {
                        flag = 2;
                    }
                    // Record the context switches happened till now
                    if (i == 2 && flag == 1)
                    {
                        flag = 0;
                        ctxt_new = toDouble(tk);
                    }
                    // Record the number of processess creayed till now
                    if (i == 2 && flag == 2)
                    {
                        flag = 0;
                        process_new = toDouble(tk);
                    }
                    i++;
                    tk = strtok(NULL, " ");
                }
                free(tk);
            }
            // Calculate the rates and add it to the global variable for doing the average later on
            ctxt_rate += ((ctxt_new - ctxt_prev) / read_time);
            process_rate += ((process_new - process_prev) / read_time);

            // Reinitialize the initial context switches and processess in the system
            ctxt_prev = ctxt_new;
            process_prev = process_new;

            fclose(kernel_stat_file);

            // Reset the timer for reading
            initial_read = clock();
        }
        // If the time elapsed is equal to the print_rate provided then print the data
        if (diff_print == print_time)
        {

            // Average out all the observations

            idle_total /= observations;
            user_total /= observations;
            system_total /= observations;

            available_mem_total /= observations;
            double percentage_mem_available = (available_mem_total * 100.0) / mem_total;

            disk_write /= observations;
            disk_read /= observations;

            ctxt_rate /= observations;
            process_rate /= (double)observations;

            // Printing all the observations
            printf("Displaying ... \n\n");
            printf("Time spent in user mode : %lf %%, idle mode : %lf %% , system mode : %lf %% \n\n", user_total, idle_total, system_total);
            printf("Available Memory : %lld kb\n", available_mem_total);
            printf("The Percentage Available Memory : %lf\n\n", percentage_mem_available);
            printf("Average Read Rate of the disk (sectors per second) : %lf\n", disk_read);
            printf("Average Write Rate of the disk (sectors per second) : %lf\n", disk_write);
            printf("Rate of Context Switches (number per sec) : %lf\n", ctxt_rate);
            printf("Rate of Process Creations (number per sec) : %lf\n", process_rate);
            kernelInfo();
            printf("\n");
	    uptime();
	    printf("\n");
	    os_name();
	    printLine();
            // Reseting all the Global Variables
            observations = 0;
            idle_total = 0;
            system_total = 0;

            user_total = 0;
            available_mem_total = 0;

            disk_read = 0;
            disk_write = 0;

            ctxt_rate = 0;
            process_rate = 0;

            // Reseting the timer
            initial_write = clock();
        }
    }
    return 0;
}








