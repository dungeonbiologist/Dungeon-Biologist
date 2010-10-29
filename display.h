#ifndef DISPLAY
#define DISPLAY
	void print();
	void init();
	int abs(int a);
	void printtext(int a, int b, string text);
	void printcentered(int a, string text);
	char* convert(int number, char* pstring);
	void printNumber(int a, int b, int number);
	void swap(int &a,int &b);
	void bresenham(int y0,int y1,int x0,int x1);
	struct Messages
	{
		bool displayhelp;
		string message;
		void print(){ printcentered(0,message);}
		void help();
	};
	#ifdef WIN32				//on windows the tiles are square,
		const int aspect=1;		//if the tile height matches width
	#else
		const int aspect=2;		//if the tiles are twice as tall as they are wide
#endif
#endif