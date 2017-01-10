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
	events = new ZDLConfigurationEvents();
}

ZDLConf *ZDLConfigurationManager::activeConfig;
string ZDLConfigurationManager::cdir;
ZDLWidget* ZDLConfigurationManager::interface;
ZDLWidget* ZDLConfigurationManager::infobar;
QString ZDLConfigurationManager::filename;
ZDLConfiguration *ZDLConfigurationManager::conf;
ZDLConfigurationManager::WhyConfig ZDLConfigurationManager::why;
QStringList ZDLConfigurationManager::argv;
QString ZDLConfigurationManager::exec;
ZDLConfigurationEvents* ZDLConfigurationManager::events;

ZDLConfigurationEvents* ZDLConfigurationManager::getEvents(){
	return ZDLConfigurationManager::events;
}

void ZDLConfigurationManager::setExec(QString execu){
	ZDLConfigurationManager::exec = execu;
}

QString ZDLConfigurationManager::getExec(){
	return ZDLConfigurationManager::exec;
}

QStringList ZDLConfigurationManager::getArgv(){
	return ZDLConfigurationManager::argv;
}

void ZDLConfigurationManager::setArgv(QStringList args){
	ZDLConfigurationManager::argv = args;
}

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
	events->signalNewConfiguration(zconf);
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

ZDLConfiguration* ZDLConfigurationManager::getConfiguration(){
	return conf;
}

