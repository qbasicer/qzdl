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
 

#include "ZListable.h"
#include "ZNameListable.h"
#include <string>
#include <QFileInfo>
#include <iostream>

using namespace std;

ZNameListable::ZNameListable( QListWidget *parent, int type, const char* file, const char* name):ZListable(parent, type){
	fileName = file;
	displayName = name;
	setName(generateName());
}


ZNameListable::ZNameListable( QListWidget *parent, int type, QString &file, QString &name):ZListable(parent, type){
	fileName = file;
	displayName = name;
	setName(generateName());

}

QString ZNameListable::getFile(){
	return fileName;
}

QString ZNameListable::getName(){
	return displayName;
}

void ZNameListable::setDisplayName(QString name){
	displayName = name;
	setName(generateName());
}

void ZNameListable::setFile(QString file){
	fileName = file;
	setName(generateName());
}

QString ZNameListable::generateName(){
	QString list = QString("%1 [%2]").arg(displayName).arg(fileName);
	return list;
}

