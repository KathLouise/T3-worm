.PHONY: all clean

all: worm

worm: worm.o portscanner.o payload.o
	$(CC) -o $@ $^

clean:
	$(RM) worm *.o *.txt
