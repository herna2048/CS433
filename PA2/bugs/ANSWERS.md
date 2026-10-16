# Part B answers

For each program, one sentence naming the actual defect, and one sentence
saying what you changed. Do not paste code here; the code is in the .c files.

## bug1_args.c
Defect: Every thread was passed a pointer to the same loop variable i, which keeps changing and goes out of scope, so each thread read whatever i held when it ran instead of its own index.
Fix: I gave each thread a pointer to its own element of an ids array, which holds its index and stays valid until after the joins.

## bug2_nojoin.c
Defect: main summed the slots without waiting for the threads, so it read them before the threads had finished counting.
Fix: I added a pthread_join for every thread before the summing loop, so every slot is complete when main reads it.

## bug3_stack.c
Defect:
Fix:

## bug4_errno.c
Defect:
Fix:
