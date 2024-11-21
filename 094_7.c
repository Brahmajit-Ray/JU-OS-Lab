/*
The objective of this assignment is to create a deadlock. For this purpose define two global variables (Total_1 and Total_2) and initialize both of them to 1000. You should also have two mutexes to protect these two global variables. You need to create two threads also.

 

The function of each of the threads is to generate a random quantity (not more than 50) and subtract that quantity from one of the Total and add that quantity to the other Total. While manipulating the Totals, each Thread should lock both the mutex and then unlock it after changing the Totals.

 

The order of locking and unlocking the Mutex is very important, because that’s what creates a Deadlock. Once you correctly identify this order, you should upload the program. Also, include that information (as comment in your source file) how to avoid this kind of Deadlock.

 

If these two Threads, Two Totals and Two Mutex are not good enough for Deadlock;

Then you need to create one more Thread, one more Total and and one more Mutex; and carry on the same experiment.

 

Make sure there are enough printf in your program so that it can be clearly understood that there is a Deadlock. One way to ensure enough printf is to display the total of Total_1 and Total_2 all the time after every operation on it.


Output:

[be2294@localhost Assignment7]$ ./a.out 3

Results:
Pair 1 (X=0, Y=1): A=0, B=0, C=1, D=0, E=-1
Pair 2 (X=8, Y=2): A=16, B=8, C=10, D=1, E=60
Pair 3 (X=6, Y=9): A=54, B=27, C=15, D=3, E=-45

Run the command 'ipcs -m' to display shared memory statistics.
[be2294@localhost Assignment7]$ ipcs -m

------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status      
0x00000000 65536      be21140    666        2008       0                       
0x00000000 98305      be21140    666        2008       0                       
0x00000000 131074     be21140    666        2008       0                       
0x00000000 163843     be21140    666        2008       0                       
0x00000000 196612     be21140    666        2008       0                       
0x00000000 229381     be22105    666        192        0                       
0x00000000 262150     be22105    666        48         0                       
0x00000000 294919     be22105    666        144        0                       
0x00000000 327688     be22105    666        144        0                       
0x00000000 360457     be22105    666        192        0                       
0x00000000 1081354    be2267     666        2840       0                       
0x00000000 1277963    be22131    666        56         0   
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_PAIRS 10

// Structure for computation data
typedef struct {
    int X, Y, A, B, C, D, E;
} computation_data;

computation_data *shared_memory;
pthread_barrier_t barrier;
int total_pairs = 0;


void *compute_A(void *arg) {
    int i;
    for (i = 0; i < total_pairs; i++) {
        pthread_barrier_wait(&barrier);
        shared_memory[i].A = shared_memory[i].X * shared_memory[i].Y;
        pthread_barrier_wait(&barrier);
    }
    return NULL;
}

void *compute_BD(void *arg) {
    int i;
    for (i = 0; i < total_pairs; i++) {
        pthread_barrier_wait(&barrier);
        shared_memory[i].B = (shared_memory[i].X * shared_memory[i].Y) / 2;
        if (shared_memory[i].X + shared_memory[i].Y != 0)
            shared_memory[i].D = (shared_memory[i].X * shared_memory[i].Y) / (shared_memory[i].X + shared_memory[i].Y);
        pthread_barrier_wait(&barrier);
    }
    return NULL;
}

void *compute_CE(void *arg) {
    int i;
    for (i = 0; i < total_pairs; i++) {
        pthread_barrier_wait(&barrier);
        shared_memory[i].C = shared_memory[i].X + shared_memory[i].Y;
        shared_memory[i].E = (shared_memory[i].X + shared_memory[i].Y) * (shared_memory[i].X - shared_memory[i].Y);
        pthread_barrier_wait(&barrier);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int i;

    if (argc != 2) {
        printf("Usage: %s <number_of_pairs>\n", argv[0]);
        return -1;
    }
    total_pairs = atoi(argv[1]);
    if (total_pairs > MAX_PAIRS) {
        printf("Maximum number of pairs is %d\n", MAX_PAIRS);
        return -1;
    }

    // Initialize shared memory for computation data
    int shm_fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(computation_data) * MAX_PAIRS);
    shared_memory = mmap(0, sizeof(computation_data) * MAX_PAIRS, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Initialize barrier for synchronizing threads
    pthread_barrier_init(&barrier, NULL, 3);

    // Seed random number generator and populate (X, Y) pairs
    srand(time(NULL));
    for (i = 0; i < total_pairs; i++) {
        shared_memory[i].X = rand() % 10;
        shared_memory[i].Y = rand() % 10;
    }

    // Create threads
    pthread_t th1, th2, th3;
    pthread_create(&th1, NULL, compute_A, NULL);
    pthread_create(&th2, NULL, compute_BD, NULL);
    pthread_create(&th3, NULL, compute_CE, NULL);

    // Wait for threads to finish
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    // Display results
    printf("\nResults:\n");
    for (i = 0; i < total_pairs; i++) {
        printf("Pair %d (X=%d, Y=%d): A=%d, B=%d, C=%d, D=%d, E=%d\n",
                i + 1, shared_memory[i].X, shared_memory[i].Y,
                shared_memory[i].A, shared_memory[i].B, shared_memory[i].C,
                shared_memory[i].D, shared_memory[i].E);
    }

    // Cleanup
    pthread_barrier_destroy(&barrier);
    munmap(shared_memory, sizeof(computation_data) * MAX_PAIRS);
    shm_unlink("/shared_memory");

    // Display shared memory info (run `ipcs -m` in terminal)
    printf("\nRun the command 'ipcs -m' to display shared memory statistics.\n");
    return 0;
}
