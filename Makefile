CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -O2

calc: main.c calc.c calc.h
	$(CC) $(CFLAGS) -o calc main.c calc.c

clean:
	rm -f calc

.PHONY: clean