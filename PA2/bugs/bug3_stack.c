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

#define N 4

static void *compute(void *arg)
{
    long k = (long)arg;
    long answer = (k + 1) * 100;
    return &answer;
}

int main(void)
{
    pthread_t t[N];
    for (long k = 0; k < N; k++)
        pthread_create(&t[k], NULL, compute, (void *)k);

    for (int k = 0; k < N; k++) {
        void *ret;
        pthread_join(t[k], &ret);
        printf("thread %d returned %ld\n", k + 1, *(long *)ret);
    }
    return 0;
}
