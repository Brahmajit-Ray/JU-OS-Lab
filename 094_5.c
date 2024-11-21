/*
Brahmajit Ray
Roll-002211001094

Question:
The objective of this assignment is to create a deadlock. For this purpose define two global variables (Total_1 and Total_2) and initialize both of them to 1000. You should also have two mutexes to protect these two global variables. You need to create two threads also.
The function of each of the threads is to generate a random quantity (not more than 50) and subtract that quantity from one of the Total and add that quantity to the other Total. While manipulating the Totals, each Thread should lock both the mutex and then unlock it after changing the Totals.
The order of locking and unlocking the Mutex is very important, because that’s what creates a Deadlock. Once you correctly identify this order, you should upload the program. Also, include that information (as comment in your source file) how to avoid this kind of Deadlock.
If these two Threads, Two Totals and Two Mutex are not good enough for Deadlock;
Then you need to create one more Thread, one more Total and and one more Mutex; and carry on the same experiment.
Make sure there are enough printf in your program so that it can be clearly understood that there is a Deadlock. One way to ensure enough printf is to display the total of Total_1 and Total_2 all the time after every operation on it.

Output:

Thread 1 trying to lock mutex1
Thread 1 locked mutex1
Thread 2 trying to lock mutex2
Thread 2 locked mutex2
Thread 1 trying to lock mutex2
Thread 2 trying to lock mutex1

^C



*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_QUANTITY 50

// Global variables
int Total_1 = 1000;
int Total_2 = 1000;

// Mutexes to protect each total
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

// Function for Thread 1
void *threadFunc1(void *arg) {
    while (1) {
        int quantity = rand() % MAX_QUANTITY + 1;
        printf("Thread 1 trying to lock mutex1\n");

        // Lock mutex1 then mutex2 (lock order 1)
        pthread_mutex_lock(&mutex1);
        printf("Thread 1 locked mutex1\n");

        sleep(1); // Intentional delay to increase chances of deadlock

        printf("Thread 1 trying to lock mutex2\n");
        pthread_mutex_lock(&mutex2);
        printf("Thread 1 locked mutex2\n");

        // Update totals
        Total_1 -= quantity;
        Total_2 += quantity;
        printf("Thread 1: Total_1 = %d, Total_2 = %d, Combined Total = %d\n", Total_1, Total_2, Total_1 + Total_2);

        // Unlock mutexes
        pthread_mutex_unlock(&mutex2);
        printf("Thread 1 unlocked mutex2\n");

        pthread_mutex_unlock(&mutex1);
        printf("Thread 1 unlocked mutex1\n");

        sleep(1); // Delay to allow context switch
    }
    return NULL;
}

// Function for Thread 2
void *threadFunc2(void *arg) {
    while (1) {
        int quantity = rand() % MAX_QUANTITY + 1;
        printf("Thread 2 trying to lock mutex2\n");

        // Lock mutex2 then mutex1 (lock order 2, opposite of threadFunc1)
        pthread_mutex_lock(&mutex2);
        printf("Thread 2 locked mutex2\n");

        sleep(1); // Intentional delay to increase chances of deadlock

        printf("Thread 2 trying to lock mutex1\n");
        pthread_mutex_lock(&mutex1);
        printf("Thread 2 locked mutex1\n");

        // Update totals
        Total_2 -= quantity;
        Total_1 += quantity;
        printf("Thread 2: Total_1 = %d, Total_2 = %d, Combined Total = %d\n", Total_1, Total_2, Total_1 + Total_2);

        // Unlock mutexes
        pthread_mutex_unlock(&mutex1);
        printf("Thread 2 unlocked mutex1\n");

        pthread_mutex_unlock(&mutex2);
        printf("Thread 2 unlocked mutex2\n");

        sleep(1); // Delay to allow context switch
    }
    return NULL;
}

/*
   Explanation:
   This program demonstrates a deadlock by creating two threads, each of which tries to lock two mutexes
   (mutex1 and mutex2) in a different order. Thread 1 locks mutex1 first, then mutex2, while Thread 2
   locks mutex2 first, then mutex1. This order discrepancy can lead to a deadlock if both threads lock
   one mutex and then try to lock the other.

   How to Avoid Deadlock:
   To prevent this type of deadlock, we should enforce a consistent locking order across all threads.
   For instance, both threads should always lock mutex1 first, then mutex2. This way, a circular wait
   cannot occur, and the deadlock can be avoided.
*/

int main() {
    srand(time(NULL));

    // Create threads
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, threadFunc1, NULL);
    pthread_create(&thread2, NULL, threadFunc2, NULL);

    // Join threads (this program is meant to deadlock, so it will hang here)
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
