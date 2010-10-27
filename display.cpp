//g++ larva.o creatures.o -lcurses
#include <cstdlib>
#include <curses.h>
#include <iostream>
#include <list>
#include "globals.h"
#include "display.h"
using namespace std;
/***************************/
Messages input;
void print()
{
	for(int y=0; y<Y; y++)
		for(int x=0; x<X; x++)
		{
			if(wall[y][x]==1)
				view[y][x]='#';
			else
				view[y][x]='.';
//			for(i=map[y][x].begin();i != map[y][x].end();i++)
//			{
//				(*i)->appear();
//				if((*i)->x !=x ||(*i)->y !=y)		//DANGER! this could conceal a bug, fix what causes the disreprency first
//					map[y][x].erase(i);
//			}
		}
	for(list<creature*>::const_iterator j=monsterlist.begin();j !=monsterlist.end();j++)
		(*j)->appear();
	input.print();
/*
	for(int y=-1; y<=Y; y++)//it stops just before the the line reaches the goal, I have to do this to include the corners
	{
		bresenham(cursor.y,y,cursor.x,X);
		bresenham(cursor.y,y,cursor.x,-1);
	}
	for(int x=-1; x<=X; x++)
	{
		bresenham(cursor.y,Y,cursor.x,x);
		bresenham(cursor.y,-1,cursor.x,x);
	}
//*/
	clear();
	for(int y=0; y<viewY; y++)
		for(int x=0; x<viewX; x++)
		{
//			if(visible[y][x])
			{
				attrset(COLOR_PAIR(color[y][x]));
				mvaddch(y, x*2, view[y][x]);
			}
			color[y][x]=0;
			view[y][x]=' ';
			visible[y][x]=false;
		}
	refresh();		// refresh the screen
}
/***************************/
void printtext(int a, int b, string text)
{
	string::iterator i=text.begin();
	for(int j=0; i < text.end(); ++i )
	{
		if(inView(a,b+j))
		{
			view[a][b+j] = (*i);
			color[a][b+j] = 1;
		}
		++j;
	}
}
/***************************/
void printcentered(int a, string text)
{
	int offset=text.length();
	offset /= 2;
	printtext(a, viewX/2-offset, text);
}
/***************************/
char* convert(int number, char* pstring)
{
	number=abs(number);
	if(number > 9)
		pstring = convert(number/10, pstring);
	number = number%10;
	pstring[0] = char(number+'0'); //places the number translated into a charecter
	return &pstring[1];
}
/***************************/
void printNumber(int a, int b, int number)
{
	int count=1;
	while(number/(10*count) != 0)
		count++;
	if(number < 0)
	{
		char pstring[count+2];
		*convert(number, &pstring[1])='\0';	//we add a terminateing character to the end of the string
		pstring[0]='-';
		printtext(a,b,pstring);
	}
	else
	{
		char pstring[count+1];
		*convert(number, pstring)='\0';	//we add a terminateing character to the end of the string
		printtext(a,b,pstring);
	}
}
/***************************/
int abs(int a)
{
	if(a<0)	return a*-1;
	else	return a;
}
void swap(int &a,int &b)
{
	int temp=a;
	a=b;
	b=temp;
}
void bresenham(int y0,int y1,int x0,int x1)
{
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if(steep)
	{
		swap(x0, y0);
		swap(x1, y1);
	}
	int deltay = abs(y1 - y0);
		int ystep;
	int y = y0;
	if (y0 < y1)
		ystep = 1;
	else 
		ystep = -1;
	int dir=1;
	if(x0 > x1)
		dir=-1;
	int deltax = dir*(x1 - x0);
	int error = deltax / 2;
	bool colored=false; //tests wether you have gone through a translucen creature
	int lightcolor=0;
	for(int x=x0; x!=x1; x+=dir)
	{
		if(steep && legal(x,y))
		{
			visible[x][y]=true;
			if(colored)
				color[x][y]=lightcolor;
			if(wall[x][y])
				return;
//			for(list<creature*>::iterator i=monsterlist.begin();i !=monsterlist.end();i++)
//				if((*i)->sameplace(x,y) && (*i)->translucent==true)
//				{
//					lightcolor=(*i)->hue;
//					colored=true;
//				}
		}
		else if(legal(y,x))
		{
			visible[y][x]=true;
			if(colored)
				color[y][x]=lightcolor;
			if(wall[y][x])
				return;
//			for(list<creature*>::iterator i=monsterlist.begin();i !=monsterlist.end();i++)
//				if((*i)->sameplace(y,x) && (*i)->translucent==true)
//				{
//					lightcolor=(*i)->hue;
//					colored=true;
//				}
		}
		error -= deltay;
		if(error < 0)
		{
	       	y += ystep;
			error += deltax;
		}
	}
}
/***************************/