#ifndef DISPLAY
#define DISPLAY
	void print();
	void init();
	int abs(int a);
	void printtext(int a, int b, char pstring[]);
	void printcentered(int a, char pstring[]);
	char* convert(int number, char* pstring);
	void printNumber(int a, int b, int number);
	void swap(int &a,int &b);
	void bresenham(int y0,int y1,int x0,int x1);
#endif