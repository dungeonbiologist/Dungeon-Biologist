#include <list>
using namespace std;
#ifndef CREATURES
#define CREATURES
/***************************/
struct trit
{
	int v;
	trit operator = (int param); 
	int operator + (int param);
	int operator - (int param);
	bool operator == (int param);
	int operator * (int param);
	operator int();
};
struct vect
{
	trit y,x;
	void swap();
	void turn(int dir);
	trit operator == (vect param);
	vect operator * (vect param);
	vect operator + (vect param);
	trit operator < (vect param);
	trit operator > (vect param);
};
struct tile
{
	tile();
	int y,x;
	virtual bool sameplace(int a,int b);
};
struct item:tile
{
	vect v;
	char appearance;
	int hue;
	virtual void update();
	virtual void appear();
	item(int a, int b,char c);
	virtual bool set(int a,int b);
};
struct creature:item
{
	creature(char c);
	int hp,ap; //action points
	int speed;
	string Cname;
	list <string> edible;
	list <item*> held;
	list <tile> memory;
	int energy;	//uses for special abilities and reproduceing
	bool small; //doesn't block other creatures
	bool solid;	//can't walk through walls
	bool translucent;	//tints any tiles that you see past it
	bool dead;
	bool egg; //if this is an egg
	bool eggeater; //it can eat any type of eggs
	virtual void update();
	virtual void die();
	virtual void move();
	virtual int directionblocked();
	virtual void choosemove1(int a);
	virtual bool dig();
	virtual void move2(int a);
	virtual bool avoidobstacles(int b);
	virtual void seek(int a, int b, int c);
	virtual void attacked (creature* attacker);
	virtual void attack(creature* victim);
	virtual bool eat();
	virtual void run(int distance);
	virtual bool reproduce();
};
/***************************/
struct me: creature
{
	virtual void update();
	me();
	virtual void attacked(creature* attacker);
};
struct cube: creature
{
	cube();
	virtual void attacked (creature* attacker);
	virtual int directionblocked();
	virtual void update();
	virtual bool reproduce();
};
struct larva: creature
{
	larva();
	virtual void attacked (creature* attacker);
	virtual int directionblocked();
	virtual void update();
	virtual bool eat();
};
struct crab: creature
{
	crab();
	virtual int directionblocked();
	virtual void update();
};
struct mole: creature
{
	mole();
	virtual int directionblocked();
	virtual void update();
	virtual bool reproduce();
};
struct dwarf: creature
{
	int diglength;
	dwarf();
	virtual void update();
};
/***************************/
#endif