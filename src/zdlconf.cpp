/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * Copyright (C) 2018-2019  Lcferrum
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
#include <cstring>
#include "zdlcommon.h"


/* ZDLConf.cpp
 *    Author: Cody Harris
 *   Purpose: To load in a configuration file, and parse it into sections and lines
 * Rationale: The current implementation of configuration files stores the values
 *            in a system which forgets keys and comments it doesn't know about.
 *            This system caches everything, and makes changes on the fly so that
 *            config files can be written back with keys it doesn't know how to use.
 *      Date: July 29th, 2007
 */

int ZDLConf::readINI(QString file)
{	
	LOGDATAO() << "Reading file " << file << endl;
	if((mode & ZDLConf::FileRead) != 0){
		writeLock();
		reads++;
		QString line;
		/* We allow lines to be outside of any section (ie header comments)
		 * We use a global section to read this.  We also keep track of
		 * which section we're in.  We do that with a pointer (current)
		 */
		ZDLSection *current = new ZDLSection("");
		current->setSpecial(ZDL_FLAG_NAMELESS);
		sections.push_back(current);
		QFile stream(file);
		stream.open(QIODevice::ReadOnly);
		if (!stream.isOpen()){
			LOGDATAO() << "Unable to open file" << endl;
			releaseWriteLock();
			return 1;
		}
		while (!stream.atEnd()){
			QByteArray array = stream.readLine();
			QString line(array);
			parse(line, current);
			current = sections.back();
		}
		stream.close();

		QFileInfo info(file);
		if(!info.isWritable()){
			LOGDATAO() << "File is unwriteable, writes will be ignored" << endl;
			mode = mode & ~FileWrite;
		}
		releaseWriteLock();
		return 0;
	}else{
		LOGDATAO() << "Cannot read file" << endl;
		return 1;
	}
}

int ZDLConf::numberOfSections()
{
	LOGDATAO() << "numberOfSections: " << sections.size();
	return sections.size();

}

int ZDLConf::writeINI(QString file)
{
	setValue("zdl.general", "conflib", "sunrise");
	LOGDATAO() << "Writing file to " << file << endl;
	if((mode & ZDLConf::FileWrite) != 0){
		writes++;
		QFile stream(file);
		stream.open(QIODevice::WriteOnly);
		if (!stream.isOpen()){
			QFileInfo fi(file);
			QDir dir = fi.dir();
			if(!dir.exists()){
				dir.mkpath(".");
				stream.open(QIODevice::WriteOnly);
				if (!stream.isOpen()){
					LOGDATAO() << "Cannot create directory and cannot save file" << endl;
					return 1;
				}
			}else{
				LOGDATAO() << "Cannot open file for writing" << endl;
				return 1;
			}
		}
		QIODevice *dev = &stream;
		writeStream(dev);
		stream.close();
		return 0;
	}else{
		LOGDATAO() << "Cannot write file, no permission" << endl;
		return 1;
	}
}

int ZDLConf::writeStream(QIODevice *stream){
	if((mode & ZDLConf::FileWrite) != 0){
		readLock();
		for(int i = 0; i < sections.size(); i++){
			sections[i]->streamWrite(stream);
		}
		releaseReadLock();
		return 0;
	}else{
		return 1;
	}
}

ZDLConf::ZDLConf(int mode)
{
	LOGDATAO() << "New ZDLConf" << endl;
	this->mode = mode;
	//cout << "New configuration" << endl;
	reads = 0;
	writes = 0;
	mutex = LOCK_BUILDER();
	//vars = new ZDLVariables(this);
}

/* int ZDLConf::reopen(int mode)
 * Returns: 0 on success, nonzero on failure
 */
int ZDLConf::reopen(int mode){
	LOGDATAO() << "Reopening with new permissions" << endl;
	this->mode = mode;
	return 0;
}

ZDLConf::~ZDLConf()
{
	writeLock();
	LOGDATAO() << "Destroying ZDLConf" << endl;
	//cout << "Configuration deleted." << endl;
	while (sections.size() > 0){
		ZDLSection* section = sections.front();
		sections.pop_front();
		delete section;
	}
	releaseWriteLock();
	delete mutex;
	//if (vars)
	//	delete vars;
	//cout << "Deleting configuration children." << endl;

}

void ZDLConf::deleteSection(QString lsection){
	LOGDATAO() << "Deleting section " << lsection << endl;
	writeLock();
	for(int i = 0; i < sections.size(); i++){
		ZDLSection* section = sections[i];
		QString secName = section->getName();
		if (secName == lsection){
			LOGDATAO() << "Found and removed" << endl;
			sections.remove(i);
			releaseWriteLock();
			return;
		}
	}
	releaseWriteLock();
	LOGDATAO() << "No such section" << endl;
}

void ZDLConf::deleteValue(QString lsection, QString variable){
	LOGDATAO() << "Deleting value " << lsection << "/" << variable << endl;
	writeLock();
	if((mode & WriteOnly) != 0){
		writes++;
		for(int i = 0; i < sections.size(); i++){
			ZDLSection* section = sections[i];
			if (section->getName().compare(lsection, Qt::CaseInsensitive) == 0){
				LOGDATAO() << "Found section" << endl;
				section->deleteVariable(variable);
				releaseWriteLock();
				return;
			}

		}
	}
	releaseWriteLock();
}

QString ZDLConf::getValue(QString lsection, QString variable, int *status){
	LOGDATAO() << "Getting value " << lsection << "/" << variable << endl;
	if((mode & ReadOnly) != 0){
		readLock();
		reads++;
		ZDLSection *sect = getSection(lsection);
		if (sect){
			*status = 0;
			releaseReadLock();
			return sect->findVariable(variable);
		}
		*status = 1;
		releaseReadLock();
	}
	*status = 2;
	LOGDATAO() << "Failed to get value" << endl;
	return QString();
}

QString ZDLConf::getValue(QString lsection, QString variable){
	LOGDATAO() << "Getting value " << lsection << "/" << variable << endl;
	if((mode & ReadOnly) != 0){
		readLock();
		reads++;
		ZDLSection *sect = getSection(lsection);
		if (sect){
			releaseReadLock();
			return sect->findVariable(variable);
		}
		releaseReadLock();
	}
	LOGDATAO() << "Failed to get value" << endl;
	return QString();
}

ZDLSection *ZDLConf::getSection(QString lsection){
	LOGDATAO() << "getting section " << lsection << endl;
	if((mode & ReadOnly) != 0){
		readLock();
		for(int i = 0; i < sections.size(); i++){
			ZDLSection* section = sections[i];
			if (section->getName().compare(lsection,Qt::CaseInsensitive) == 0){
				LOGDATAO() << "Got it " << DPTR(section) << endl;
				releaseReadLock();
				ZDLSection *copy = section->clone();
				copy->setIsCopy(true);
				return copy;
			}
		}
		releaseReadLock();
	}
	LOGDATAO() << "Failed to find section" << endl;
	return NULL;
}

int ZDLConf::hasValue(QString lsection, QString variable){
	LOGDATAO() << "hasValue: " << lsection << "/" << variable << endl;
	if((mode & ReadOnly) != 0){
		reads++;
		readLock();
		for(int i = 0; i < sections.size(); i++){
			ZDLSection* section = sections[i];
			if (section->getName().compare(lsection, Qt::CaseInsensitive) == 0){
				releaseReadLock();
				return section->hasVariable(variable);
			}

		}
		releaseReadLock();
	}
	LOGDATAO () << "No matching sections" << endl;
	return false;
}

int ZDLConf::setValue(QString lsection, QString variable, int value)
{
	if((mode & WriteOnly) != 0){
		return setValue(lsection,variable,QString::number(value));
	}else{
		return -1;
	}
}

int ZDLConf::setValue(QString lsection, QString variable, QString szBuffer)
{
	LOGDATAO() << "setValue: " << lsection << "/" << variable << " = " << szBuffer << endl;
	if((mode & WriteOnly) != 0){
		QString value = szBuffer;

		//Better handing of variables.  Don't overwrite if you don't have to.
		writeLock();
		if(hasValue(lsection,variable)){
			int stat;
			QString oldValue = getValue(lsection, variable, &stat);
			if(oldValue == szBuffer){
				LOGDATAO() << "No difference between set and previous variable" << endl;
				releaseWriteLock();
				return 0;
			}
		}

		writes++;
		for(int i = 0; i < sections.size(); i++){
			ZDLSection* section = sections[i];
			if (section->getName().compare(lsection,Qt::CaseInsensitive) == 0){
				section->setValue(variable, value);
				LOGDATAO() << "Asked section to set variable" << endl;
				releaseWriteLock();
				return 0;
			}

		}
		LOGDATAO() << "No such section, creating" << endl;
		//In this case, we didn't find the section
		ZDLSection *section = new ZDLSection(lsection);
		sections.push_back(section);
		//cout << "int ZDLConf::setValue("<<lsection<<","<<variable<<","<<szBuffer<<")"<<endl;
		section->setValue(variable, value);
		releaseWriteLock();
	}
	return 0;
}

void ZDLConf::parse(QString in, ZDLSection* current)
{
	if (in.length() < 1){
		return;
	}
	in=in.trimmed();;
	LOGDATAO() << "Parse " << in << endl;
	if (in[0] == '[' && in[in.length() - 1] == ']'){
		in = in.mid(1, in.length()-2);
		//This will remove duplicate sections automagically
		ZDLSection *ptr = getSection(in);
		if (ptr == NULL){
			current = new ZDLSection(in);
			sections.push_back(current);
		}else{
			current = ptr;
		}
	}else{
		current->addLine(in);
	}
}

ZDLConf *ZDLConf::clone(){
	LOGDATAO() << "Closing self" << endl;
	ZDLConf *copy = new ZDLConf();
	readLock();
	for(int i = 0; i < sections.size(); i++){
		copy->addSection(sections[i]->clone());
	}
	releaseReadLock();
	LOGDATAO() << "Cloned self to " << DPTR(copy) << endl;
	return copy;
}

void ZDLConf::deleteSectionByName(QString section){
	LOGDATAO() << "Deleting section " << section << endl;
	writeLock();
	for(int i = 0; i < sections.size(); i++){
		if(sections[i]->getName().compare(section) == 0){
			ZDLSection *sect = sections[i];
			sections.remove(i);
			LOGDATAO() << "Deleted section" << endl;
			releaseWriteLock();
			delete sect;
		}
	}
	releaseWriteLock();
	LOGDATAO() << "No such section" << endl;
}

int ZDLConf::getFlagsForValue(QString lsection, QString var){
	readLock();
	for(int i = 0; i < sections.size(); i++){
		ZDLSection* section = sections[i];
		if (section->getName().compare(lsection,Qt::CaseInsensitive) == 0){
			releaseReadLock();
			return section->getFlagsForValue(var);
		}
	}
	releaseReadLock();
	return -1;
}

bool ZDLConf::setFlagsForValue(QString lsection, QString var, int value){
	readLock();
	for(int i = 0; i < sections.size(); i++){
		ZDLSection* section = sections[i];
		if (section->getName().compare(lsection,Qt::CaseInsensitive) == 0){
			releaseReadLock();
			return section->setFlagsForValue(var,value);
		}
	}
	releaseReadLock();
	return false;
}

bool ZDLConf::deleteRegex(QString lsection, QString regex){
	readLock();
	for(int i = 0; i < sections.size(); i++){
		ZDLSection* section = sections[i];
		if (section->getName().compare(lsection,Qt::CaseInsensitive) == 0){
			bool rc = section->deleteRegex(regex);
			releaseReadLock();
			return rc;
		}
	}
	releaseReadLock();
	return false;

}


