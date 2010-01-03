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