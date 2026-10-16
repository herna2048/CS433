#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// A function that returns the current time in seconds
// Asks the OS clock for current time
static double now_sec(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + 1e-9 * (double)ts.tv_nsec;
}

// A function that is for the math
// take v which is an element of the array
// square root of absolute value, trig term, log term, arctangent term then return a
// Math is so that the threads can have enough computations to do
static double work(double v)
{
    double a = sqrt(fabs(v));
    a += sin(a) * cos(v);
    a += log1p(fabs(v)) * 0.5;
    a += atan(a);
    return a;
}


// provides information to each thread:
// x: where the numbers are (start of array)
// lo: where to start (indivual to each thread)
// hi: where to stop (individual to each thread)
// partial: blank spot to write its anwser
typedef struct {
    // 32 byte struct
    const double *x; // 8 bytes
    long   lo; // 8 bytes
    long   hi; // 8 bytes
    double partial; // 8 bytes
} targ_t;

static void *sum_range(void *arg)
{
    // ===== A1: sum one thread's section of the array =====
    // turns the generic input to the struct so you can read its fields
    targ_t *a = (targ_t *)arg;

    // creates the running total
    double sum = 0.0;

    // goes through this individual threads section of the array
    for (long i = a->lo; i < a->hi; i++) {
        
        // runs the math of each each number then adds it to the running total
        sum += work(a->x[i]);
    }

    // saves total in struct for later use
    a->partial = sum;
    
    // ends thread
    return NULL;
}

static int run_once(double *x, long n, int nthreads,
                    double *fill_out, double *sum_out, double *total_out)
{
    double t0 = now_sec();
    for (long i = 0; i < n; i++) x[i] = sin((double)i);
    *fill_out = now_sec() - t0;

    double t1 = now_sec();

    // ===== A2: allocate thread handles and structs =====
    // reserves memory for nthreads (malloc request memory, how much threads times bytes per handle (8))
    pthread_t *tid = malloc(nthreads * sizeof *tid);

    // reserves memory for nthreads structs, so each thread can have its own struct
    targ_t *args = malloc(nthreads * sizeof *args);

    // checks if the memory was allocated
    if (!tid || !args) {
        free(tid);
        free(args);
        fprintf(stderr, "tsum: out of memory\n");
        return 1;
    }
    

    // ===== A3: create the threads =====
    // creates one thread per section of the array
    for (int k = 0; k < nthreads; k++) {

        // gives thread k the array and its own range (multiply before divide so no remainder is lost)
        args[k].x  = x;
        args[k].lo = (long)k * n / nthreads;
        args[k].hi = (long)(k + 1) * n / nthreads;

        // starts the thread on sum_range with its own struct, rc is 0 on success or an error number
        int rc = pthread_create(&tid[k], NULL, sum_range, &args[k]);

        // if creation failed, wait for the k threads already started, then clean up and quit
        if (rc != 0) {
            fprintf(stderr, "tsum: pthread_create for thread %d: %s\n", k, strerror(rc));
            for (int j = 0; j < k; j++) pthread_join(tid[j], NULL);
            free(tid);
            free(args);
            return 1;
        }
    }

    // ===== A4: join, combine, free =====
    // waits for every thread to finish so every partial is ready to read
    int failed = 0;
    for (int k = 0; k < nthreads; k++) {
        int rc = pthread_join(tid[k], NULL);

        // remembers a join failure but keeps joining the rest
        if (rc != 0) {
            fprintf(stderr, "tsum: pthread_join for thread %d: %s\n", k, strerror(rc));
            failed = 1;
        }
    }

    // releases the thread handles, no longer needed after joining
    free(tid);

    // a failed join means some partials may not be valid, so quit
    if (failed == 0) {
        free(args);
        return 1;
    }

    // adds the partial sums together in thread order (0, 1, 2, ...) so the result is reproducible
    double total = 0.0;
    for (int k = 0; k < nthreads; k++) total += args[k].partial;

    // releases the structs, no longer needed after the total is computed
    free(args);


    *sum_out   = now_sec() - t1;
    *total_out = total;
    return 0;
}

int main(int argc, char **argv)
{
    long n = 20000000; // size of array
    int nthreads = 1, reps = 3; // number of threads and number of repetitions


    // reads command line arguments
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--threads") && i + 1 < argc)    nthreads = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--n") && i + 1 < argc)     n        = atol(argv[++i]);
        else if (!strcmp(argv[i], "--reps") && i + 1 < argc)  reps     = atoi(argv[++i]);
        else { fprintf(stderr, "usage: %s [--threads T] [--n N] [--reps R]\n", argv[0]); return 2; }
    }

    // rejects invalid values of threads, n, and reps
    if (nthreads < 1 || n < 1 || reps < 1) {
        fprintf(stderr, "tsum: --threads, --n and --reps must all be >= 1\n");
        return 2;
    }

    // allocates memory for the array, returns error if not enough memory
    double *x = malloc((size_t)n * sizeof *x);
    if (!x) { fprintf(stderr, "tsum: cannot allocate %ld doubles\n", n); return 1; }

    // initializes variables for the best wall, fill, sum, and total
    double best_wall = 0.0, best_fill = 0.0, best_sum = 0.0, total = 0.0;

    // runs the program reps times
    for (int r = 0; r < reps; r++) {
        double f, s, t;
        if (run_once(x, n, nthreads, &f, &s, &t) != 0) { free(x); return 1; }
        double wall = f + s;
        if (r == 0 || wall < best_wall) {
            best_wall = wall; best_fill = f; best_sum = s; total = t;
        }
    }

    // prints the results
    printf("threads   %d\n", nthreads);
    printf("n         %ld\n", n);
    printf("reps      %d\n", reps);
    printf("total     %.6f\n", total);
    printf("fill_s    %.4f\n", best_fill);
    printf("sum_s     %.4f\n", best_sum);
    printf("wall_s    %.4f\n", best_wall);

    // releases memory
    free(x);
    return 0;
}
