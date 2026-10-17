# PA2 analysis — <your name>

Group members:  
Michael Hernandez, Evan Petersen, Wes Loewenberg, Tikhon Peterson, and Hamzeh Sabatini.  
Anyone outside your group you discussed this with: N/A  
AI assistance (tool, and what you used it for; write "none" if none): Our group agreed to avoid generative AI for generating completed code/solutions. Using it as a reference and Google search alternative for information or quick documentation lookup is OK.

---

## Part C — can the kernel see your threads?

**System 1**

- Machine and OS: WSL Ubuntu gcc 13.3.0
- Command used to count threads: `ls /proc/$(pgrep -n tcount)/task | wc -l`
- Threads before the pthread_create calls: 1
- Threads after: 5
- Difference: 4
- Does the difference match the number you asked for?: Yes, I asked for 4 threads and the count went up by exactly 4.

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
| 1 |0.4788|1.8745|2.3533 | 1.00 | 1.00 |
| 2 |0.4760 |0.9383 |1.4143 |1.9978 |1.6640 |
| 4 |0.4761 |0.4625 |0.9386 |4.0530 |2.5072 |
| 8 |0.5008 |0.2993 |0.8000 |6.2629 |2.9416 |
|  (cores) 20 |0.4778 |0.2221 |0.6999 |8.4399 | 3.3623|

### The serial fraction

- S = fill_s / wall_s at one thread = .2035
- Amdahl ceiling = 1 / S = 4.9150

### The four questions

**1. Your measured S, and the ceiling it implies.**  
The measured S implies that 20.35% of the machine is serial processing and cannot be sped up by adding more threads. The ceiling implies that the maximum speed up we can reach is x4.9140

**2. What whole-program speedup did you actually reach at the core count? How far below the ceiling, and one reason for the gap.**  
The core count whole-program speed up was at 3.3623, which is 1.292 below the ceiling. One possible reason for this gap is the need for thread overhead and management as well as limitations on memory bandwidth as the threads are all waiting on their turn for memeory access.

**3. Why is the sum-phase speedup so much better than the whole-program speedup? Which one would a user of this program notice?**  
Because it mainly relies on parallell processing while whole-program speedup includes the serial processing of the machine

**4. Why does `total` change in its last digits when the thread count changes, and why is that not a bug?**  
The last digits in total change due to floating point addition which is not perfect on computers. This is not a bug because we can ignore these last digits in our final analysis

## Stretch (only if you did it)

