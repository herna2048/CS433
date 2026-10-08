/*
 * BUG 4.  The error checking below is written the way you would check a system
 * call like open, fork or read: compare against -1 and read errno.  Pthreads
 * does not use that convention, so the check can never fire.
 *
 * To let you watch your own error path run, thread creation goes through
 * create_worker() instead of calling pthread_create directly.  When
 * CS433_FAIL_CREATE is set in the environment, create_worker reports a failure
 * the way pthread_create reports one.  Nothing here relies on undefined
 * behaviour, and you should not change create_worker.
 *
 * Run it BOTH ways:
 *
 *     ./bugs/bug4_errno                      the worker runs, you get 42
 *     CS433_FAIL_CREATE=1 ./bugs/bug4_errno  creation reports a failure
 *
 * With the variable set, the program still announces success and then prints a
 * result it never received.  Fix the error checking so the failure is detected
 * and reported, and so main does not use a result that was never delivered.
 *
 * Read the RETURN VALUE section of  man 3 pthread_create  first.
 *
 * Write one sentence in bugs/ANSWERS.md naming the actual defect.
 */
#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static long answer = 42;

static void *worker(void *arg) { (void)arg; return &answer; }
static void *idle(void *arg)   { (void)arg; return NULL; }

/* Do not change this function.  It exists so that the failure path is
 * observable and portable: it still starts a real joinable thread, so the
 * rest of the program stays well defined, but it returns an error number
 * exactly as pthread_create would. */
static int create_worker(pthread_t *t, void *(*fn)(void *), void *arg)
{
    if (getenv("CS433_FAIL_CREATE") != NULL) {
        int rc = pthread_create(t, NULL, idle, NULL);
        return (rc == 0) ? EAGAIN : rc;
    }
    return pthread_create(t, NULL, fn, arg);
}

int main(void)
{
    pthread_t t;
    void *ret = NULL;
    long result = -1;                  /* -1 means: nothing was received */

    if (create_worker(&t, worker, NULL) == -1) {
        fprintf(stderr, "bug4: create_worker failed: %s\n", strerror(errno));
        return 1;
    }
    printf("thread created with no error\n");

    if (pthread_join(t, &ret) == -1) {
        fprintf(stderr, "bug4: pthread_join failed: %s\n", strerror(errno));
        return 1;
    }
    if (ret != NULL) result = *(long *)ret;

    printf("the worker returned %ld\n", result);
    return 0;
}
