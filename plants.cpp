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
			for(int i=-1;i<2;++i)
				for(int j=-1;j<2;++j)
				{
					v.y=i;
					v.x=j;
					if(!directionblocked())
					{
						move();
						v.y=0;
						v.x=0;
						if(!directionblocked())
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