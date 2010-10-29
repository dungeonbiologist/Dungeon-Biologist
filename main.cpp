#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"
#include "creatures.h"
#include "parser.h"
#include "display.h"
using namespace std;
/***************************/
	list <creature*> prototypes;
	int wall[Y][X];
	list <creature*> map[Y][X];
	long inchar;
	int turncount;
	char view[viewY][viewX];
	bool visible[viewY][viewX];
	int color[viewY][viewX];
	list<creature*> monsterlist;
/***************************/
	void init();
/***************************/
int main()
{
	parse("creatures.txt");
	init();
	creature player;
	player.y=Y/2;
	player.x=X/2;
	player.isType("PLAYER");
	while(inchar != 'q' && inchar != 'Q')
	{
		print();
		for(list<creature*>::iterator j=monsterlist.begin();j !=monsterlist.end();j++)
		{
			(*j) ->update();
			if((*j)->energy<=0)
			{
				(*j)=NULL;
				monsterlist.erase(j);
			}
		}
	}
	refresh();
	echo();			// turn echoing back on before exiting
	endwin();		// end curses control of the window
}
/***************************/
void init()
{
	WINDOW *window;
	window = initscr();		// set up the curses window
	raw();					// I don't think this does anything
	srand(time(NULL));		// initializes the random number generator
	keypad(stdscr, TRUE);	// this line allows me to use the the arrow keys
	clear();				// clear the window
	noecho();				// don't show typed characters on the screen
	start_color();
		init_pair(0,	COLOR_BLACK,	COLOR_BLACK);
		init_pair(1,	COLOR_WHITE,	COLOR_BLACK);
		init_pair(2,	COLOR_RED,		COLOR_BLACK);
		init_pair(3,	COLOR_YELLOW,	COLOR_BLACK);
		init_pair(4,	COLOR_GREEN,	COLOR_BLACK);
		init_pair(5,	COLOR_CYAN,		COLOR_BLACK);
		init_pair(6,	COLOR_BLUE,		COLOR_BLACK);
		init_pair(7,	COLOR_MAGENTA,	COLOR_BLACK);
		
		init_pair(8,	COLOR_WHITE,	COLOR_RED);
	turncount=0;
	
	for(int y=0; y<Y; y++)
	{
		for(int x=0; x<X; x++)
		{
			wall[y][x]=1;//(rand()%5)/4;	//1/5th of the time it==1
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
}
/***************************/