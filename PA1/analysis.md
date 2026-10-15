# PA1 Written Analysis

**Name(s):** Michael Hernandez, Evan Petersen, Wes Loewenberg, Tikhon Peterson, and Hamzeh Sabatini.  
**Date:** 9/28/26  
**Built and tested on:** _course server and on WSL Ubuntu gcc 13.3.0_

Answer all five questions, **including every lettered part** — a question with
(a), (b) and (c) is not answered until all three are. Two to four sentences per
part is the right size; the whole file should come to two or three pages, not
one and not eight. Paste real output from your own runs where a question asks
for it; invented output is easy to spot and is handled as an academic-honesty
issue, not as a wrong answer.

Budget about two hours for this, and write it while the code is fresh in your
head. It is 20 points, which is more than any two tests are worth.

Write your own explanation. "It gets buffered" is not an explanation. Say
*what* is buffered, *where* that buffer lives, and *what happens to it*.

---

## Q1. The buffering trap (4 points)

Build and run the provided demonstration two different ways:

```
make
./workload/buffer_trap
./workload/buffer_trap | cat
```

Run the first command in a real terminal (an ssh session or a WSL shell), not from an editor's Run button. If both outputs look the same, your stdout is not a terminal.

**(a)** Paste both outputs exactly as you got them.

Terminal run:
```bash
A: printed before fork()
B: child
C: parent
```
Piped run:
```bash
A: printed before fork()
B: child
A: printed before fork()
C: parent
```
**(b)** They are different, and the program did not change. Explain why, in terms of where `printf` actually writes and what `fork()` copies.

_printf writes to a stdio buffer in the process, not directly to the kernel. With a pipe, line A can still be in the buffer at fork(). fork() copies that buffer to parent and child. Both flush on exit, so A can print twice._


**(c)** Name the one library call that fixes it, say exactly where it goes in
`ptime.c`, and explain why placing it *after* the `fork()` would not work.

_fflush(NULL) in ptime.c after clock_gettime, before fork(). Flush after fork() does not clear the child's copy of the buffer._


---

## Q2. Wall time is not CPU time (4 points)

Run both of these with your finished `ptime` and paste both reports:

```
./ptime ./workload/sleeper 1000
./ptime ./workload/spin 1000
```

**(a)** For each one, say which is larger — wall, or user+sys — and why.
```bash
sleeper: slept 1000 ms
=== ptime ===
command : ./workload/sleeper 1000
pid     : 5109
status  : exited 0
wall    : 1.007 s
user    : 0.000 s
sys     : 0.001 s
```
```bash
spin: burned 1000 ms of CPU (checksum 152440000)
=== ptime ===
command : ./workload/spin 1000
pid     : 5113
status  : exited 0
wall    : 1.003 s
user    : 0.923 s
sys     : 0.013 s
```
_Sleeper: wall (1.007 s) >> user+sys (0.001 s), mostly sleeping.  
Spin: wall (1.003 s) ≈ user+sys (0.936 s), CPU-bound._

**(b)** A program can finish with `user + sys` *greater* than `wall`. Describe
a program that would do that. (None of the provided workload programs on its own will do it; say what
kind of program would, and what hardware makes it possible. You can check your answer with
`./ptime sh -c "./workload/spin 500 & ./workload/spin 500 & wait"`.)
```bash
spin: burned 500 ms of CPU (checksum 80540000)
spin: burned 500 ms of CPU (checksum 80780000)
=== ptime ===
command : sh -c ./workload/spin 500 & ./workload/spin 500 & wait
pid     : 5116
status  : exited 0
wall    : 0.506 s
user    : 0.982 s
sys     : 0.012 s
```
_Two spin processes on a multi-core CPU in parallel. user+sys (0.994 s) > wall (0.506 s)._


---

## Q3. Which clock, and why (4 points)

You used `CLOCK_MONOTONIC`. `CLOCK_REALTIME` also exists, and it reports the
wall-clock time of day.

**(a)** Describe a concrete situation in which measuring an interval with
`CLOCK_REALTIME` would give a wrong answer — including one where the measured duration comes out *negative*.

_CLOCK_REALTIME can jump if NTP or an admin changes the system clock. end − start can be wrong or negative. CLOCK_MONOTONIC only counts forward, good for intervals._

**(b)** Given that, why does `CLOCK_REALTIME` exist at all? Name one job it is right for and `CLOCK_MONOTONIC` is wrong for.

_CLOCK_REALTIME for time-of-day (logs, timestamps). CLOCK_MONOTONIC is not calendar time._


---

## Q4. `_exit` versus `exit` in the child (4 points)

Your child process calls `_exit()` after a failed `execvp()` — never `exit()`,
and never `return`.

**(a)** What does `exit()` do that `_exit()` does not?

_exit() does peforms user-space cleanup before it terminates whereas _exit() skips that step and just teminates._

**(b)** Connect this to Q1. Suppose `ptime` also printed a line to stdout before `fork()` (a debug line, say). Describe the specific wrong output a student would see if they used `exit()` in the child and had also skipped the fix from Q1. Pasting a real run is the best answer.
```bash
./ptime thiscommanddoesnotexist
Line before forking...
ptime: cannot run 'thiscommanddoesnotexist': No such file or directory
=== ptime ===
command : thiscommanddoesnotexist
pid     : 58562
status  : exited 127
wall    : 0.004 s
user    : 0.000 s
sys     : 0.002 s

Using exit
```

**(c)** In a program larger than this one, why is `return` from `main()` in the
child worse still?

_Since it is the same as using exit(). It also crashes the parent process, allowing the child to keep running._


---

## Q5. Whose CPU time did you measure? (4 points)

You called `getrusage(RUSAGE_CHILDREN, &ru)` once, after `waitpid` returned.

**(a)** Suppose `ptime` were changed to run the command three times in a row,
calling `getrusage(RUSAGE_CHILDREN, ...)` after each one. What would the third
call report — that run's CPU time, or something else? Say precisely what.

_No, it would report the accumilated CPU times of all 3 childs rather than just the third child, as getrusage() doesn't reset and just accumulates._

**(b)** Give a correct way to get *per-run* CPU time out of `RUSAGE_CHILDREN`
anyway.

_In order to get the per-run CPU time, you would have to save each child after each iteration._

```c
struct rusage ru;
struct rusage runs[NUM_RUNS];
for(int i = 0; i < NUM_RUNS; i++){
    while (waitpid(pid, &status, 0) < 0) {
        if (errno == EINTR) {
            continue;
        }
        else {
            perror("waitpid failed");
            return PTIME_FAILURE;
        }
    }
    if (getrusage(RUSAGE_CHILDREN, &ru) != 0) {
        perror("ptime: getrusage");
        return PTIME_FAILURE;
    }
    runs[i] = ru;
}
```

**(c)** What would `getrusage(RUSAGE_SELF, ...)` have reported instead, and
roughly what number would you have seen in your report?

_Instead of the reaped child, it reports the parent instead. As self is hardly moving as the child's data is not being placed inside, it would be a fraction of a milisecond._


---

## Optional: stretch features

If you implemented any STRETCH items, list which ones and give one example
command line plus its output for each. Extra credit is not awarded for stretch
work that is not documented here.

_Your answer:_
