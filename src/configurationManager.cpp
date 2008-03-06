#include <stdlib.h>

#include <iostream>
#include <list>
#include <string>

using namespace std;

#include <zdlcommon.h>
#include "configurationManager.h"

void configurationManager::init(){
	activeConfig = NULL;
	cdir = "";
}

ZDLConf *configurationManager::activeConfig;
string configurationManager::cdir;

void configurationManager::setActiveConfiguration(ZDLConf *zconf){
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
