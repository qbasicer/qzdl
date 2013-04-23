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

#ifndef _ZDLCONF_HPP_
#define _ZDLCONF_HPP_
 
class ZDLVariables;

#include <QtCore>
#include <iostream>
#include <list>
#include "zdlcommon.h"
#include "zdlsection.hpp"

class ZDLConf {
	friend class ZDLVariables;
public:
	enum modes{
		ReadOnly  		= 0x01,
		WriteOnly 		= 0x02,
		ReadWrite 		= ReadOnly | WriteOnly,
		FileRead  		= 0x04,
  		FileWrite 		= 0x08,
		FileReadWrite 	= FileRead | FileWrite,
  		Default			= ReadWrite | FileReadWrite
	};
	
	int readINI(QString file);
	int writeINI(QString file);
	//char* getValue(QString section, QString variable);
	QString getValue(QString lsection, QString variable, int *status);
	int hasValue(QString section, QString variable);
	void deleteValue(QString lsection, QString variable);
	int setValue(QString lsection, QString variable, int value);
	int setValue(QString lsection, QString variable, QString szBuffer);
	int numberOfSections();
	~ZDLConf();
	ZDLConf(int mode = ZDLConf::Default);
	int reopen(int mode);
	QVector<ZDLSection*> sections;
	int writeStream(QIODevice *stream);
	ZDLSection *getSection(QString section);
	void deleteSection(QString section);
	ZDLConf *clone();
	void deleteSectionByName(QString section);
	void addSection(ZDLSection *section){
		sections.push_back(section);
	}
	int getFlagsForValue(QString section, QString var);
	bool setFlagsForValue(QString section, QString var, int value);
	bool deleteRegex(QString section, QString regex);
protected:
	void readLock(){
		if(writelock){
			return;
		}
		LOGDATAO() << "ReadLockGet on " << PTR_ADDR(mutex) << endl;
		GET_READLOCK(mutex);
	}
	void writeLock(){
		LOGDATAO() << "WriteLockGet on" << PTR_ADDR(mutex) << endl;
		GET_WRITELOCK(mutex);
		writelock = 1;
	}
	void releaseReadLock(){
		LOGDATAO() << "ReadLockRelease on " << PTR_ADDR(mutex) << endl;
		RELEASE_READLOCK(mutex);
	}
	void releaseWriteLock(){
		LOGDATAO() << "WriteLockRelease on " << PTR_ADDR(mutex) << endl;
		writelock = 0;
		RELEASE_WRITELOCK(mutex);
	}
	bool tryReadLock(int timeout = 999999999){
		if(writelock){
			return true;
		}
		LOGDATAO() << "ReadLockTryGet on " << PTR_ADDR(mutex) << endl;
		return TRY_READLOCK(mutex, timeout);
	}
	bool tryWriteLock(int timeout = 999999999){
		LOGDATAO() << "WriteLockTryGet on " << PTR_ADDR(mutex) << endl;
		bool r = TRY_WRITELOCK(mutex, timeout);
		if(r){
			writelock = 1;
		}
		return r;
	}
private:
	int writelock;
	int mode;
	int reads;
	int writes;
	void parse(QString in, ZDLSection* current);
	LOCK_CLASS *mutex;
	//ZDLVariables *vars;
};

#endif

