#include <cstdlib>
#include <curses.h>
#include <iostream>
#include <list>
#include <math.h>
#include "creatures.h"
#include "plants.h"
#include "globals.h"
using namespace std;
/***************************/
	plant::plant(char c) : creature(c){egg=true;}
	void plant::appear()
	{
		item::appear();
		if(egg==true)
		view[y][x]=',';
	}
	bool plant::sprout()
	{
		if(egg==true)
		{
			v.y=0;
			v.x=0;
			if(directionblocked())
			{
				v.y=1;
				v.x=1;
				for(int i=0;directionblocked()&& i<8;++i)
					v.turn(-1);
			}
			if(!directionblocked())
			{
				move();
				if(!directionblocked())
				{
					egg=false;
					return true;
				}
			}
		}
		return false;
	}
/***************************/
	slime::slime():plant('#')
	{
		Cname="Wall slime";
		hue=4;
		small=true;
		while(wall[y][x]==0)
		{
			set(rand()%Y,rand()%X);
		}
	}
	void slime::update()
	{
		creature::update();
		energy++;
		if(heldby==NULL)
		{
			if(egg==false)
			{
				energy+=rand()%2;
				reproduce();
				if(wall[y][x]==0)
					hp=0;
			}
			sprout();
		}
	}
	int slime::directionblocked()
	{
		int a=v.y+y;
		int b=v.x+x;
		if(!legal(a,b) || wall[a][b]==0)
			return true;
		for(list<creature*>::const_iterator i=monsterlist.begin();i !=monsterlist.end();i++)
			if((*i)->sameplace(a,b) && (*i)!=this)
				return 1;
		return false;
	}
	bool slime::reproduce()
	{
		if(energy>200)
		{
			if(wall[y+1][x]&& wall[y-1][x]&& wall[y][x+1]&& wall[y][x-1])
				return false;
			else
			{
				v.y=-1;
				v.x=0;
				for(int i=0;i<4;++i)
				{
					v.turn(-1);
					v.turn(-1);
					if(legal(y+v.y,x+v.x) && wall[y+v.y][x+v.x]==0)
						break;
				}
				monsterlist.push_front(new slime);
				energy-=100;
				list<creature*>::const_iterator i=monsterlist.begin();
					(*i)->set(y+v.y,x+v.x);
				return true;
			}
		}
	}
/***************************/
	moss::moss():plant(';')
	{
		Cname="Moss";
		hue=4;
		small=true;
	}
	void moss::update()
	{
		creature::update();
		energy++;
		if(heldby==NULL)
		{
			if(egg==false)
			{
				energy+=rand()%2;
				reproduce();
				if(wall[y][x])
					hp=0;
			}
			sprout();
		}
	}
	int moss::directionblocked()
	{
		int a=v.y+y;
		int b=v.x+x;
		if(!legal(a,b) || wall[a][b])
			return true;
		for(list<creature*>::const_iterator i=monsterlist.begin();i !=monsterlist.end();i++)
			if((*i)->sameplace(a,b) && (*i)!=this)
				return 1;
		return false;
	}
	bool moss::reproduce()
	{
		if(energy>200)
		{
			v.y=-1;
			v.x=0;
			for(int i=0;i<9;++i)
			{
				v.turn(-1);
				if(legal(y+v.y,x+v.x) && wall[y+v.y][x+v.x]==0 && map[y+v.y][x+v.x].size()==0)
					break;
				if(i==8)	//if it has gone all the way around
					return false;
			}
			monsterlist.push_front(new moss);
			energy-=100;
			list<creature*>::const_iterator i=monsterlist.begin();
				(*i)->set(y+v.y,x+v.x);
			return true;
		}
	}