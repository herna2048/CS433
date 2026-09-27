# CS 433 PA1 — README write up

**Name(s):** Michael Hernandez, Evan Petersen, Wes Loewenberg, Tikhon Peterson, and Hamzeh Sabatini.  
**Date:** 9/28/26  

## What this is
`ptime` runs a command as a child process, waits for it, and reports how it
ended and how long it took (wall, user, and system time).

## Building
```
make
./ptime /bin/echo hello
```
Produces `ptime` and the workload programs. Requires gcc on Linux
(built and tested on WSL Ubuntu, gcc 13.3.0).

## Running
```
./ptime [COMMAND] [ARG]...
```
For example:
```
./ptime ./workload/spin 500
```

## Testing
```
make test
```

## Citations / sources
- GeeksforGeeks — looked up `ptime.c` usage of fork(), execvp(), waitpid(), etc.
- Course ZyBooks / lecture material (process lifecycle, stdio buffering).
