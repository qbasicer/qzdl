#ifndef _configurationManger_h_
#define _configurationManger_h_
#include <iostream>
#include <list>
#include <string>
#include "ZQWidget.h"

using namespace std;

#include <zdlcommon.h>

class configurationManager{
	public:
		static void init();
		static ZQWidget* getInterface();
		static void setInterface(ZQWidget *widget);
		static void setActiveConfiguration(ZDLConf *zconf);
		static ZDLConf* getActiveConfiguration();
		static void setCurrentDirectory(string dir);
		static const char* getCurrentDirectory();
		static void setInfobar(ZQWidget *zib);
		static ZQWidget *getInfobar();
		static QPixmap getIcon();
		static void setInfobarMessage(const char* msg, int icon = 0);
	protected:
		static ZQWidget* interface;
		static ZQWidget* infobar;
		static ZDLConf *activeConfig;
		static string cdir;
};

#endif
