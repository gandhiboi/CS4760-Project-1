CC = gcc
CFLAGS = -g
TARGET = bt
OBJS = bt.o queue.o permissions.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
bt.o: bt.c
	$(CC) $(CFLAGS) -c bt.c
queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c
clean:
	rm -f *.o $(TARGET)
