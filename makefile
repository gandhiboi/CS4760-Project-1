CC=gcc
OBJ = bt.o
CFILE = bt.c
MATH = -lm
Cleanup=rm -rf *.o transfer
EXE = bt
WITHNAME = -o
CFLAGS=-c

.SUFFIXES: .c .o

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(WITHNAME) $@ $(OBJ) $(MATH)

.c.o:
	$(CC) $(CFLAGS) $(MATH) $<

clean:
	$(Cleanup)
