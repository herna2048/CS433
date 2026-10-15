# PA2 — tsum: doing one job with many threads

Read `PA2_instructions.pdf` on Canvas for the full assignment. This file is the
short version.

## Build

```bash
unzip PA2_starter.zip   # if you have not already
cd PA2
make
```

A fresh `make` prints **three warnings**, and all three are expected:

- `work` unused and `sum_range` unused, in `tsum.c` — they stay unused until
  you implement `sum_range` and call it. Both go away once Part A works.
- `address of stack memory associated with local variable 'answer' returned`,
  in `bugs/bug3_stack.c` — that is the compiler handing you one of the Part B
  bugs for free. It goes away when you fix bug3.

Any **other** warning is yours. By the time you submit, `make` must be silent.

## What you edit

| File | What to do |
|---|---|
| `tsum.c` | Part A. Four TODOs: A1 the thread routine, A2 the arrays, A3 create, A4 join and total |
| `bugs/bug1_args.c` | Part B. Fix it |
| `bugs/bug2_nojoin.c` | Part B. Fix it |
| `bugs/bug3_stack.c` | Part B. Fix it |
| `bugs/bug4_errno.c` | Part B. Fix it |
| `bugs/ANSWERS.md` | Part B. One sentence naming each defect, one on your fix |
| `analysis.md` | Parts C and D. Thread counts, speedup table, measured S, four questions |

Do not edit `tcount.c`, and do not edit `work()` inside `tsum.c`.

## No locks

There is no mutex, semaphore or condition variable in this assignment and you
should not add one. Each thread writes only its own slot. If you think you need
a lock, your ranges overlap.

## Quick checks before you submit

```bash
make clean && make                       # warning-free
./tsum --threads 1 | grep total
./tsum --threads 8 | grep total          # same to within 1e-9 relative
./tsum --threads 3 --n 10 | grep total   # must match --threads 1 --n 10
./tsum --threads 0; echo $?              # must be 2
grep -nE 'mutex|semaphore|cond_|atomic' tsum.c   # only the file's own comment may match
```

## Submit

```bash
make clean
cd ..
tar czf LastName_FirstName_PA2.tar.gz PA2/
```
