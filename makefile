CC=gcc
OBJ = transfer.o
CFILE = transfer.c
MATH = -lm
Cleanup=rm -rf *.o transfer
EXE = transfer
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
