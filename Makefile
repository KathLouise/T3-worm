.PHONY: all clean

all: worm

worm: worm.o bruteforce.o payload.o portscanner.o
	$(CC) -o $@ $^

clean:
	$(RM) worm *.o *.txt
