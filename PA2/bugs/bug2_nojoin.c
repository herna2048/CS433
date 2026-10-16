/*
 * BUG 2.  Four threads each add 1000 to their own slot.  main then reports the
 * total, which should be 4000.  It usually reports less, and the number changes
 * between runs.
 *
 * Fix it so the total is 4000 on every run.
 * Write one sentence in bugs/ANSWERS.md naming the actual defect.
 */
#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>

// defines the number of threads (4)
#define N 4

// defines an array of longs to hold the slots
static long slot[N];

// k = the thread's index
// it adds 1 to its own slot 1000 times so each thread will add 1000 to its own slot
static void *add_mine(void *arg)
{
    long k = (long)arg;
    for (int i = 0; i < 1000; i++) slot[k]++;
    return NULL;
}

int main(void)
{
    // start 4 threads
    pthread_t t[N];
    for (long k = 0; k < N; k++)
        pthread_create(&t[k], NULL, add_mine, (void *)k);

    for (int k = 0; k < N; k++) pthread_join(t[k], NULL); // wait for all threads to finish, similar to the bug 1 code near the end
    
    // add up the 4 boxes and print the total (should be 4 x 1000 = 4000)
    long total = 0;
    for (int k = 0; k < N; k++) total += slot[k];
    printf("total %ld (expected 4000)\n", total);
    return 0;
}
