OBJS	= build/pidLock.o
SOURCE	= pidLock.cpp
HEADER	= pidLock.hpp
OUT	= build/libPidLock.a
CC	= g++
AR	= ar
FLAGS 	= -c -g -Wall -D_GLIBCXX_USE_CXX11_ABI=1
LFLAGS = 

all: $(OBJS)
	$(AR) rvs $(OUT) $(OBJS)

build/pidLock.o: pidLock.cpp
	mkdir -p build/
	$(CC) $(FLAGS) $(LFLAGS) pidLock.cpp -o $(OBJS)


clean:
	rm -f $(OBJS) $(OUT)
	rmdir build
