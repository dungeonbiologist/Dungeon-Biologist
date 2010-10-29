#include <cstdlib>
#include <curses.h>
#include <iostream>
#include <fstream>
#include <list>
#include "creatures.h"
#include "display.h"
using namespace std;
#ifndef GLOBALS
#define GLOBALS
	#define legal(a,b) (a>=0 && a<Y && b>=0 && b<X)
	#define inView(a,b) (a >= 0 && a<viewY && b>=0 && b<viewX)
	#define viewY 20		//height of view	
	#define viewX 40		//1/2 width of veiw
	#define Y 20		//height of world
	#define X 40		//width of world
	extern int wall[Y][X];
	extern list <creature*> map[Y][X];
	extern long inchar;
	extern int turncount;
	extern char view[viewY][viewX];
	extern bool visible[viewY][viewX];
	extern int color[viewY][viewX];
	extern list <creature*> monsterlist;
	extern list <creature*> prototypes;
struct errorfile
{
	int linenum;
	ofstream myfile;
	void log(string error)
	{
		if (!myfile.is_open())
			myfile.open("error.txt");
  		if (myfile.is_open())
			myfile << error.c_str() << "\n";
	}
};
extern errorfile parseError;
extern Messages input;
#endif