CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -O2 -pthread
LDLIBS  = -lm
BUGS    = bugs/bug1_args bugs/bug2_nojoin bugs/bug3_stack bugs/bug4_errno

all: tsum tcount $(BUGS)

tsum: tsum.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

tcount: tcount.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

bugs/%: bugs/%.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f tsum tcount $(BUGS)

.PHONY: all clean
