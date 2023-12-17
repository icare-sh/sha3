CC = gcc
CFLAGS = -Wall -g
FILES = src/main.c

sha3: sha3.o
	$(CC) $(CFLAGS) -o $@ $^

sha3.o: $(FILES)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f *.o sha3