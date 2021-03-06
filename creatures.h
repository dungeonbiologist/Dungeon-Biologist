#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "trivect.h"
using namespace std;
#ifndef CREATURES
#define CREATURES
/***************************/
//things that make a creature more difficult to eat
#define live	(1 << 0)
#define isegg	(1 << 1)
#define moveing	(1 << 2)	//not paralyzed
#define stone	(1 << 3)
#define isplant	(1 << 4)
#define animal	(1 << 5)
#define yucky	(1 << 6)
#define ghostly	(1 << 7)	//can walk through walls
#define large	(1 << 8)
#define active	(1 << 9)	//metabolically active
/***************************/
struct creature
{
	string identifier;
	int y,x;
	vect v;
	string Cname;
	char appearance;
	int hue,speed;
	int hp,actionpoints,energy;			//energy is used for special abilities and reproduceing
	
	int properties;						//bitfield of counterdigestive properties
	int appetite;						//bitfield of digestive abilities
	int bitesize;						//how much energy they consume per eating attempt
	int waste;							//how much energy gets lost in the transfer from the prey
	bool photosynthesizes;
	bool player;
	bool eat();
	
	void transformTo(creature* master);
	int reproductivetrigger;
	int reproductivewaste;
	bool reproduce();

	bool isType(string symbol);
	void update();
	
	int response;						//response to attack
	void attacked(creature* agressor);
		void calcify(creature* agressor);
		
	bool move();
	int movetype;
	void choosemove(int b);				//function pointers got too complicated
		void moveNone(int prob);		//none
		void moveInertial(int prob);	//inertial
		void moveSnake(int prob);		//slithery
		void moveManual(int prob);		//manual
		
	bool avoidobstacles(int angle);
	int blockedby;						//bitfield of disallowed moves
	bool blocked();
		bool blockedbyWalls();
		bool adjacenttoWalls();
		bool blockedByCreatures();
		
	int digtype;
	bool dig();
		bool digmaze();
		bool digwall();
		bool digexposed();
		
	int hatchwhen;
	bool hatch();
		bool inToTile();
		
	creature();
	void appear();
//	int blocked();
};
#endif