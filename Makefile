CC=g++
CFLAGS=-std=c++17 -O3
DBGFLAGS=-std=c++17 -ggdb3 -DDEBUG

SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o, $(SRCS))
DBGOBJS=$(patsubst %.cpp,%.dbg.o, $(SRCS))

.PHONY: clean depend all

all: BufferManager Debug

BufferManager:$(OBJS)
	$(CC) -o $@ $(OBJS) 
Debug:$(DBGOBJS)
	$(CC) -o $@ $(DBGOBJS)

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<
%.dbg.o:%.cpp
	$(CC) $(DBGFLAGS) -c -o $@ $<

clean:
	rm -f *.o *~ BufferManager Debug

depend:
	makedepend $(SRCS)
	makedepend -a -o .dbg.o $(SRCS)
