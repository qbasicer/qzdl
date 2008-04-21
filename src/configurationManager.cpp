#include <stdlib.h>

#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

#include <zdlcommon.h>
#include "configurationManager.h"

void configurationManager::init(){
	activeConfig = NULL;
	cdir = "";
}

ZDLConf *configurationManager::activeConfig;
string configurationManager::cdir;
ZQWidget* configurationManager::interface;

void configurationManager::setInterface(ZQWidget *widget){
	interface  = widget;
}
ZQWidget* configurationManager::getInterface(){
	return interface;
}

void configurationManager::setActiveConfiguration(ZDLConf *zconf){
	cout << "Using new configuration" << endl;
	configurationManager::activeConfig = zconf;
}

ZDLConf* configurationManager::getActiveConfiguration(){
	return configurationManager::activeConfig;
}

void configurationManager::setCurrentDirectory(string dir){
	cdir = dir;
}

const char* configurationManager::getCurrentDirectory(){
	return cdir.c_str();
}
