#include <list>
using namespace std;
#ifndef PLANTS
struct plant: creature
{
	plant(char c);
	bool seed;
	virtual bool sprout();
	virtual void appear();
};
struct slime: plant
{
	slime();
	virtual void update();
	virtual int directionblocked();
	virtual bool reproduce();
};
#define PLANTS
#endif
