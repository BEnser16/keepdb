CC = gcc
CFLAGS = -Wall -g
TARGET = keepdb

all: $(TARGET)

$(TARGET): main.o kv_store.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c kv_store.h
	$(CC) $(CFLAGS) -c main.c

kv_store.o: kv_store.c kv_store.h
	$(CC) $(CFLAGS) -c kv_store.c

clean:
	rm -f *.o $(TARGET)
