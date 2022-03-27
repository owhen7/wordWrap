By Owen Wexler (osw9) and Huzaif Mansuri (Your netID)



A description of our testing Strategy:

We did not test yet.





The following is Owen's MAKEFILE:
CC=gcc
CFLAGS= -Wall -Wvla -fsanitize=address,undefined
DEPS = wwheader.h
OBJ = ww.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ww: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
