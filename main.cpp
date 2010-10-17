//g++ larva.o creatures.o -lcurses
#include <cstdlib>
#include <curses.h>
#include <iostream>
#include <list>
#include "creatures.h"
#include "globals.h"
#include "display.h"
using namespace std;
/***************************/
	int wall[Y][X];
	char inchar;
	int turncount;
	char view[viewY][viewX];
	bool visible[viewY][viewX];
	int color[viewY][viewX];
	list<creature*> monsterlist;
	me cursor;
/***************************/
void init();
/***************************/
void init()
{
	turncount=0;
	for(int y=0; y<Y; y++)
	{
		for(int x=0; x<X; x++)
		{
			view[y][x]=' ';
			wall[y][x]=(rand()%5)/4;
			visible[y][x]=false;
			color[y][x]=0;
		}
	}
/************************************************************/
/** Edit this if you want to change which monsters show up **/
/************************************************************/
	for(int i=0;i<5;++i)
	{
//		monsterlist.push_front(new larva);	//creates some tunnelers
//		monsterlist.push_front(new crab);	//creates some hammercrabs
		monsterlist.push_front(new mole);	//creates come coolaid mascots (they burst through walls)
		monsterlist.push_front(new cube);	//creates some gelotinous cubes
	}
	monsterlist.push_front(&cursor);	//places you into existance on this world
}
/***************************/
int main()
{
	WINDOW *window;
	window = initscr();		// set up the curses window
	srand(time(NULL));	// initializes the random number generator
//	nodelay(initscr(),true);			// don't wait for user input
	clear();				// clear the window
	noecho();				// don't show typed characters on the screen
	start_color();
		init_pair(0, COLOR_WHITE, COLOR_BLACK);
		init_pair(1, COLOR_RED, COLOR_BLACK);
		init_pair(2, COLOR_BLUE, COLOR_BLACK);
		init_pair(3, COLOR_YELLOW, COLOR_BLACK);
		init_pair(4, COLOR_GREEN, COLOR_BLACK);
		init_pair(5, COLOR_CYAN, COLOR_BLACK);
		init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init();
	while(inchar!='q')	//press q to quit
	{
		print();
		for(list<creature*>::iterator i=monsterlist.begin();i !=monsterlist.end();i++)
		{
			if(((*i)->hp)<=0)
			{
				(*i)->die();
				monsterlist.erase(i);
			}			
			(*i)->energy+=(*i)->speed;
			if((*i)->energy>=100)
			{
				(*i)->energy-=100;
				(*i)->act();
			}
		}
		++turncount;
	}
	refresh();
	echo();			// turn echoing back on before exiting
	endwin();		// end curses control of the window
}
/***************************/