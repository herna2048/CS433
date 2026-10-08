/*
 * BUG 1.  Each thread is supposed to print its own index, 0 through 3.
 * Run it a few times.  The indices come out wrong, repeated, or out of range.
 *
 * Fix it so every run prints 0, 1, 2 and 3 exactly once each, in some order.
 * Write one sentence in bugs/ANSWERS.md naming the actual defect.
 */
#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>

#define N 4

static void *show(void *arg)
{
    int id = *(int *)arg;
    printf("thread index %d\n", id);
    return NULL;
}

int main(void)
{
    pthread_t t[N];
    for (int i = 0; i < N; i++)
        pthread_create(&t[i], NULL, show, &i);
    for (int i = 0; i < N; i++)
        pthread_join(t[i], NULL);
    return 0;
}
