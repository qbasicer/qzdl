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
#include <QtCore>
#include <list>
using namespace std;
#include <zdlcommon.h>

QString chomp(QString in);

ZDLLine::ZDLLine(QString inLine)
{
	flags = FLAG_NORMAL;
	reads = 0;
	writes = 1;
	line = QString(chomp(inLine));
	//Convert slashes on Windows
#if defined(Q_WS_WIN)
	//If the line already contains slashes, don't mess with it
	if(line.contains("/")){
		slashConvert = false;
	}else{
		slashConvert = true;
	}
#else
	slashConvert = false;
#endif
	comment = "";
	if (line[0] == ';' || line[0] == '#'){
		type = 2;
	}else{
		type = 0;
		parse();
	}
	isCopy = false;
}

ZDLLine::ZDLLine()
{
	reads = 0;
	writes = 1;
	line = "";
	comment = "";
	value = "";
	variable = "";
	type = 2;
	isCopy = false;
}

ZDLLine::~ZDLLine()
{
}

void ZDLLine::setIsCopy(bool val){
	isCopy = val;
}

QString ZDLLine::getValue()
{
	reads++;
	if(!slashConvert){
		return value;
	}
	return QString(value).replace("\\","/");
}

QString ZDLLine::getVariable()
{
	reads++;
	if(!slashConvert){
		return variable;
	}
	return QString(variable).replace("\\","/");
}

QString ZDLLine::getLine()
{
	reads++;
	if(!slashConvert){
		return line;
	}
	return QString(line).replace("\\","/");;
}

int ZDLLine::setValue(QString inValue)
{
	if(isCopy){
		qDebug() << "SETTING A VALUE ON A COPY" << endl;
	}
	if(slashConvert){
		inValue.replace("/","\\");
	}
	// Don't overwrite if the string is the same!
	if(inValue.compare(value, Qt::CaseInsensitive) == 0){
		return 0;
	}
	line = variable + QString("=") + inValue;
	if(comment.size() > 0){
		line = line + QString("     ") + comment;
	}
	value = inValue;
	writes++;
	return 0;
}

int ZDLLine::findComment(char delim){
	int cloc = line.indexOf(delim, line.size());
	if (cloc > -1){
		if (cloc > 0){
			if (line[cloc-1] != '\\'){
				return cloc;
			}
		}
	}
	return -1;
}

void ZDLLine::parse()
{
	int cloc = findComment(';');
	int cloc2 = findComment('#');
	if(cloc != -1 && cloc2 != -1){
		cloc = min(cloc,cloc2);
	}else if(cloc == -1 && cloc2 != -1){
		cloc = cloc2;
	}
	
	if(cloc != -1){
		//Strip out comment
		comment = chomp(line.mid(cloc, line.size()));
		
	}
	
	int loc = line.indexOf("=", 0);
	if (loc > -1){
		variable = chomp(line.mid(0, loc));
		value = chomp(line.mid(loc+1, line.length() - loc - 1));
		//This is important for cross platform
		//Currently disabled
		if (slashConvert == true){
		}
		type = 0;
	}else{
		type = 1;
		variable = line;
		value = "";
	}

}

ZDLLine *ZDLLine::clone(){
	ZDLLine *copy = new ZDLLine();
	copy->variable = variable;
	copy->comment = comment;
	copy->line = line;
	copy->value = value;
	copy->slashConvert = slashConvert;
	copy->type = type;
	copy->flags = flags;
	reads += 7;
	copy->writes += 7;
	return copy;
}

bool ZDLLine::setFlags(int value){
	// Virtual flags cannot be modified
	if ((flags & FLAG_NOWRITE) == FLAG_NOWRITE){
		LOGDATAO() << "Cannot change flags on FLAG_NOWRITE" << endl;
		return false;
	}
	flags = value;
}

