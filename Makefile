.PHONY: all clean

all: worm

worm: worm.o portscanner.o target_selection.o bruteforce.o payload.o
	$(CC) -o $@ $^

clean:
	$(RM) worm *.o *.txt
