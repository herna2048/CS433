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

#define N 4

static long slot[N];

static void *add_mine(void *arg)
{
    long k = (long)arg;
    for (int i = 0; i < 1000; i++) slot[k]++;
    return NULL;
}

int main(void)
{
    pthread_t t[N];
    for (long k = 0; k < N; k++)
        pthread_create(&t[k], NULL, add_mine, (void *)k);

    long total = 0;
    for (int k = 0; k < N; k++) total += slot[k];
    printf("total %ld (expected 4000)\n", total);
    return 0;
}
