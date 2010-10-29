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
void creature::update()	//THIS IS THE MAIN FUNCTION
{
	if(!player && !photosynthesizes && energy>0)
		energy--;
	if(photosynthesizes && !(properties & isegg))
		energy++;
	if(energy<=0 || hp<=0)
	{
		properties &= ~live;
		return;
	}
	actionpoints += speed;
	if(actionpoints < 100)	return;
	actionpoints-=100;
	reproduce();
	eat();
	choosemove(1);
	dig();
	avoidobstacles(2);	//find a workable path
	move();
	if(properties & isegg)
		hatch();
}
bool creature::eat()
{
	list<creature*>::iterator i;
	for(i=monsterlist.begin();i !=monsterlist.end();i++)
	{
		int a=(*i)->y - y;
		int b=(*i)->x - x;
		if((*i) != this && a*a<=1 && b*b<=1)	//not me but next to me
			if((appetite | ~(*i)->properties) == ~0)	//for every 1 they have We must have a 1 in the same spot
//				if((*i)->energy >= bightsize)
				{
					(*i)->attacked(this);
					(*i)->energy-=bitesize;
					energy+=(bitesize-waste);
					return true;
				}
	}
	return false;
}
void creature::attacked(creature* agressor)
{
	switch(response)
	{
		case 0 :
			break;
		case 1 :
			calcify(agressor);
			break;
		default :
			break;
	}
}
void creature::calcify(creature* agressor)
{
	v.y=y-agressor->y;	//face away
	v.x=x-agressor->x;
	avoidobstacles(3);
	wall[y][x] |= 1;	//stick wall here
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
	if(blockedby & 4)
		b= b || blockedByCreatures();
	if(blockedby & 8)
		b= b || !blockedbyWalls();	//it can only move inside walls
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
bool creature::blockedByCreatures()
{
	list<creature*>::iterator i;
	for(i=monsterlist.begin();i !=monsterlist.end();i++)
	{
		int a=(*i)->y - y;
		int b=(*i)->x - x;
		if((*i) != this && a==v.y && b==v.x)	//not me but next to me
			return true;
	}
	return false;
}

bool creature::avoidobstacles(int angle) //angel = how far they can turn
{
	if(angle>4)angle=4;
	if(angle<0)angle=0;
	if(blocked())
	{
		int a=rand()%2*2-1;
		vect left,right,old;
		old=v;
		left=v;	//abstractly left, it could actually be either direction
		right=v;	//opposite of 'left'
		for(int i=0;i<angle;++i)
		{
			right.turn(a);
			left.turn(-a);
			v=right;
			if(!blocked())
				return true;
			v=left;
			if(!blocked())
				return true;
		}
		v=old;
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
			else if(inchar >= 'A' && inchar <= 'Z')
				input.message+=inchar;
			else if(inchar == '?')
				input.displayhelp=true;
			if(inchar == KEY_DC || inchar == KEY_BACKSPACE || inchar==127 || inchar==8)
			{
				string::iterator i=input.message.end();
				if(input.message.length()>0)
					input.message.erase(i-1);
			}
			if (inchar == 10 || inchar == '\n') //KEY_ENTER
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
bool creature::hatch()
{
	bool b=true;
	switch(hatchwhen)
	{
		case 1 :
			b &= inToTile();
			break;
	}
	if(b)
		properties &= ~isegg;
	return b;
}
bool creature::inToTile()
{
	v.setTo(0);
	if(blocked())
	{
		v.polarize();
		int i;
		for(i=0;i<9 && blocked();++i)
			v.turn(1);
		if(i==9)
			return false;
	}
	move();
	return true;
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
bool creature::reproduce()
{
	if(energy >= reproductivetrigger)
	{
		creature* temp=new creature;		//because if I just declare a creature by value it will go out of scope
		if(temp->isType(identifier))		//just in case somthing goes horribly wrong
		{
			energy-=reproductivewaste;
			energy/=2;
			temp->y=y;
			temp->x=x;
			temp->energy=energy;
			return true;
		}
		else
			parseError.log("When reproduceing, " +identifier+" was unable to find prototype");
	}
	return false;
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
	energy=100;								//energy is used for special abilities and reproduceing
	properties=0;							//bitfield of counterdigestive properties
	appetite=0;								//bitfield of digestive abilities
	movetype=0;
	blockedby=0;
	digtype=0;
	bitesize=5;
	waste=0;
	photosynthesizes=false;
	player=false;
	response =0;
	hatchwhen=0;
	reproductivetrigger=200;
	reproductivewaste=0;
}
void creature::appear()
{
	if(legal(y,x))
	{
		if(!(properties & isegg))
			view[y][x]=appearance;
		else
			view[y][x]=',';
		if(properties & live)
			color[y][x]=hue;
		else
			color[y][x]=8;
	}
	else
		parseError.log("creature cordinants out of bounds");
}
