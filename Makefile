CC=gcc
CFLAGS=-Wall -g -fPIC $(CINCLUDE)

SOURCES=ring_buf.c
OBJS=$(patsubst %.c, %.o,$(SOURCES))
TARGET=libring_buf.so

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -shared  -o $(TARGET) $(LDFLAGS)

clean:
	rm -rf *.o $(TARGET)

install:
	cp $(TARGET) ../main

