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
	trit trit::operator = (int param) {v = param;if(v>1)v=1;if(v<-1)v=-1;return *this;} 
	int  trit::operator + (int param) {return v + param;}
	int  trit::operator - (int param) {return v - param;}
	bool trit::operator ==(int param) {return v==param;}
	int  trit::operator * (int param) {return v*param;}
		 trit::operator int(){return v;}
/***************************/
	void vect::swap() {trit temp=y;y=x;x=temp;}
	void vect::turn(int dir){ /*-1==left 1==right*/int tempY;tempY = y + x*dir;x = x - y*dir;y=tempY;}
	vect vect::operator * (vect param) {vect temp;temp.y=y*param.y;temp.x=x*param.x;return temp;}
	vect vect::operator + (vect param) {vect temp;temp.y=y+param.y;temp.x=x+param.x;return temp;}
	trit vect::operator < (vect param)
	{ /*if this is to the left of param*/
		vect temp=(*this);
		param.y=param.y*-1;
		param.swap();
		temp=param*temp;trit temp2;
		return temp2= (temp.y+temp.x);
	}
/***************************/
	tile::tile(){}
	bool tile::sameplace(int a,int b)
	{
		return (a==y && b==x);
	}
/***************************/
	void item::appear()
	{
		view[y][x]=appearance;
		color[y][x]=hue;
	}
	item::item(int a, int b,char c):tile()
	{
		set(a,b);
		appearance=c;
		hue=1;
	}
	void item::update(){}
	bool item::set(int a,int b)
	{
		if(legal(a,b))
		{
			for(list<item*>::iterator i=map[y][x].begin(); i != map[y][x].end(); i++)
				if((*i)==this)
					map[y][x].erase(i);
			map[a][b].push_front(this);
			y=a;
			x=b;
		}
	}
/***************************/
	void creature::die()
	{
		dead=true;
		hue=7;
	}
	creature::creature(char c):item(rand()%Y/2,rand()%X,c)
	{
		hp=5;
		speed=100;
		ap=0;
		energy=100;
		small=true;
		solid=true;
		translucent=false;
		dead=false;
		for(int i=0;i<10 && directionblocked();++i)
		{
			set(rand()%Y,rand()%X);
		}
	}
	/***********************/
	void creature::move()
	{
		int a=v.y+y;
		int b=v.x+x;
		for(list<creature*>::const_iterator i=monsterlist.begin();i !=monsterlist.end();i++)
			if((*i)->sameplace(a,b) && (*i)!=this && (*i)->small!=true)
				return;
		if(legal(a,b) && !directionblocked())
		{
			set(a,b);
		}
	}
	/***********************/
	int creature::directionblocked()
	{
		int a=v.y+y;
		int b=v.x+x;
		for(list<creature*>::const_iterator i=monsterlist.begin();i !=monsterlist.end();i++)
			if((*i)->sameplace(a,b) && (*i)!=this && !(*i)->small)
				return 1;
		if(wall[a][b] && solid)  return 2;
		if(!legal(a,b)) return 3;
		return 0;
	}
	/***********************/
	void creature::update()
	{
		choosemove1(1);
		avoidobstacles(4);
		move();
	}
	/***********************/
	void creature::choosemove1(int a)
	{
		if(rand()%a==0)
		{
			v.y= v.y+(rand()%3-1);
			v.x= v.y+(rand()%3-1);
		}
	}
	/***********************/
	bool creature::dig()
	{
		int a=y+v.y;
		int b=x+v.x;
		if(legal(a,b) && wall[a][b])
		{
			wall[a][b]=0;
			return true;
		}
		return false;
	}
	/***********************/
	void creature::move2(int a)
	{
		if(rand()%a==0)
			v.turn(rand()%2*2-1);
	}
	/***********************/
	bool creature::avoidobstacles(int b) //b=how far they can turn
	{
		if(b>4)b=4;
		if(b<0)b=0;
		if(directionblocked())
		{
			int a=rand()%2*2-1;
			vect left,right,old;
			old=v;
			left=v;	//abstractly left, it could actually be either direction
			right=v;	//opposite of 'left'
			for(int i=0;i<b;++i)
			{
				right.turn(a);
				left.turn(-a);
				v=right;
				if(!directionblocked())
					return true;
				v=left;
				if(!directionblocked())
					return true;
			}
			v=old;
			return false;
		}
		return true;
	}
	void creature::seek(int a, int b, int c){v.y=(a-y)*c;v.x=(b-x)*c;}//c is  -1 to 1
	void creature::attacked (creature* attacker) {}
	void creature::attack(creature* victim) {victim->attacked(this);}
	bool creature::eat()
	{
		int a,b;
		for(int i=-1;i<2;++i)
			for(int j=-1;j<2;++j)
			{
				a=y+i;
				b=x+j;
				for(list<creature*>::const_iterator i=monsterlist.begin();i !=monsterlist.end();i++)
				{
					if((*i)->sameplace(a,b) && (*i)!=this)
					{
						for(list<string>::const_iterator j=edible.begin();j !=edible.end();j++)
							if((*i)->Cname==(*j))
							{
								(*i)->energy-=5;
								energy+=5;
								(*i)->attacked(this);
								return true;
							}
					}
				}
			}
	}
	/***********************/
	void creature::run(int distance)
	{
		for(int i=0;i<distance;i++)
		{
			avoidobstacles(3);
			move();
		}
	}
	bool creature::reproduce(){}
/***************************/
	me::me(): creature('@')
	{
		Cname="you";
		solid=false;
	}
	void me::update()
	{
		energy++;
		inchar = char(getch());
		if(inchar != ERR)
		{
			v.y=0;
			v.x=0;
				if(inchar=='w' ||inchar=='7'||inchar=='8'||inchar=='9')
					v.y=-1;
				if(inchar=='s'||inchar=='1'||inchar=='2'||inchar=='3')
					v.y=1;
				if(inchar=='a'||inchar=='7'||inchar=='4'||inchar=='1')
					v.x=-1;
				if(inchar=='d'||inchar=='3'||inchar=='6'||inchar=='9')
					v.x=1;
				else if(inchar==' ')
					wall[y][x]^=1;
				else if(inchar=='S')
					monsterlist.push_front(new slime);	//create some food
				else if(inchar=='C')
					monsterlist.push_front(new cube);	//creates some gelatinous cubes
				else if(inchar=='M')
					monsterlist.push_front(new mole);	//create some apex predators
				else if(inchar=='H')
					monsterlist.push_front(new crab);	//create some space
				else if(inchar=='L')
					monsterlist.push_front(new larva);	//create some tunnels
				else if(inchar=='D')
					monsterlist.push_front(new dwarf);	//create some tunnels
//				avoidobstacles(1);
				move();
		}
	}
	void me::attacked(creature* attacker){}
/***************************/	
	cube::cube():creature('C')
	{
		Cname="Gelatinous Cube";
		v.y=rand()%2*2-1;
		v.x=rand()%2*2-1;
		speed=50;
		translucent=true;
		edible.push_front("Wall slime");
		edible.push_front("Hammer Crab");
	}
	void cube::attacked (creature* attacker)
	{
		v.y=y-attacker->y;//face away from the attacker
		v.x=x-attacker->x;
		avoidobstacles(3);	//only turn a little so that you don't approch the preditor
		if((wall[y][x-1]==0 && wall[y][x+1]==0) || (wall[y-1][x]==0 && wall[y+1][x]==0))
			wall[y][x]=1;	//place a wall
		move();			//and run
	}
	int cube::directionblocked()
	{
		int a=v.y+y;
		int b=v.x+x;
		for(list<creature*>::const_iterator i=monsterlist.begin();i !=monsterlist.end();i++)
			if((*i)->sameplace(a,b) && (*i)!=this && !(*i)->small)
				return true;
		if(legal(a,b)&& !wall[a][b])
		{
			if(wall[a-1][b] || wall[a+1][b] ||wall[a][b-1] || wall[a][b+1])
				return false;
		}
		return true;
	}
	void cube::update()
	{
		eat();
		reproduce();
		move2(3);
		avoidobstacles(3);
		move();
	}
	bool cube::reproduce()
	{
		if(energy>300)
		{
			monsterlist.push_front(new cube);
			list<creature*>::const_iterator i=monsterlist.begin();
			(*i)->set(y,x);
			energy-=200;
		}
	}
/***************************/
	larva::larva():creature('~')
	{
		Cname="Larva";
		hp=20;
		hue=5;
	}
	void larva::attacked (creature* attacker) {}
	int larva::directionblocked()
	{
		int a=v.y+y;
		int b=v.x+x;
		if(!legal(a,a))
			return true;
		for(list<creature*>::const_iterator i=monsterlist.begin();i !=monsterlist.end();i++)
			if((*i)->sameplace(a,b) && (*i)!=this && !(*i)->small)
				return 1;
		if(wall[a][b] && v.y*v.x != 0)
			return true;
		if(wall[a][b]) //you can only remove walls when you are moveing orthagonally
		{
			for(int i=-1;i<2;++i)
			{
				for(int j=-1;j<2;++j)
				{
					if((i*v.y)+(j*v.x)>-1 && wall[a+i][b+j]==0)//make them dig mazes
						return true;
				}
			}
		}
		return false;
	}
	void larva::update()
	{
		choosemove1(1);
		eat();
		move();
	}
	bool larva::eat()
	{
		if(wall[y+v.y][x+v.x]&&!directionblocked())
		{
			wall[y+v.y][x+v.x]=0;
			energy+=100;
			return true;
		}
		return false;
	}
/***************************/
	crab::crab():creature('c')
	{
		Cname="Hammer Crab";
		hue=6;
	}
	int crab::directionblocked()
	{
		int a=v.y+y;
		int b=v.x+x;
		if(v.y*v.x!=0 || !legal(a,a))
			return true;
		for(list<creature*>::const_iterator i=monsterlist.begin();i !=monsterlist.end();i++)
			if((*i)->sameplace(a,b) && (*i)!=this && !(*i)->small)
				return 1;
		if(wall[a][b]==1)
		{
			if((wall[a][b+1]+wall[a][b-1]+wall[a+1][b]+wall[a-1][b])>1)//make them remove blocks that stick out
				return true;
		}
		return false;
	}
	void crab::update(){choosemove1(2);avoidobstacles(3);move();wall[y][x]=0;}
/***************************/
	mole::mole():creature('m')
	{
		Cname="Mole";
		hue=3;//brown
		edible.push_front("Gelatinous Cube");
		edible.push_front("Mole");
	}
	int mole::directionblocked() //it eats cubes
	{
		int a=v.y+y;
		int b=v.x+x;
		if(!legal(a,b))
			return true;
		for(list<creature*>::const_iterator i=monsterlist.begin();i !=monsterlist.end();i++)
			if((*i)->sameplace(a,b) && (*i)!=this && (*i)->small!=true)
				return 1;
		if(wall[a][b]==1)
		{
			if(wall[a+v.y][b]==0 && wall[a][b+1]==1 && wall[a][b-1]==1)//make them burst through thin walls
				return false;
			if(wall[a][b+v.x]==0 && wall[a+1][b]==1 && wall[a-1][b]==1)//make them burst through thin walls
				return false;
			return true;
		}
		return false;
	}
	void mole::update()
	{
		eat();
		reproduce();
		choosemove1(3);
		avoidobstacles(3);
		move();
		wall[y][x]=0;
	}
	bool mole::reproduce()
	{
/*		if(energy>800)
		{
			monsterlist.push_front(new mole);
			list<creature*>::const_iterator i=monsterlist.begin();
			set(y,x);
			(*i)->energy=200;
			energy-=500;
		}
*/	}
/***************************/
	dwarf::dwarf():creature('d')
	{
		Cname="Dwarf";
		hue=3;
		diglength=0;
		v.y=rand()%2*2-1;
		v.x=0;
		solid=false;
	}
	void dwarf::update()
	{
		if(diglength==0)
			diglength=pow(2,rand()%6);
		if(diglength==1 || directionblocked())
		{
			int a;
			do{
				a=rand()%2*2-1;
				v.turn(a);
				v.turn(a);
			}while(directionblocked());
		}

		wall[y][x]=0;
		move();
		--diglength;
	}
/***************************/