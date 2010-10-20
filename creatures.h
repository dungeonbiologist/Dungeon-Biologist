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
/*
//things that make a creature more difficult to eat
#define live	(1 << 0)
#define egg		(1 << 1)
#define moveing	(1 << 2)	//not paralyzed
#define stone	(1 << 3)
#define plant	(1 << 4)
#define animal	(1 << 5)
#define yucky	(1 << 6)
*/
struct tile
{
	tile();
	int y,x;
	virtual bool sameplace(int a,int b);
};
struct item:tile
{
	bool small; //doesn't block creatures
	vect v;
	string Cname;
	item* heldby;
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
	int hp;
	int ap; //action points
	int defenses;	//bitfield of defensive properties
	int overcome;	//bitfield of overcomeing those defensive properties
	int speed;
	list <string> edible;
	list <item*> held;
	list <tile> memory;
	int energy;	//uses for special abilities and reproduceing
	bool solid;	//can't walk through walls
	bool translucent;	//tints any tiles that you see past it
	bool dead;
	bool egg; //if this is an egg
	bool eggeater; //it can eat any type of eggs
	virtual void update();
	virtual void die();
	virtual bool move();
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
	virtual bool pickup(string itemname);
	virtual bool drop();
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
	virtual bool reproduce();
};
struct mole: creature
{
	mole();
	virtual int directionblocked();
	virtual void update();
	virtual bool reproduce();
};
struct trantoro: creature
{
	trantoro();
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