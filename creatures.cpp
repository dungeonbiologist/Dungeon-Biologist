#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "creatures.h"
#include "display.h"
using namespace std;

bool creature::isType(string symbol)
{
	list<creature*>::iterator i;
	for(i=prototypes.begin();i !=prototypes.end();i++)
	{
		if((*i)->identifier==symbol)
		{
			break;
		}
	}
	//implement later; later have it continue scanning the document to look for the definition
	if(i==prototypes.end())
		return false;
	else
	{
		transformTo(*i);
		monsterlist.push_front(this);
		return true;
	}
}
void creature::update()
{
//	if(!(properties & live))
//		return;
//	actionpoints += speed;
//	if(actionpoints < 100)	return;
	choosemove(1);
	dig();
	move();
}
bool creature::move()
{
	int a = v.y+y;
	int b = v.x+x;
	if(legal(a,b) && !blocked())
	{
		y=a;
		x=b;
		return true;
	}
	return false;
}
bool creature::blocked()	 //check wether all of it's contrains allow it to move here
{
	bool b=false;
	if(blockedby & 1)
		b= b || blockedbyWalls();
	if(blockedby & 2)
		b= b || adjacenttoWalls();
	return b;
}
bool creature::blockedbyWalls()
{
	int a = v.y+y;
	int b = v.x+x;
	if(wall[a][b] &1)
		return true;
	return false;
}
bool creature::adjacenttoWalls()
{
	int a = v.y+y;
	int b = v.x+x;
	for(int i=-1;i<2;i++)
	{
		if(legal(a+i,b) && wall[a+i][b] &1)
			return false;
		if(legal(a,b+i) && wall[a][b+i] &1)
			return false;
	}
	return true;
}
void creature::choosemove(int a)
{
	switch(movetype)
	{
		case 0 :
			moveNone(a);
			break;
		case 1 :
			moveInertial(a);
			break;
		case 2 :
			moveSnake(a);
			break;
		case 3 :
			moveManual(a);
			break;
		default :
			moveNone(a);
			break;
	}
}
void creature::moveNone(int prob) //none, zeros out velocity;
{
		v.setTo(0);
}
void creature::moveInertial(int prob) //changes velocity
{
	if(rand()%prob==0)
		v.y = v.y + rand()%2*2-1; //+ -1 or 1
		v.x = v.x + rand()%2*2-1;
}
void creature::moveSnake(int prob)
{
	if(v.y==0 && v.x==0)
		v.polarize();
	if(rand() % prob==0)
		v.turn(rand()%2*2-1);
}
void creature::moveManual(int prob)
{
	energy++;
	inchar = getch();
//	if(inchar != ERR)
	{
		v.setTo(0);
			if(inchar=='7'||inchar=='8'||inchar=='9' || inchar==KEY_UP)
				v.y=-1;
			if(inchar=='1'||inchar=='2'||inchar=='3' || inchar==KEY_DOWN)
				v.y=1;
			if(inchar=='7'||inchar=='4'||inchar=='1' || inchar==KEY_LEFT)
				v.x=-1;
			if(inchar=='3'||inchar=='6'||inchar=='9' || inchar==KEY_RIGHT)
				v.x=1;
			else if(inchar==' ')
				wall[y][x]^=1;
			else if(inchar=='x')
			{
				list<creature*>::iterator i=map[y][x].begin();
				if(wall[y][x])
					printtext(0,1,"wall");
				else
					printtext(0,1,"floor");
				for(int j=1;i !=map[y][x].end();i++)
				{
					printtext(j,1,(*i)->Cname);
					if(!((*i)->properties & live))
						printtext(j,(*i)->Cname.size(),"dead");
					++j;
				}
				getch();
			}
			else if(inchar >= 'A' && inchar <= 'Z')
			{
				input.message+=inchar;
			}
			if (inchar == 10) //KEY_ENTER
			{		//add a creature to the field
				creature* temp=new creature;		//because if I just declare a creature by value it will go out of scope
				temp->isType(input.message);
				temp->y=y;
				temp->x=x;
				input.message.clear();
			}
	}
}
bool creature::dig()
{
	int b;
	switch(digtype)
	{
		case 0 :
			b=false;
			break;
		case 1 :
			b=digexposed();
			break;
		case 2 :
			b=digwall();
			break;
		case 3 :
			b=digmaze();
			break;
		default :
			b=false;
			break;
	}
	if(b==true)
		wall[y+v.y][x+v.x]=0; //remove wall
	return b;
}
bool creature::digmaze()
{
	int a=y+v.y;
	int b=x+v.x;
	if(legal(a,b) && (wall[a][b] & 1) && (v.y*v.x==0))	//if it is orthagonal
	{
		for(int i=-1;i<2;++i)
		{
			for(int j=-1;j<2;++j)
			{
				if((i*v.y)+(j*v.x)>-1	//only check tiles on the other side of this wall
				&& (!legal(a+i,b+j) || wall[a+i][b+j]==0))	// if you would be connectin spaces or digging to the edge
					return false;
			}
		}
		return true;	//passed all the tests
	}
	return false;
}
bool creature::digwall()
{
	int a=y+v.y;
	int b=x+v.x;
	if(legal(a,b) && (wall[a][b] & 1))	//if it is orthagonal
	{
		if((legal(a-v.x,b+v.y) && (wall[a-v.x][b+v.y] & 1))		//if there is a wall to the left (v.x and v.y are switched on purpose)
		&&(legal(a+v.x,b-v.y)  && (wall[a+v.x][b-v.y] & 1))				//and a wall to the right
		&&(legal(a+v.y,b+v.x)  && ! (wall[a+v.y][b+v.x] & 1)))	//and no wall on the oposite side
			return  true;
	}
	return false;
}
bool creature::digexposed()
{
	int a=y+v.y;
	int b=x+v.x;
	if(legal(a,b) && (wall[a][b] & 1))	//if it is orthagonal
	{
		int sidesExposed=0;
		for(int i=-1; i<2;i+=2)
		{
			if(legal(a+i,b) && (wall[a+i][b]&1)==0)		//add up the exposed sides
				++sidesExposed;
			if(legal(a,b+i) && (wall[a][b+i]&1)==0)
				++sidesExposed;
		}
		if(sidesExposed > 2)
			return true;
	}
	return false;
}
void creature::transformTo(creature* master)
{
	int tempy=y;
	int tempx=x;
	int temphp=hp;
	int tempenergy=energy;
	int tempactionpoints=actionpoints;
	*this= *master;
	y=tempy;
	x=tempx;
	hp=temphp;
	energy=tempenergy;
	actionpoints=tempactionpoints;
}
creature::creature()
{
	identifier="???";
	appearance='?';
	Cname="unknown";
	y=0;
	x=0;
	v.setTo(0);
	hue=1;
	speed=100;
	hp=5;
	actionpoints=0;
	energy=100;					//energy is used for special abilities and reproduceing
	properties=0;								//bitfield of counterdigestive properties
	appetite=0;								//bitfield of digestive abilities
	movetype=0;
	blockedby=0;
	digtype=0;
}
void creature::appear()
{
	if(legal(y,x))
	{
		view[y][x]=appearance;
		color[y][x]=hue;
	}
	else
		parseError.log("creature cordinants out of bounds");
}
