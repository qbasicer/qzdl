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

#ifdef QT_CORE_LIB
#include <QRegExp>
#endif

using namespace std;
#include <zdlcommon.h>

ZDLSection::ZDLSection(const char *name)
{
	//cout << "New section: \"" << name << "\"" << endl;
	reads = 0;
	writes = 0;
	sectionName = name;
}

ZDLSection::~ZDLSection()
{
	//cout << "Deleting section and it's children..." << endl;
	while (lines.size() > 0){
		ZDLLine *line = lines.front();
		lines.pop_front();
		delete line;
	}
	//cout << "Section deleted" << endl;
}

void ZDLSection::setSpecial(int inFlags)
{
	flags = inFlags;
}

int ZDLSection::hasVariable(const char* variable)
{
	reads++;
	list<ZDLLine*>::iterator itr;
	for (itr=lines.begin(); itr!=lines.end();itr++){
		ZDLLine* line = (*itr);
		if (strcmp(line->getVariable(), variable) == 0){
			return true;
		}
	}
	return false;
}

void ZDLSection::deleteVariable(const char* variable)
{
	reads++;
	list<ZDLLine*>::iterator itr;
	for (itr=lines.begin(); itr!=lines.end();itr++){
		ZDLLine* line = (*itr);
		if (strcmp(line->getVariable(), variable) == 0){
			lines.remove(line);
			delete line;
			return;
		}
	}
}

char* ZDLSection::findVariable(const char* variable)
{
	reads++;
	list<ZDLLine*>::iterator itr;
	for (itr=lines.begin(); itr!=lines.end();itr++){
		ZDLLine* line = (*itr);
		if (strcmp(line->getVariable(), variable) == 0){
			string rc = line->getValue();
			return (char*)rc.c_str();
		}
	}
	string rc = "";
	return (char*)rc.c_str();
}

int ZDLSection::getRegex(const char* regex, vector<ZDLLine*> &vctr){
#ifdef QT_CORE_LIB
	QRegExp rx(regex);
	list<ZDLLine*>::iterator itr;
	for (itr=lines.begin(); itr!=lines.end();itr++){
		ZDLLine* line = (*itr);
		if (rx.exactMatch(line->getVariable())){
			vctr.push_back(line);
		}
	}
	return vctr.size();
	
#endif
	return 0;
}

int ZDLSection::setValue(const char *variable, const char *value)
{
	writes++;
	list<ZDLLine*>::iterator itr;
	for (itr=lines.begin(); itr!=lines.end();itr++){
		ZDLLine* line = (*itr);
		if (strcmp(line->getVariable(), variable) == 0){
			line->setValue(value);
			return 0;
		}
	}
	//We can't find the line, create a new one.
	string buffer = variable;
	buffer.append("=");
	buffer.append(value);
	//cout << "Buffer: " << buffer << endl;
	ZDLLine *line = new ZDLLine((char*)buffer.c_str());
	lines.push_back(line);
	
	return 0;
}

int ZDLSection::streamWrite(ostream &stream)
{
	
	//Write only if we have stuff to write
	if (lines.size() > 0){
		writes++;
		//Global's don't have a section name
		if (sectionName.length() > 0){
			stream << "[" << sectionName << "]" << endl;
		}
		list<ZDLLine*>::iterator itr;
		for (itr=lines.begin(); itr!=lines.end();itr++){
			ZDLLine* line = (*itr);
			stream << line->getLine() << endl;
		}
	}
	return 0;
}

char* ZDLSection::getName()
{
	reads++;
	return (char*)sectionName.c_str();
}

ZDLLine *ZDLSection::findLine(const char *inVar)
{
	list<ZDLLine*>::iterator itr;
	for (itr=lines.begin(); itr!=lines.end();itr++){
		ZDLLine* line = (*itr);
		if (strcmp(line->getVariable(), inVar) == 0){
			return line;
		}
	}
	return NULL;

}

int ZDLSection::addLine(const char *linedata)
{
	writes++;
	ZDLLine *newl = new ZDLLine(linedata);
	ZDLLine *ptr = findLine(newl->getVariable());
	if (ptr == NULL){
		lines.push_back(newl);
		return 0;
	}else{
		//cerr << "ERROR: Duplicate variable " << sectionName << "#" << newl->getVariable() << endl;
		ptr->setValue(newl->getValue());
		delete newl;
		return 1;
	}
	
}









