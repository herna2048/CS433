/*
 * BUG 3.  Each thread computes a value and hands it back through
 * pthread_join.  The numbers main prints are garbage, or the program crashes,
 * or it prints the right thing on your laptop and the wrong thing on the
 * course server.
 *
 * Fix it so main reliably prints 100, 200, 300, 400 for threads 1 through 4.
 * Write one sentence in bugs/ANSWERS.md naming the actual defect.
 */
#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4

static void *compute(void *arg)
{
    // get the callers memory and send the value into the long
    long k = (long)arg;
    // dynamically allocate memory for one long
    long *answer = malloc(sizeof *answer);
    // do the calculation from k's value sent at (void *)k
    *answer = (k + 1) * 100;
    // return the answer so main has a valid pointer pointing to a long in memory, and you get memory (k + 1) * 100
    return answer;
}

int main(void)
{
    // creates a new thread array
    pthread_t t[N];

    // create the 4 threads, pass in the thread array at that specific instant, Null, the function, and you pass in the address at k's value + 1 * 100
    for (long k = 0; k < N; k++)
        pthread_create(&t[k], NULL, compute, (void *)k);

    // for each thread, join them with the calculation done so each thread is stored with each of k's value + 1 * 100
    for (int k = 0; k < N; k++) {
        void *ret;
        pthread_join(t[k], &ret);
        printf("thread %d returned %ld\n", k + 1, *(long *)ret);
        // need to avoid memory leaks
        free(ret);
    }

    return 0;
}
