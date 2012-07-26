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

#define READLOCK() (readLock(__FILE__,__LINE__))
#define WRITELOCK() (writeLock(__FILE__,__LINE__))
#define READUNLOCK() (releaseReadLock(__FILE__,__LINE__))
#define WRITEUNLOCK() (releaseWriteLock(__FILE__,__LINE__))
#define TRYREADLOCK(to) (tryReadLock(__FILE__,__LINE__,to))
#define TRYWRITELOCK(to) (tryWriteLock(__FILE__,__LINE__,to))

ZDLSection::ZDLSection(QString name)
{
	//cout << "New section: \"" << name << "\"" << endl;
	reads = 0;
	writes = 0;
	sectionName = name;
	mutex = LOCK_BUILDER();
	isCopy = false;
}

ZDLSection::~ZDLSection()
{
	//cout << "Deleting section and it's children..." << endl;
	WRITELOCK();
	while (lines.size() > 0){
		ZDLLine *line = lines.front();
		lines.pop_front();
		delete line;
	}
	WRITEUNLOCK();
	delete mutex;
	//cout << "Section deleted" << endl;
}

void ZDLSection::setSpecial(int inFlags)
{
	flags = inFlags;
}

int ZDLSection::hasVariable(QString variable)
{
	reads++;
	READLOCK();
	for(int i = 0; i < lines.size(); i++){
		ZDLLine* line = lines[i];
		if (line->getVariable().compare(variable) == 0){
			READUNLOCK();
			return true;
		}
	}
	READUNLOCK();
	return false;
}

void ZDLSection::deleteVariable(QString variable)
{
	WRITELOCK();
	reads++;
	for(int i = 0; i < lines.size(); i++){
		ZDLLine* line = lines[i];
		if (line->getVariable().compare(variable) == 0){
			lines.remove(i);
			delete line;
			WRITEUNLOCK();
			return;
		}
	}
	WRITEUNLOCK();
}

QString ZDLSection::findVariable(QString variable)
{
	reads++;
	READLOCK();
	for(int i = 0; i < lines.size(); i++){
		ZDLLine* line = lines[i];
		if (line->getVariable().compare(variable) == 0){
			QString val(line->getValue());
			READUNLOCK();
			return val;
		}
	}
	READUNLOCK();
	return QString("");
}

int ZDLSection::getRegex(QString regex, QVector<ZDLLine*> &vctr){
#ifdef QT_CORE_LIB
	QRegExp rx(regex);
	READLOCK();
	for(int i = 0; i < lines.size(); i++){
		ZDLLine* line = lines[i];
		if (rx.exactMatch(line->getVariable())){
			ZDLLine *copy = line->clone();
			copy->setIsCopy(true);
			vctr.push_back(copy);
		}
	}
	READUNLOCK();
	return vctr.size();

#endif
	return 0;
}

int ZDLSection::setValue(QString variable, QString value)
{
	writes++;
	WRITELOCK();
	for(int i = 0; i < lines.size(); i++){
		ZDLLine* line = lines[i];
		if (line->getVariable().compare(variable) == 0){
			if((line->getFlags() & FLAG_NOWRITE) == FLAG_NOWRITE){
				return -1;
			}
			line->setValue(value);
			WRITEUNLOCK();
			return 0;
		}
	}
	//We can't find the line, create a new one.
	QString buffer = variable;
	buffer.append("=");
	buffer.append(value);
	//cout << "Buffer: " << buffer << endl;
	ZDLLine *line = new ZDLLine(buffer);
	lines.push_back(line);
	WRITEUNLOCK();
	return 0;
}

int ZDLSection::streamWrite(QIODevice *stream)
{

#if defined(Q_WS_WIN)
	QString el("\r\n");
#define ENDOFLINE el
#else
	QString el("\n");
#define ENDOFLINE el
#endif
	READLOCK();
	QTextStream tstream(stream);
	//Write only if we have stuff to write
	if (lines.size() > 0){
		writes++;
		//Global's don't have a section name
		if (sectionName.length() > 0){
			tstream << "[" << sectionName << "]" << ENDOFLINE;
		}
		for(int i = 0; i < lines.size(); i++){
			ZDLLine *line = lines[i];
			if((line->getFlags() & FLAG_VIRTUAL) != 0 || (line->getFlags() & FLAG_TEMP) != 0){
				tstream << line->getLine() << ENDOFLINE;
			}
		}
	}
	READUNLOCK();
	return 0;
}

QString ZDLSection::getName()
{
	reads++;
	return sectionName;
}

ZDLLine *ZDLSection::findLine(QString inVar)
{
	READLOCK();
	for (int i = 0; i < lines.size(); i++){
		ZDLLine* line = lines[i];
		if (line->getVariable().compare(inVar) == 0){
			qDebug() << "UNSAFE OPERATION AT " << __FILE__ << ":" << __LINE__ << endl;
			READUNLOCK();
			return line;
		}
	}
	READUNLOCK();
	return NULL;

}

int ZDLSection::addLine(QString linedata)
{
	writes++;
	ZDLLine *newl = new ZDLLine(linedata);
	WRITELOCK();
	ZDLLine *ptr = findLine(newl->getVariable());
	if (ptr == NULL){
		lines.push_back(newl);
		WRITEUNLOCK();
		return 0;
	}else{
		//cerr << "ERROR: Duplicate variable " << sectionName << "#" << newl->getVariable() << endl;
		ptr->setValue(newl->getValue());
		delete newl;
		WRITEUNLOCK();
		return 1;
	}

}

ZDLSection *ZDLSection::clone(){
	READLOCK();
	ZDLSection *copy = new ZDLSection(sectionName);
	for(int i = 0; i < lines.size(); i++){
		/* Virtual flags do not get cloned */
		if((lines[i]->getFlags() & FLAG_VIRTUAL) == 0){
			copy->addLine(lines[i]->clone());
		}
	}
	READUNLOCK();
	return copy;
}

int ZDLSection::getFlagsForValue(QString var){
	READLOCK();
	for(int i = 0; i < lines.size(); i++){
		ZDLLine* line = lines[i];
		if (line->getVariable().compare(var) == 0){
			return line->getFlags();
		}
	}
	READUNLOCK();
	return -1;
}

bool ZDLSection::setFlagsForValue(QString var, int value){
	READLOCK();
	for(int i = 0; i < lines.size(); i++){
		ZDLLine* line = lines[i];
		if (line->getVariable().compare(var) == 0){
			return line->setFlags(value);
		}
	}
	READUNLOCK();
	return false;
}





