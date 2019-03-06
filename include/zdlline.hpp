/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * Copyright (C) 2019  Lcferrum
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

#ifndef _ZDLLINE_HPP_
#define _ZDLLINE_HPP_

#include <string>
#include <QtCore>
 
#define FLAG_NORMAL	0	// Normal flag
#define FLAG_VIRTUAL	1	// Does not get read/written, not cloned
#define FLAG_NOWRITE	2	// Can not write to this value
#define FLAG_TEMP	4	// Read/write and cloned, but not written

class ZDLLine{
	friend class ZDLVariables;
public:
	ZDLLine(QString inLine);
	ZDLLine();
	~ZDLLine();
	int getType();
	QString getValue();
	QString getVariable();
	QString getLine();
	int setValue(QString inValue);
	int isSlashConverting(){return slashConvert;}
	ZDLLine *clone();
	void setIsCopy(bool val);
	bool setFlags(int val);
	int getFlags(){return flags;}
private:
	bool isCopy;
	int reads;
	int writes;
	void parse();
	int findComment(char delim);
	int type;
	QString line;
	QString comment;
	QString value;
	QString variable;
	bool slashConvert;
	int flags;
};

#endif

