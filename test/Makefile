CC=gcc
CXX=g++
CFLAGS=-Wall -g -fPIC $(CINCLUDE)
CXXFLAGS=$(CFLAGS) -std=gnu++11 -Wall

SOURCES= test.cpp
OBJS+=$(patsubst %.cpp, %.o,$(SOURCES))
TARGET=test

LDFLAGS := -lgtest -lgtest_main -lpthread -L.. -lring_buf

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET):$(OBJS)
	$(CXX) $(OBJS)  -o $(TARGET) $(LDFLAGS)

clean:
	rm -rf *.o $(TARGET)

install:
	cp $(TARGET) ../main