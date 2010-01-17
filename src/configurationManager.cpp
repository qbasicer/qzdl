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
 
#include <stdlib.h>

#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

#include <zdlcommon.h>
#include "configurationManager.h"
#include "ZInfoBar.h"
#include "ico_icon.xpm"

void configurationManager::init(){
	activeConfig = NULL;
	cdir = "";
}

ZDLConf *configurationManager::activeConfig;
string configurationManager::cdir;
ZQWidget* configurationManager::interface;
ZQWidget* configurationManager::infobar;
QString configurationManager::filename;
ZUpdater *configurationManager::zupper;

void configurationManager::setInterface(ZQWidget *widget){
	interface  = widget;
}
ZQWidget* configurationManager::getInterface(){
	return interface;
}

void configurationManager::setActiveConfiguration(ZDLConf *zconf){
	//cout << "Using new configuration" << endl;
	configurationManager::activeConfig = zconf;
}

ZDLConf* configurationManager::getActiveConfiguration(){
	return configurationManager::activeConfig;
}

void configurationManager::setCurrentDirectory(string dir){
	cdir = dir;
}

void configurationManager::setInfobar(ZQWidget *widget){
	infobar = widget;
}

QPixmap configurationManager::getIcon(){
	return QPixmap(zdlicon);
}

ZQWidget* configurationManager::getInfobar(){
	return infobar;
}

void configurationManager::setInfobarMessage(const char* message, int icon){
	ZInfoBar *bar = (ZInfoBar*)infobar;
	bar->setMessage(message,icon);
}

const char* configurationManager::getCurrentDirectory(){
	return cdir.c_str();
}


QString configurationManager::getConfigFileName(){
	return filename;
}

void configurationManager::setConfigFileName(QString name){
	filename = name;
}

void configurationManager::setUpdater(ZUpdater *zup){
	zupper = zup;
}

ZUpdater *configurationManager::getUpdater(){
	return zupper;
}