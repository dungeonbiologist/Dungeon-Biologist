#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "creatures.h"
#include "parser.h"
using namespace std;
/***************************/
errorfile parseError;
/***************************/
void parse(string file)						//this may not be the best parser but it's the first way I thought of doing it
{
	std::ifstream fseed(file.c_str());		//convert "file" into the format ifstream wants and open the file
	if(fseed.is_open())
	{
		string str;
		creature* victim=NULL;
		for (int i=0; !fseed.eof(); i++)
		{
			fseed.ignore(10000,'[');		//ignore everthing outside of the brackets
			getline(fseed,str,']');
			victim=tokenize(str,victim);	//find out what this word actually means and do it
			str.clear();					//empty the string
		}
	}
	else
		parseError.log("Unable to find file: "+ file);
	fseed.close();
}
creature* tokenize(string letters,creature* victim)
{
	if(letters.size()<=0)
		return victim;
	int colon =letters.find(':');
	int underscore =letters.find('_');
	string first=letters.substr(0,colon);
	string second=letters.substr(colon+1,letters.npos);
	if(first=="CREATURE")
	{
		if(second.length()==0 || colon > letters.length())
		{
			parseError.log("Expected an identifier after CREATURE");
			return NULL;
		}
		victim=new creature;
		victim->identifier=second;
		prototypes.push_back(victim);
	}
	else if(victim==NULL)
		parseError.log("symbol: "+ letters +" outside of creature definition");
	else if(first=="DEFAULT")
	{
		string temp = victim->identifier;
		list<creature*>::iterator i;
		for(i=prototypes.begin();i !=prototypes.end();i++)
		{
			if((*i)->identifier==second);
				break;
		}
		//later have it continue scanning the document to look for the definition
		if(i==prototypes.end())
			parseError.log("Creature "+ second +" needed for definition of "+ victim->identifier +" was not found");
		else
		{
			victim->transformTo(*i);
			victim->identifier=temp;
		}
	}
	else if(first=="NAME")
		victim->Cname=second;
	else if(first=="TILE")
	{
		victim->appearance=second[0];
		if(second.length()!=1)
			parseError.log("expected single letter for TILE in " + victim->identifier + " definition");
	}
	else if(first=="COLOR")
	{
		int i;
		string colors[]={"BLACK","WHITE","RED","YELLOW","GREEN","CYAN","BLUE","MAGENTA",""};
		for(i=0;i<8;++i)
			if(second==colors[i])		//turn the name into a number
				break;
		if(i>7)
			parseError.log("In creature definition: "+ victim->identifier+", "+second+" is not a color");
		else
			victim->hue=i;
	}
	else if(first=="SPEED")
		victim->speed=atoi(second.c_str());			//string to integer 
	else if(first=="MOVE")
	{
		string movetypes[]={"NONE","NORMAL","SNAKE","MANUAL",""};
		int i;
		for(i=0;i<5;++i)
			if(second==movetypes[i])		//turn the name into a number
				break;
		if(i>4)
			parseError.log("In creature definition: "+ victim->identifier+", "+second+" is not a type of move");
		else
		{
			victim->movetype = i;	//assign a function to the function pointer
		}
	}
	else if(letters.find("EATS_") < letters.length())			//parse the properties of this creature , is it a plant? an animal?
	{
		string property =letters.substr(underscore+1,letters.npos);					//find rest of symbol
		victim->appetite = changeProp(victim->appetite,false,property);	//update properties accordingly
	}
	else if(letters.find("CAN'T_EAT_") < letters.length())			//parse the properties of this creature , is it a plant? an animal?
	{
		int canteat=letters.find("CAN'T_EAT_") < letters.length();
		string property =letters.substr(canteat+9,letters.npos);		//find rest of symbol, removeing the first 9 since they = "can't"
		victim->appetite = changeProp(victim->appetite,true,property);	//update properties accordingly
	}
	else if(underscore < letters.length())			//parse the properties of this creature , is it a plant? an animal?
	{
		bool NOT=letters.find("NOT_") < letters.length();						//is there a NOT- ?
		string property =letters.substr(underscore+1,letters.npos);					//find rest of symbol
		victim->properties = changeProp(victim->properties,NOT,property);	//update properties accordingly
	}
	else if(letters.find("SOLID") < letters.length())
		victim->blockedby |= 1;
	else if(letters.find("STICKS TO WALLS") < letters.length())
		victim->blockedby |= 2;
	else if(letters.find("NOT PUSHY") < letters.length())
		victim->blockedby |= 4;
	else if(letters.find("EROSIVE") < letters.length())
		victim->digtype = 1;
	else if(letters.find("COOLAID MAN") < letters.length())
		victim->digtype = 2;
	else if(letters.find("MAZE DIGGER") < letters.length())
		victim->digtype = 3;
	else if(letters.find("PHOTOSYNTHESIZES") < letters.length())
		victim->photosynthesizes=true;
	else if(letters.find("PLAYER") < letters.length())
		victim->player=true;
	else if(letters.find("CALCIFEROUS") < letters.length())
		victim->response=1;
	else if(letters.find("EGG SCATTER") < letters.length())
		victim->hatchwhen=1;
	else
		parseError.log("unknown symbol: "+ letters + " in creature definition: "+ victim->identifier);
	return victim;
}
/***************************/
int changeProp(int properties, bool NOT, string symbol)
{
	int property=0;				//just to be safe
	if(symbol=="PLANT")
		property=isplant;		//macro defined in creatures.h
	else if(symbol=="YUCKY")
		property=yucky;			//macro defined in creatures.h
	else if(symbol=="ANIMAL")
		property=animal;		//macro defined in creatures.h
	else if(symbol=="ALIVE")
		property=live;			//macro defined in creatures.h	
	else if(symbol=="EGG")
		property=isegg;			//macro defined in creatures.h
	else if(symbol=="MOVES")
		property=moveing;		//macro defined in creatures.h
	else if(symbol=="STONE")
		property=stone;			//macro defined in creatures.h
	else if(symbol=="IMMATERIAL")
		property=ghostly;		//macro defined in creatures.h
	else if(symbol=="LARGE")
		property=large;			//macro defined in creatures.h
	else
	{
		if(NOT)
			parseError.log("unknown symbol: NOT_" + symbol);
		parseError.log("unknown symbol: " + symbol);
	}
	if(NOT)
		properties &= ~property;	//remove that bit from the bitstring
	else
		properties |= property;		//add that bit to the bitstring
	return properties;
}
/***************************/