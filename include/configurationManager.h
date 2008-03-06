#ifndef _configurationManger_h_
#define _configurationManger_h_
#include <iostream>
#include <list>
#include <string>

using namespace std;

#include <zdlcommon.h>

class configurationManager{
	public:
		static void init();
		static void setActiveConfiguration(ZDLConf *zconf);
		static ZDLConf* getActiveConfiguration();
		static void setCurrentDirectory(string dir);
		static const char* getCurrentDirectory();
	private:
		static ZDLConf *activeConfig;
		static string cdir;
};

#endif
