#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;
#include <zdlcommon.h>

extern char* chomp(string in);

ZDLLine::ZDLLine(const char *inLine)
{
	reads = 0;
	writes = 1;
	line = chomp(inLine);
	if (line[0] == ';' || line[0] == '#'){
		type = 2;
	}else{
		parse();
	}
	slashConvert = false;
	
}

ZDLLine::~ZDLLine()
{
}

const char* ZDLLine::getValue()
{
	return (char*)value.c_str();
	reads++;
}

const char* ZDLLine::getVariable()
{
	return (char*)variable.c_str();
	reads++;
}

const char* ZDLLine::getLine()
{
	return (char*)line.c_str();
	reads++;
}

int ZDLLine::setValue(const char *inValue)
{
	line = "";
	line.append(variable);
	line.append("=");
	line.append(inValue);
	parse();
	writes++;
	return 0;
}

void ZDLLine::parse()
{
	string::size_type loc = line.find("=", 0);
	if (loc != string::npos){
		variable = chomp(line.substr(0, loc));
		value = chomp(line.substr(loc+1, line.length() - loc - 1));
		//This is important for cross platform
		//Currently disabled
		if (slashConvert == true){
		}
		type = 0;
	}else{
		type = 1;
		variable = line;
		value = "";
	}

}






