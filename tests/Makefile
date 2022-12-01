
   
OBJSCPP	= example_cpp.o
OBJSCC	= example_c.o 
SOURCE	= example_c.c example_cpp.cpp
HEADER	= 
OUTDIR	= build
OUTCPP	= program_cpp
OUTCC	= program_c
CPP	 	= g++
CC		= gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = ../build/libPidLock.a

all: $(OBJSCPP) $(OBJSCC)
	mkdir -p $(OUTDIR)
	$(CC) $(OBJSCC) $(LFLAGS) -o $(OUTDIR)/$(OUTCC) -lstdc++
	$(CPP) $(OBJSCPP) $(LFLAGS) -o $(OUTDIR)/$(OUTCPP) $(LFLAGS) -lstdc++

example_c.o: example_c.c
	$(CC) $(FLAGS) example_c.c 

example_cpp.o: example_cpp.cpp
	$(CPP) $(FLAGS) example_cpp.cpp 


clean:
	rm -fr $(OBJSCPP) $(OBJSCC) $(OUTDIR)