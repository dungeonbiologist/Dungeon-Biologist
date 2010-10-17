

CCSYSTEM = $(shell $(CC) -dumpmachine)

EXE = ".exe"
# Files
PROGNAME = "dungeonbiology"
ifeq ($(findstring mingw, $(CCSYSTEM)), mingw)
PROGNAME = "dungeonbiology.exe"
endif
ifeq ($(findstring cygwin, $(CCSYSTEM)), cygwin)
PROGNAME = "dungeonbiology.exe"
endif
PROGEXT = EXE
ifeq ($(findstring linux, $(CCSYSTEM)), linux)
PROGEXT =
endif

all : main.o creatures.o display.o
	g++ -o $(PROGNAME) main.o creatures.o display.o -lcurses
main.o	: main.cpp display.h creatures.h globals.h
	g++ -c main.cpp
creatures.o : creatures.cpp creatures.h globals.h
	g++ -c creatures.cpp
display.o : display.cpp display.h globals.h
	g++ -c display.cpp

clean : 
	rm -f $(PROGNAME) *~ *.a *.o *.out
