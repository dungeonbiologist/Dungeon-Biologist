all : main.o creatures.o display.o
	g++ main.o creatures.o display.o -lcurses
main.o	: main.cpp display.h creatures.h globals.h
	g++ -c main.cpp
creatures.o : creatures.cpp creatures.h globals.h
	g++ -c creatures.cpp
display.o : display.cpp display.h globals.h
	g++ -c display.cpp
