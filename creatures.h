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
	int hp,actionpoints,energy;					//energy is used for special abilities and reproduceing
	int properties;								//bitfield of counterdigestive properties
	int appetite;								//bitfield of digestive abilities
	bool isType(string symbol);
	void update();
	bool move();
	int movetype;
	void choosemove(int b);				//function pointers got too complicated
		void moveNone(int prob);		//none
		void moveInertial(int prob);	//inertial
		void moveSnake(int prob);		//slithery
		void moveManual(int prob);		//manual
	int blockedby;						//bitfield of disallowed moves
	bool blocked();
		bool blockedbyWalls();
		bool adjacenttoWalls();
	int digtype;
	bool dig();
		bool digmaze();
		bool digwall();
		bool digexposed();
	void transformTo(creature* master);
	creature();
	void appear();
//	int blocked();
};
#endif