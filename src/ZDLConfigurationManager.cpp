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
ZDLWidget* ZDLConfigurationManager::interface;
ZDLWidget* ZDLConfigurationManager::infobar;
QString ZDLConfigurationManager::filename;
ZDLUpdater *ZDLConfigurationManager::zupper;
ZDLConfiguration *ZDLConfigurationManager::conf;
ZDLConfigurationManager::WhyConfig ZDLConfigurationManager::why;

void ZDLConfigurationManager::setWhy(ZDLConfigurationManager::WhyConfig conf){
	ZDLConfigurationManager::why = conf;
}

ZDLConfigurationManager::WhyConfig ZDLConfigurationManager::getWhy(){
	return ZDLConfigurationManager::why;
}


void ZDLConfigurationManager::setInterface(ZDLWidget *widget){
	interface  = widget;
}
ZDLWidget* ZDLConfigurationManager::getInterface(){
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

void ZDLConfigurationManager::setInfobar(ZDLWidget *widget){
	infobar = widget;
}

QPixmap ZDLConfigurationManager::getIcon(){
	return QPixmap(zdlicon);
}

ZDLWidget* ZDLConfigurationManager::getInfobar(){
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

void ZDLConfigurationManager::setUpdater(ZDLUpdater *zup){
	zupper = zup;
}

ZDLUpdater *ZDLConfigurationManager::getUpdater(){
	return zupper;
}

ZDLConfiguration* ZDLConfigurationManager::getConfiguration(){
	return conf;
}
