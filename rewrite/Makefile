CCSYSTEM = $(shell $(CC) -dumpmachine)

EXE = ".exe"
# Files
PROGNAME = "dungeon"
ifeq ($(findstring mingw, $(CCSYSTEM)), mingw)
PROGNAME = "dungeon.exe"
endif
ifeq ($(findstring cygwin, $(CCSYSTEM)), cygwin)
PROGNAME = "dungeon.exe"
endif
PROGEXT = EXE
ifeq ($(findstring linux, $(CCSYSTEM)), linux)
PROGEXT =
endif

all : main.o parser.o creatures.o trivect.o display.o
	g++ -o $(PROGNAME) main.o parser.o creatures.o trivect.o display.o -lcurses
main.o : main.cpp creatures.h parser.h globals.h display.h
	g++ -c -g main.cpp
parser.o : parser.cpp creatures.h parser.h
	g++ -c -g parser.cpp
creatures.o : creatures.cpp creatures.h trivect.h globals.h
	g++ -c -g creatures.cpp
trivect.o : trivect.cpp trivect.h
	g++ -c -g trivect.cpp
display.o : display.cpp display.h creatures.h 
	g++ -c -g display.cpp
clean : 
	rm -f $(PROGNAME) *~ *.a *.o *.out
