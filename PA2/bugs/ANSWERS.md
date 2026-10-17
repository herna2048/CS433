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
Defect: compute was returning the address of a local variable, so when the thread was destroyed, the information was lost, creating a seg fault.
Fix: I added a dynamic array that returned the address of the memory from the calculation so ret had a valid pointer pointing to a long.

## bug4_errno.c
Defect: pthread_create and join were checking for an error case that doesn't exist for threads, and if it were to return an error, it would return a success case as errno doesn't get updated.
Fix: I created variables to store the return values from create and join, then use them to see if they had failed and use them in the error message instead of errno.
