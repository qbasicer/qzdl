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
#ifndef _ZDLSECTION_HPP_
#define _ZDLSECTION_HPP_

#include "zdlcommon.h"
#include <QtCore>
#include "zdlline.hpp"

class ZDLSection{
	friend class ZDLVariables;
public:
	ZDLSection(QString name);
	~ZDLSection();
	int addLine(QString data);
	QString getName();
	void setSpecial(int inFlags);
	QString findVariable(QString variable);
	int hasVariable(QString variable);
	void deleteVariable(QString variable);
	int setValue(QString variable, QString value);
	int streamWrite(QIODevice *stream);
	int getRegex(QString regex, QVector<ZDLLine*> &vctr);
	QVector<ZDLLine*> lines;
	ZDLSection *clone();
	void addLine(ZDLLine* line){
		lines.push_back(line);
	}
	void setIsCopy(bool copy){
		isCopy = copy;
		for(int i = 0; i < lines.size(); i++){
			lines[i]->setIsCopy(copy);
		}
	}
	int getFlagsForValue(QString var);
	bool setFlagsForValue(QString var, int value);
	bool deleteRegex(QString regex);
protected:
	void readLock(const char* file, int line){
		if(writelock){
			return;
		}
		LOGDATAO() << "ReadLockGet@" << file << ":" << line << endl;
		GET_READLOCK(mutex);
	}
	void writeLock(const char* file, int line){
		LOGDATAO() << "WriteLockGet@" << file << ":" << line << endl;
		if(isCopy){
			qDebug() << "WriteLock on copy from " << file << ":" << line << endl;
		}
		GET_WRITELOCK(mutex);
		writelock = true;
	}
	void releaseReadLock(const char* file, int line){
		LOGDATAO() << "ReadLockRelease@" << file << ":" << line << endl;
		RELEASE_READLOCK(mutex);
	}
	void releaseWriteLock(const char* file, int line){
		LOGDATAO() << "WriteLockRelease@" << file << ":" << line << endl;
		writelock = false;
		RELEASE_WRITELOCK(mutex);
	}
	bool tryReadLock(const char* file, int line, int timeout = 999999999){
		if(writelock){
			return true;
		}
		LOGDATAO() << "ReadLockTryGet@" << file << ":" << line << endl;
		return TRY_READLOCK(mutex, timeout);
	}
	bool tryWriteLock(const char* file, int line, int timeout = 999999999){
		LOGDATAO() << "WriteLockTryGet@" << file << ":" << line << endl;
		if(isCopy){
			qDebug() << "WriteLock on copy" << endl;
		}
		bool r = TRY_WRITELOCK(mutex, timeout);
		if(r){
			writelock = true;
		}
		return r;
	}
	private:
	LOCK_CLASS *mutex;
	bool writelock;
	int reads;
	int writes;
	bool isCopy;
	ZDLLine *findLine(QString inVar);
	int flags;
	QString sectionName;
};

#endif

