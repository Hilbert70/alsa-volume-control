CC=gcc
CFLAGS=-I.
DEPS = name_to_uid.h trim.h readIni.h
LIBS=-lasound
OBJS=keyscan.o alsa.o trim.o name_to_uid.o readIni.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: keyscan

keyscan: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o keyscan
