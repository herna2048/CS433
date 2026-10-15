/*
 * PA2 Part C  --  tcount: can the operating system see your threads?
 *
 * Chapter 4 says a Pthreads thread on Linux and on macOS is one-to-one: every
 * thread you create gets its own kernel-schedulable entity.  This program lets
 * you check that claim from outside the process instead of believing it.
 *
 * The program prints its own pid, waits, creates T threads that do nothing but
 * stay alive, waits again, then exits.  While it waits, you count its threads
 * from another terminal and see the number change.
 *
 * Build:  make
 * Run:    ./tcount --threads 4
 */
#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void *idle(void *arg)
{
    long id = (long)arg;
    sleep(6);
    return (void *)id;
}

int main(int argc, char **argv)
{
    int nthreads = 4;
    for (int i = 1; i < argc; i++)
        if (!strcmp(argv[i], "--threads") && i + 1 < argc) nthreads = atoi(argv[++i]);
    if (nthreads < 1) { fprintf(stderr, "tcount: --threads must be >= 1\n"); return 2; }

    printf("pid %ld\n", (long)getpid());
    printf("Only main is running. You have about 3 seconds to count my threads.\n");
    fflush(stdout);
    sleep(3);

    pthread_t *tid = malloc((size_t)nthreads * sizeof *tid);
    if (!tid) { fprintf(stderr, "tcount: out of memory\n"); return 1; }

    for (long k = 0; k < nthreads; k++) {
        int rc = pthread_create(&tid[k], NULL, idle, (void *)k);
        if (rc != 0) { fprintf(stderr, "tcount: pthread_create: %s\n", strerror(rc)); return 1; }
    }
    printf("%d more threads now exist. You have about 6 seconds to count again.\n", nthreads);
    fflush(stdout);

    for (int k = 0; k < nthreads; k++) pthread_join(tid[k], NULL);
    free(tid);
    printf("All joined. Exiting.\n");
    return 0;
}
