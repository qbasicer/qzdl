#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cstring>



using namespace std;
#include <zdlcommon.h>

extern char* chomp(string in);

/* ZDLConf.cpp
 *    Author: Cody Harris
 *   Purpose: To load in a configuration file, and parse it into sections and lines
 * Rationale: The current implementation of configuration files stores the values
 *            in a system which forgets keys and comments it doesn't know about.
 *            This system caches everything, and makes changes on the fly so that
 *            config files can be written back with keys it doesn't know how to use.
 *      Date: July 29th, 2007
 */



int ZDLConf::readINI(const char* file)
{
	reads++;
	string line;
	/* We allow lines to be outside of any section (ie header comments)
	 * We use a global section to read this.  We also keep track of
	 * which section we're in.  We do that with a pointer (current)
	 */
	ZDLSection *current = new ZDLSection("");
	current->setSpecial(ZDL_FLAG_NAMELESS);
	sections.push_back(current);
	ifstream stream(file);
	if (!stream.is_open()){
		cerr << "Unable to open file \"" << file << "\"" << std::endl;
		return 1;
	}
	while (!stream.eof()){
		getline(stream,line);
		parse(line, current);
		current = sections.back();
	}
	stream.close();
	return 0;
}

int ZDLConf::numberOfSections()
{
	int count = 0;
	list<ZDLSection*>::iterator itr;
	for (itr = sections.begin(); itr != sections.end();itr++){
		count++;
	}
	return count;

}

int ZDLConf::writeINI(const char *file)
{
	writes++;
	ofstream stream(file);
	if (!stream.is_open()){
		cerr << "Unable to open file \"" << file << "\"" << endl;
		return 1;
	}
	writeStream(stream);
	stream.close();
	return 0;
}

int ZDLConf::writeStream(ostream &stream){
	list<ZDLSection*>::iterator itr;
	for (itr = sections.begin(); itr != sections.end();itr++){
		ZDLSection* section = (*itr);
		section->streamWrite(stream);
	}
	return 0;
}

ZDLConf::ZDLConf()
{
	cout << "New configuration" << endl;
	reads = 0;
	writes = 0;
	vars = new ZDLVariables(this);
}

ZDLConf::~ZDLConf()
{
	cout << "Configuration deleted." << endl;
	while (sections.size() > 0){
		ZDLSection* section = sections.front();
		sections.pop_front();
		delete section;
	}
	if (vars)
		delete vars;
	cout << "Deleting configuration children." << endl;

}

void ZDLConf::deleteValue(const char *lsection, const char *variable){
	reads++;
	list<ZDLSection*>::iterator itr;
	for (itr = sections.begin(); itr != sections.end();itr++){
		ZDLSection* section = (*itr);
		if (strcmp(section->getName(), lsection) == 0){
			section->deleteVariable(variable);
			
		}
	}
}

const char *ZDLConf::getValue(const char *lsection, const char *variable, int *status){
	reads++;
	if (vars){
		cout << "ZDLConf::getValue using variable resolution" << endl;
		string rc = vars->getVariable(lsection, variable, status);
		return rc.c_str();
	}else{
		cout << "ZDLConf::getValue using non-variable resolution" << endl;
		ZDLSection *sect = getSection(lsection);
		if (sect){
			*status = 1;
			return sect->findVariable(variable);
		}

	}
	*status = 0;
	string ret = "";
	return ret.c_str();

}

//char *ZDLConf::getValue(const char *lsection, const char *variable){
//	int stat;
//	string temp = getValue(lsection, variable, &stat);
//	return (char*)temp.c_str();
//}

ZDLSection *ZDLConf::getSection(const char* lsection){
	list<ZDLSection*>::iterator itr;
	for (itr = sections.begin(); itr != sections.end();itr++){
		ZDLSection* section = (*itr);
		if (strcmp(section->getName(), lsection) == 0){
			return section;
		}
	}
	return NULL;
}

int ZDLConf::hasValue(const char *lsection, const char *variable){
	reads++;
	//If we actually have a variable resolver, lets use that.
	if (vars){
		int nRc = 0;
		return vars->hasVariable(lsection, variable, &nRc);
	//Otherwise, lets look for it ourself.
	}else{
		list<ZDLSection*>::iterator itr;
		for (itr = sections.begin(); itr != sections.end();itr++){
			ZDLSection* section = (*itr);
			if (strcmp(section->getName(), lsection) == 0){
				return section->hasVariable(variable);
			}
		}
	}
	return false;
}

int ZDLConf::setValue(const char *lsection, const char *variable, int value)
{
	char szBuffer[256];
	snprintf(szBuffer, 256, "%d", value);
	return setValue(lsection,variable,szBuffer);
}

int ZDLConf::setValue(const char *lsection, const char *variable, const char *szBuffer)
{
	string value = szBuffer;
	
	cout << "int ZDLConf::setValue("<<lsection<<","<<variable<<","<<value<<")"<<endl;
	writes++;
	list<ZDLSection*>::iterator itr;
	
	for (itr = sections.begin(); itr != sections.end();itr++){
		ZDLSection* section = (*itr);
		if (strcmp(section->getName(), lsection) == 0){
			section->setValue(variable, value.c_str());
			return 0;
		}
	}
	//In this case, we didn't find the section
	ZDLSection *section = new ZDLSection(lsection);
	sections.push_back(section);
	cout << "int ZDLConf::setValue("<<lsection<<","<<variable<<","<<szBuffer<<")"<<endl;
	section->setValue(variable, value.c_str());
	return 0;
}

void ZDLConf::parse(string in, ZDLSection* current)
{
	if (in.length() < 1){
		return;
	}
	string chomped = chomp(in);
	if (chomped[0] == '[' && chomped[chomped.length() - 1] == ']'){
		chomped = chomped.substr(1, chomped.length()-2);
		//This will remove duplicate sections automagically
		ZDLSection *ptr = getSection((char*)chomped.c_str());
		if (ptr == NULL){
			current = new ZDLSection((char*)chomped.c_str());
			sections.push_back(current);
		}else{
			current = ptr;
		}
	}else{
		current->addLine((char*)chomped.c_str());
	}
}


