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
 
#ifndef _configurationManger_h_
#define _configurationManger_h_
#include <iostream>
#include <list>
#include <string>
#include "ZDLWidget.h"
#include "ZDLUpdater.h"
#include "ZDLConfiguration.h"

using namespace std;

#include <zdlcommon.h>

class ZDLConfigurationManager{
	public:
		enum WhyConfig {UNKNOWN, USER_SPECIFIED, USER_CONF, IN_EXEC_DIR, IN_CWD};
		static void init();
		static ZDLWidget* getInterface();
		static void setInterface(ZDLWidget *widget);
		static void setWhy(WhyConfig conf);		
		static WhyConfig getWhy();	

		// Deprecating this soon!
		static void setActiveConfiguration(ZDLConf *zconf);
		static ZDLConf* getActiveConfiguration();
		
		static ZDLConfiguration* getConfiguration();
		
		static void setCurrentDirectory(string dir);
		static const char* getCurrentDirectory();
		static void setInfobar(ZDLWidget *zib);
		static ZDLWidget *getInfobar();
		static QPixmap getIcon();
		static void setInfobarMessage(const char* msg, int icon = 0);
		static QString getConfigFileName();
		static void setConfigFileName(QString name);
		static void setUpdater(ZDLUpdater *zup);
		static ZDLUpdater *getUpdater();
		static QStringList getArgv();
		static void setArgv(QStringList args);
		static QString getExec();
		static void setExec(QString execu);
	protected:
		static QString exec;
		static QStringList argv;
		static QString filename;
		static ZDLWidget* interface;
		static ZDLWidget* infobar;
		static ZDLConf *activeConfig;
		static string cdir;
		static ZDLUpdater *zupper;
		static ZDLConfiguration *conf;
		static WhyConfig why;
};

#endif
