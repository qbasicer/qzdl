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
 
class ZDLVariables;

#include <QtCore>
#include <iostream>
#include <list>

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
private:
	int mode;
	int reads;
	int writes;
	void parse(QString in, ZDLSection* current);
	//ZDLVariables *vars;
};
