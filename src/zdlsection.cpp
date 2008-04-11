#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;
#include <zdlcommon.h>

ZDLSection::ZDLSection(char *name)
{
	cout << "New section: \"" << name << "\"" << endl;
	reads = 0;
	writes = 0;
	sectionName = name;
}

ZDLSection::~ZDLSection()
{
	cout << "Deleting section and it's children..." << endl;
	while (lines.size() > 0){
		ZDLLine *line = lines.front();
		lines.pop_front();
		delete line;
	}
	cout << "Section deleted" << endl;
}

void ZDLSection::setSpecial(int inFlags)
{
	flags = inFlags;
}

int ZDLSection::hasVariable(char* variable)
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

void ZDLSection::deleteVariable(char* variable)
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

char* ZDLSection::findVariable(char* variable)
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

int ZDLSection::setValue(char *variable, const char *value)
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
	ZDLLine *line = new ZDLLine((char*)buffer.c_str());
	lines.push_back(line);
	return 0;
}

int ZDLSection::streamWrite(ostream &stream)
{
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
	return 0;
}

char* ZDLSection::getName()
{
	reads++;
	return (char*)sectionName.c_str();
}

ZDLLine *ZDLSection::findLine(char *inVar)
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

int ZDLSection::addLine(char *linedata)
{
	writes++;
	ZDLLine *newl = new ZDLLine(linedata);
	ZDLLine *ptr = findLine(newl->getVariable());
	if (ptr == NULL){
		lines.push_back(newl);
	}else{
		cerr << "ERROR: Duplicate variable " << sectionName << "#" << newl->getVariable() << endl;
		ptr->setValue(newl->getValue());
		delete newl;
	}
}









