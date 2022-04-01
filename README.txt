By Owen Wexler (osw9) and Huzaif Mansuri (htm23)

We tested our program by 



- calling it with only an integer and then entering standard input. It prints to the terminal.
./ww 50


- calling it on a subdirectory, so it produces .wrap files inside the same folder.

./ww 30 subfolder

 - calling it on a single file, where it prints to the terminal.

./ww 60 sampletextfile




The following is Owen's MAKEFILE:
CC=gcc
CFLAGS= -Wall -Wvla -fsanitize=address,undefined
DEPS = wwheader.h
OBJ = ww.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ww: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
