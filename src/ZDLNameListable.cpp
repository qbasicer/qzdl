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


#include "ZDLListable.h"
#include "ZDLNameListable.h"
#include <QFileInfo>
#include "zdlconf.h"

using namespace std;

ZDLNameListable::ZDLNameListable( QListWidget *parent, int type, QString file, QString name):ZDLListable(parent, type){
	setFile(file);
	setDisplayName(name);
	setName(generateName());

	// TODO: There used to be a signal->slot connection to call setName whenever the configuration changes.
}

ZDLNameListable::~ZDLNameListable(){
}


QString ZDLNameListable::getFile(){
	return fileName;
}

QString ZDLNameListable::getName(){
	return displayName;
}

void ZDLNameListable::setDisplayName(QString name){
	displayName = name;
	setName(generateName());
}

void ZDLNameListable::setFile(QString file){
	fileName = file;
	setName(generateName());
}


QString ZDLNameListable::generateName(){
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	bool showPath = true;
	if(zconf->contains("zdl.general/showpaths")){
		QString rc = zconf->value("zdl.general/showpaths").toString();
		if(!rc.isNull()){
			if(rc == "0"){
				showPath = false;
			}
		}
	}
	QString list = "";
	if(showPath){
		list = QString("%1 [%2]").arg(displayName).arg(fileName);
	}else{
		list = displayName;
	}
	return list;
}

