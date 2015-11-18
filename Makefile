CC=gcc
CFLAGS=-I.
DEPS = trim.h readIni.h
LIBS=
OBJS=keyscan.o trim.o  readIni.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: keyscan

keyscan: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o keyscan
