#ifndef TRIVECT
#define TRIVECT
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
	void setTo (int param);
	void polarize();
};
#endif