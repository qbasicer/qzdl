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
#include "ZQWidget.h"
#include "ZUpdater.h"

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
		static QString getConfigFileName();
		static void setConfigFileName(QString name);
		static void setUpdater(ZUpdater *zup);
		static ZUpdater *getUpdater();
	protected:
		static QString filename;
		static ZQWidget* interface;
		static ZQWidget* infobar;
		static ZDLConf *activeConfig;
		static string cdir;
		static ZUpdater *zupper;
};

#endif
