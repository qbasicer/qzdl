/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * 
 * qZDL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
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
	comment = "";
	
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
	if(comment.size() > 0){
		line.append("     ");
		line.append(comment);
	}
	parse();
	writes++;
	return 0;
}

int ZDLLine::findComment(char delim){
	string::size_type cloc = line.find(delim, line.size());
	if (cloc != string::npos){
		if (cloc > 0){
			if (line[cloc-1] != '\\'){
				return cloc;
			}
		}
	}
	return -1;
}

void ZDLLine::parse()
{
	int cloc = findComment(';');
	int cloc2 = findComment('#');
	if(cloc != -1 && cloc2 != -1){
		cloc = min(cloc,cloc2);
	}else if(cloc == -1 && cloc2 != -1){
		cloc = cloc2;
	}
	
	if(cloc != -1){
		//Strip out comment
		comment = chomp(line.substr(cloc, line.size()));
		
	}
	
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






