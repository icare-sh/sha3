CC = gcc
CFLAGS = -Wall -g
FILES = src/main.c src/sha3.c

sha3: $(FILES)
	$(CC) $(CFLAGS) -o sha3 $(FILES) -g

clean:
	rm -f *.o sha3