#include <cstdlib>
#include <curses.h>
#include <iostream>
#include <list>
#include "creatures.h"
using namespace std;
#ifndef GLOBALS
#define GLOBALS
	#define legal(a,b) (a>=0 && a<Y && b>=0 && b<X)
	#define viewY 20		//height of view	
	#define viewX 40		//1/2 width of veiw
	#define Y 20		//height of world
	#define X 40		//width of world
	extern int wall[Y][X];
	extern char inchar;
	extern int turncount;
	extern char view[viewY][viewX];
	extern bool visible[viewY][viewX];
	extern int color[viewY][viewX];
	extern list<creature*> monsterlist;
	extern me cursor;
#endif