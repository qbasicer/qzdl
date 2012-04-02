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
#if 0 
#include <iostream>
#include <list>
#include <string>
#include <cstring>
#include <cstdlib>
using namespace std;
#include <zdlcommon.h>

extern char* chomp(string in);

/* There are currently two sections which are currently off limits for other programs
 * They are:
 *   o env  [Grabs the current environment variable]
 *   o sys  [Internal settings                     ]
 * 
 * There are some restricted characters due to the variable resolving. 
 * They are:
 *   o #    [The hash (#) is used to delimit,      ]
 *          [sections and variables, and may not be]
 *          [used in any section name or variable  ]
 *          [name in which another variable may    ]
 *          [point at it.                          ]
 */


int ZDLVariables::trendBytes;
int ZDLVariables::trendNum;

ZDLVariables::ZDLVariables (ZDLConf *parent)
{
	_parent = parent;
	depth = 0;
	blank = "";
	trendNum = 0;
	trendBytes = 0;
}

ZDLVariables::ZDLVariables (ZDLConf *parent, int flags)
{
	_parent = parent;
	_flags = flags;
	depth = 0;
	blank = "";
	trendNum = 0;
	trendBytes = 0;
}

ZDLVariables::~ZDLVariables ()
{
}

int ZDLVariables::hasVariableInternal(const char *lsection, const char *variable, int* status)
{
	*status = 0;
	//Handle environment requests differently
	if (strcmp("env", lsection) == 0){
		return true;
		
	}

	//Handle system variables differently
	if (strcmp("sys", lsection) == 0){
		if (strcmp("VERSION", variable) == 0){
			return true;
		}
		return false;
	}

	for(int i = 0; i < _parent->sections.size(); i++){
		ZDLSection* section = _parent->sections[i];
		if (section->getName().compare(lsection) == 0){
			return section->hasVariable(variable);
		}
	}
	return false;
}

const char *ZDLVariables::getVariableInternal(const char *lsection, const char *variable, int* status)
{
	*status = 0;
	int cdepth = depth;
	/* This mechanism prevents a stack overflow by recursing down too far */
	if (++depth > 16){
		depth--;
		//cerr << "[error]Overflow bailing" << endl;
		return (char*)blank.c_str();
	}
	//Handle environment requests differently
	if (strcmp("env", lsection) == 0){
		char *env = getenv(variable);
		if (env){
			return env;
		}else{
			return (char*)blank.c_str();
		}
		
	}

	//Handle system variables differently
	if (strcmp("sys", lsection) == 0){
		if (strcmp("VERSION", variable) == 0){
			return ZDL_VERSION_STRING;
		}
		return (char*)blank.c_str();
	}

	list<ZDLSection*>::iterator itr;
	for (itr = (_parent->sections).begin(); itr != (_parent->sections).end();itr++){
		ZDLSection* section = (*itr);
		if (section->getName().compare(lsection) == 0){
			string rc = section->findVariable(variable).toStdString();
			while (containsUnresolved((char*)rc.c_str())){
				rc = resolveVariable((char*)rc.c_str());
			}
			if (cdepth == 0){
				depth = 0;
				rc = demangle((char*)rc.c_str());
			}else{depth--;}
			return (char*)rc.c_str();
		}
	}
	if (cdepth == 0){depth = 0;}else{depth--;}
	return (char*)blank.c_str();
}
int ZDLVariables::getVariableStart(const char *inBuffer)
{
	string buffer(inBuffer);
	string::size_type loc = buffer.find("${", 0);
	if (loc != string::npos){
		return loc;
	}else{
		return 0;
	}

}

int ZDLVariables::demangleHelper(const char *inBuffer)
{
	string buffer(inBuffer);
	string::size_type loc = buffer.find("$\\{", 0);
	if (loc != string::npos){
		return loc;
	}
	return -1;
}

char *ZDLVariables::demangle(const char *inBuffer)
{
	string buffer(inBuffer);
	int pos = demangleHelper((char*)buffer.c_str());
	while (pos >= 0){
		buffer.replace(pos, 3, "${");
		pos = demangleHelper((char*)buffer.c_str());
	}
	return (char*)buffer.c_str();
}

int ZDLVariables::getVariableEnd(const char *inBuffer)
{
	string buffer(inBuffer);
	for (unsigned int i = getVariableStart(inBuffer); i < buffer.length(); i++){
		if (buffer[i] == '}'){
			return i;
		}
	}
	//Cannot find end of variable
	return 0;
}

char *ZDLVariables::resolveVariable(const char *inBuffer)
{
	string buffer(inBuffer);
	/* We use this to remove our varible from our string */
	int varStart = getVariableStart((char*)buffer.c_str());
	int varEnd = getVariableEnd((char*)buffer.c_str());
	
	string varName = buffer.substr(varStart + 2, varEnd - varStart - 2);

	/* Now we have the variable name */
	string cVarName = chomp(varName);
	string::size_type loc = cVarName.find("#", 0);
	if (loc != string::npos){
		int stat = 0;
		string section = cVarName.substr(0, loc);
		string variable = cVarName.substr(loc + 1, cVarName.length() - loc);
		string value = getVariable((char*)section.c_str(), (char*)variable.c_str(), &stat);
		buffer.replace(varStart, varEnd - varStart + 1, value);
		return (char*)buffer.c_str();
	}else{
		//cerr << "[ERROR]Invalid notation (${section#variable})" << endl;
		return (char*)blank.c_str();
	}
}

int ZDLVariables::containsUnresolved(const char *inBuffer)
{
	string built(inBuffer);
	string::size_type loc = built.find("${", 0);
	if (loc != string::npos){
		return 1;
	}else{
		return 0;
	}
}
#endif
