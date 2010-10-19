//g++ larva.o creatures.o -lcurses
#include <cstdlib>
#include <curses.h>
#include <iostream>
#include <list>
#include "creatures.h"
#include "plants.h"
#include "globals.h"
#include "display.h"
using namespace std;
/***************************/
	int wall[Y][X];
	list <item*> map[Y][X];
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
		init_pair(7, COLOR_WHITE, COLOR_RED);
	turncount=0;
	for(int y=0; y<Y; y++)
	{
		for(int x=0; x<X; x++)
		{
			wall[y][x]=1;//(rand()%5)/4;
		}
	}
	for(int a=0; a<viewY; a++)
	{
		for(int b=0; b<viewX; b++)
		{
			view[a][b]=' ';
			visible[a][b]=false;
			color[a][b]=0;
		}
	}
	/************************************************************/
	/** Edit this if you want to change which monsters show up **/
	/************************************************************/
	for(int i=0;i<5;++i)
	{
		for(int j=0;j<15;++j)
			monsterlist.push_front(new larva);	//creates some tunnelers
//		monsterlist.push_front(new crab);	//creates some hammercrabs
//		monsterlist.push_front(new slime);	//food
		monsterlist.push_front(new slime);
//		monsterlist.push_front(new cube);	//creates some gelatinous cubes
	}
//	monsterlist.push_front(new mole);	//creates come coolaid mascots (they burst through walls)
	monsterlist.push_front(new dwarf);
	monsterlist.push_front(&cursor);	//places you into existance on this world
}
/***************************/
int main()
{
	init();
	while(inchar!='q')	//press q to quit
	{
		print();
		for(list<creature*>::iterator i=monsterlist.begin();i !=monsterlist.end();i++)
		{
			if(((*i)->hp)<=0)
				(*i)->die();
			(*i)->ap+=(*i)->speed;
			(*i)->energy--;
			if((*i)->ap>=100 && !(*i)->dead)
			{
				(*i)->ap-=100;
				(*i)->update();
			}
			if(((*i)->energy)<=0)//HACK!! this is a memory leak, this creature hasn't actually been deleted
			{
				int a=(*i)->y;
				int b=(*i)->x;
				for(list<item*>::iterator j=map[a][b].begin(); j != map[a][b].end(); j++)
					if((*j)==(*i))
						map[a][b].erase(j);
				monsterlist.erase(i);
			}
		}
		++turncount;
	}
	refresh();
	echo();			// turn echoing back on before exiting
	endwin();		// end curses control of the window
}
/***************************/