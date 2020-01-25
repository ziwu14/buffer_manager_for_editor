SRC_PATH=src
OBJ_PATH=obj

CC=g++
CFLAGS=-std=c++17 -O3
DBGFLAGS=-std=c++17 -ggdb3 -DDEBUG

SRCS=$(wildcard $(SRC_PATH)/*.cpp)
BASE=$(basename $(notdir $(SRCS)))
OBJS=$(addsuffix .o, $(addprefix $(OBJ_PATH)/, $(BASE)))
DBGOBJS=$(addsuffix .dbg.o, $(addprefix $(OBJ_PATH)/, $(BASE)))

.PHONY: clean depend all

all: BufferManager Debug

BufferManager:$(OBJS)
	$(CC) -o $@ $(OBJS) 
Debug:$(DBGOBJS)
	$(CC) -o $@ $(DBGOBJS)

$(OBJ_PATH)/%.o:$(SRC_PATH)/%.cpp
	$(CC) $(CFLAGS)   -c -o $@ $<
$(OBJ_PATH)/%.dbg.o:$(SRC_PATH)/%.cpp
	$(CC) $(DBGFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_PATH)/*.o $(SRC_PATH)/*~ *~ BufferManager Debug

depend:
	makedepend $(SRCS)
	makedepend -a -o .dbg.o $(SRCS)
