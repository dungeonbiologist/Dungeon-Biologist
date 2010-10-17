#include <cstdlib>
#include <curses.h>
#include <iostream>
#include <list>
#include "creatures.h"
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
	bool tile::sameplace(int a,int b) {return (a==y && b==x);}
/***************************/
	void item::appear()
	{
		view[y][x]=appearance;
		color[y][x]=hue;
	}
	void item::die(){}
	item::item(int a, int b,char c):tile()
	{
		y=a;
		x=b;
		appearance=c;
		hue=1;
	}
/***************************/
	creature::creature(char c):item(Y/2,rand()%X,c)
	{
		hp=5;
		speed=100;
		energy=0;
		small=false;
		solid=true;
		translucent=false;
	}
	/***********************/
	void creature::move()
	{
		int a=v.y+y;
		int b=v.x+x;
		if(legal(a,b) && !directionblocked())
		{
			y=a;
			x=b;
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
	void creature::act()
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
			right=v;
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
	/***********************/
	void creature::run(int distance)
	{
		for(int i=0;i<distance;i++)
		{
			avoidobstacles(3);
			move();
		}
	}
/***************************/
	void me::act()
	{
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
//				avoidobstacles(1);
				move();
		}
	}
	me::me(): creature('@'){solid=false;}
	void me::attacked(creature* attacker){}
/***************************/	
	cube::cube():creature('C')
	{
		v.y=rand()%2*2-1;
		v.x=rand()%2*2-1;
		speed=50;
		translucent=true;
	}
	void cube::attacked (creature* attacker)
	{
		v.y=y-attacker->y;//face away from the attacker
		v.x=x-attacker->x;
		wall[y][x]=1;	//place a wall
		run(1);			//and run
	}
	int cube::directionblocked()
	{
		int a=v.y+y;
		int b=v.x+x;
		if(legal(a,b)&& wall[a][b])
		{
			if(wall[a-1][b] || wall[a+1][b] ||wall[a][b-1] || wall[a][b+1])
				return false;
		}
		return true;
	}
	void cube::act(){move2(3);avoidobstacles(3);move();}
/***************************/
	larva::larva():creature('~'){hp=20;}
	void larva::attacked (creature* attacker) {}
	int larva::directionblocked()
	{
		int a=v.y+y;
		int b=v.x+x;
		if(v.y*v.x!=0 || !legal(a,a))
			return true;
		if(wall[a][b]==1)
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
	void larva::act()
	{
		choosemove1(1);
		move();
		wall[y][x]=0;
	}
/***************************/
	crab::crab():creature('c'){}
	int crab::directionblocked()
	{
		int a=v.y+y;
		int b=v.x+x;
		if(v.y*v.x!=0 || !legal(a,a))
			return true;
		if(wall[a][b]==1)
		{
			if((wall[a][b+1]+wall[a][b-1]+wall[a+1][b]+wall[a-1][b])>1)//make them remove blocks that stick out
				return true;
		}
		return false;
	}
	void crab::act(){choosemove1(2);avoidobstacles(3);move();wall[y][x]=0;}
/***************************/
	mole::mole():creature('m'){}
	int mole::directionblocked() //it eats cubes
	{
		int a=v.y+y;
		int b=v.x+x;
		if(!legal(a,b))
			return true;
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
	void mole::act()
	{
		choosemove1(3);
		avoidobstacles(3);
		move();
		wall[y][x]=0;
	}
/***************************/