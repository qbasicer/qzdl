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
#include "ZDLConfigurationManager.h"
#include "ZDLInfoBar.h"
#include "ico_icon.xpm"

void ZDLConfigurationManager::init(){
	activeConfig = NULL;
	cdir = "";
	conf = new ZDLConfiguration();
}

ZDLConf *ZDLConfigurationManager::activeConfig;
string ZDLConfigurationManager::cdir;
ZQWidget* ZDLConfigurationManager::interface;
ZQWidget* ZDLConfigurationManager::infobar;
QString ZDLConfigurationManager::filename;
ZUpdater *ZDLConfigurationManager::zupper;
ZDLConfiguration *ZDLConfigurationManager::conf;

void ZDLConfigurationManager::setInterface(ZQWidget *widget){
	interface  = widget;
}
ZQWidget* ZDLConfigurationManager::getInterface(){
	return interface;
}

void ZDLConfigurationManager::setActiveConfiguration(ZDLConf *zconf){
	//cout << "Using new configuration" << endl;
	ZDLConfigurationManager::activeConfig = zconf;
}

ZDLConf* ZDLConfigurationManager::getActiveConfiguration(){
	return ZDLConfigurationManager::activeConfig;
}

void ZDLConfigurationManager::setCurrentDirectory(string dir){
	cdir = dir;
}

void ZDLConfigurationManager::setInfobar(ZQWidget *widget){
	infobar = widget;
}

QPixmap ZDLConfigurationManager::getIcon(){
	return QPixmap(zdlicon);
}

ZQWidget* ZDLConfigurationManager::getInfobar(){
	return infobar;
}

void ZDLConfigurationManager::setInfobarMessage(const char* message, int icon){
	ZDLInfoBar *bar = (ZDLInfoBar*)infobar;
	bar->setMessage(message,icon);
}

const char* ZDLConfigurationManager::getCurrentDirectory(){
	return cdir.c_str();
}


QString ZDLConfigurationManager::getConfigFileName(){
	return filename;
}

void ZDLConfigurationManager::setConfigFileName(QString name){
	filename = name;
}

void ZDLConfigurationManager::setUpdater(ZUpdater *zup){
	zupper = zup;
}

ZUpdater *ZDLConfigurationManager::getUpdater(){
	return zupper;
}

ZDLConfiguration* ZDLConfigurationManager::getConfiguration(){
	return conf;
}