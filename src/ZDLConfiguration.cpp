/*
 * This file is part of qZDL
 * Copyright (C) 2007-2012  Cody Harris
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

#include "ZDLConfiguration.h"

#include <iostream>

ZDLConfiguration::ZDLConfiguration(){
	//TODO These need to migrate
	QSettings system(QSettings::IniFormat,QSettings::SystemScope,"Vectec Software","qZDL",NULL);
	QSettings user(QSettings::IniFormat,QSettings::UserScope,"Vectec Software","qZDL",NULL);
	paths[CONF_SYSTEM] = system.fileName();
	paths[CONF_USER] = user.fileName();
	paths[CONF_FILE] = "zdl.ini";
	confs[CONF_SYSTEM] = new ZDLConf();
	confs[CONF_USER] = new ZDLConf();
	confs[CONF_FILE] = new ZDLConf();
}

QString ZDLConfiguration::getPath(ConfScope scope){
	if(scope >= NUM_CONFS || scope < 0){
		return QString();
	}
	return paths[scope];
}

ZDLConf* ZDLConfiguration::getConf(ConfScope scope){
	if(scope >= NUM_CONFS || scope < 0){
		return NULL;
	}
	return confs[scope];
}

QString ZDLConfiguration::getString(QString section, QString key, int *ok,ConfScope scope, ScopeRules rules){
	Q_UNUSED(section);
	Q_UNUSED(key);
	Q_UNUSED(scope);
	Q_UNUSED(rules);
	if(ok){*ok = FALSE;}
	return QString();
}

int ZDLConfiguration::getInt(QString section, QString key, int *ok, ConfScope scope, ScopeRules rules){
	Q_UNUSED(section);
	Q_UNUSED(key);
	Q_UNUSED(scope);
	Q_UNUSED(rules);
	if(ok){*ok = FALSE;}
	return 0;
}

bool ZDLConfiguration::setString(QString section, QString key, QString value, ConfScope scope, ScopeRules rules){
	Q_UNUSED(section);
	Q_UNUSED(key);
	Q_UNUSED(value);
	Q_UNUSED(scope);
	Q_UNUSED(rules);
	return FALSE;
}

bool ZDLConfiguration::setInt(QString section, QString key, int value, ConfScope scope, ScopeRules rules){
	Q_UNUSED(section);
	Q_UNUSED(key);
	Q_UNUSED(value);
	Q_UNUSED(scope);
	Q_UNUSED(rules);
	return FALSE;
}

bool ZDLConfiguration::hasVariable(QString section, QString key, ConfScope scope, ScopeRules rules){
	Q_UNUSED(section);
	Q_UNUSED(key);
	Q_UNUSED(scope);
	Q_UNUSED(rules);
	return FALSE;
}
