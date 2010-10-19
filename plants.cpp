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
	plant::plant(char c) : creature(c){seed=true;}
	void plant::appear()
	{
		item::appear();
		if(seed==true)
		view[y][x]=',';
	}
	bool plant::sprout()
	{
		if(seed==true)
		{
			for(int i=-1;i<2;++i)
				for(int j=-1;j<2;++j)
				{
					v.y=i;
					v.x=j;
					if(!directionblocked())
					{
						move();
						seed=false;
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
//		small=true;
		while(wall[y][x]==0)
		{
			set(rand()%Y,rand()%X);
		}
	}
	void slime::update()
	{
		sprout();
		energy++;
		if(seed==false)
		{
			energy+=rand()%3/2;
			reproduce();
			if(wall[y][x]==0)
				hp=0;
		}
		v.y=0;
		v.x=0;
		move();
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