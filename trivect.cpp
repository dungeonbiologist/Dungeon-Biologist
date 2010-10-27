#include <cstdlib>
#include "trivect.h"
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
void vect::setTo (int param)
{	//0 1 or 2
	if(param==0)
	{
		y=0;
		x=0;
	}
	else if(param==1)
	{
		y=0;
		x=rand()%2*2-1;
		if(rand()%2)
			swap();
	}
	else if(param==2)
	{
		y=rand()%2*2-1;
		x=rand()%2*2-1;
	}
}
void vect::polarize()
{
	if(rand()%2)
		setTo(1);
	else
		setTo(2);
}