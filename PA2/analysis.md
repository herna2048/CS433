# PA2 analysis — <your name>

Group members (all names, or write "working alone"):
Anyone outside your group you discussed this with:
AI assistance (tool, and what you used it for; write "none" if none):

---

## Part C — can the kernel see your threads?

**System 1**

- Machine and OS:
- Command used to count threads:
- Threads before the pthread_create calls:
- Threads after:
- Difference:
- Does the difference match the number you asked for?

**System 2** (optional but recommended — the course server and your laptop)

- Machine and OS:
- Command used:
- Before / after / difference:

**One-to-one or many-to-one?** Two sentences. Say what you would have seen
instead if the mapping were many-to-one.

---

## Part D — measurements

Machine used for the timings:
Core count, and the command you got it from:
Plugged in, or on battery:

### Speedup table

`--n 20000000 --reps 3`. Speedup is against your own 1-thread run.

| threads | fill_s | sum_s | wall_s | sum-phase speedup | whole-program speedup |
|---|---|---|---|---|---|
| 1 | | | | 1.00 | 1.00 |
| 2 | | | | | |
| 4 | | | | | |
| 8 | | | | | |
|  (cores) | | | | | |

### The serial fraction

- S = fill_s / wall_s at one thread =
- Amdahl ceiling = 1 / S =

### The four questions

**1. Your measured S, and the ceiling it implies.**

**2. What whole-program speedup did you actually reach at the core count? How
far below the ceiling, and one reason for the gap.**

**3. Why is the sum-phase speedup so much better than the whole-program
speedup? Which one would a user of this program notice?**

**4. Why does `total` change in its last digits when the thread count changes,
and why is that not a bug?**

---

## Stretch (only if you did it)

