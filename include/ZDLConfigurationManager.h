/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * Copyright (C) 2018-2019  Lcferrum
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
#include <QComboBox>
#include <QValidator>
#include "ZDLWidget.h"
#include "ZDLConfiguration.h"
#include "ZDLConfigurationEvents.h"

#include "zdlcommon.h"

class ZDLConfigurationEvents;

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
		
		static void setCurrentDirectory(const QString &dir);
		static QString getCurrentDirectory();
		static QPixmap getIcon();
		static QString getConfigFileName();
		static void setConfigFileName(QString name);
		static QStringList getArgv();
		static void setArgv(QStringList args);
		static QString getExec();
		static void setExec(QString execu);
		static ZDLConfigurationEvents* getEvents();
	protected:
		static QString exec;
		static QStringList argv;
		static QString filename;
		static ZDLWidget* interface;
		static ZDLConf *activeConfig;
		static QString cdir;
		static ZDLConfiguration *conf;
		static WhyConfig why;
		static ZDLConfigurationEvents *events;
};

QString getLastDir(ZDLConf *zconf=NULL);
void saveLastDir(QString fileName, ZDLConf *zconf=NULL);
QString getWadLastDir(ZDLConf *zconf=NULL, bool dwd_first=false);
void saveWadLastDir(QString fileName, ZDLConf *zconf=NULL, bool is_dir=false);
QString getSrcLastDir(ZDLConf *zconf=NULL);
void saveSrcLastDir(QString fileName, ZDLConf *zconf=NULL);
QString getSaveLastDir(ZDLConf *zconf=NULL);
void saveSaveLastDir(QString fileName, ZDLConf *zconf=NULL);
QString getZdlLastDir(ZDLConf *zconf=NULL);
void saveZdlLastDir(QString fileName, ZDLConf *zconf=NULL);
QString getIniLastDir(ZDLConf *zconf=NULL);
void saveIniLastDir(QString fileName, ZDLConf *zconf=NULL);

class VerboseComboBox: public QComboBox {
	Q_OBJECT
public:
	VerboseComboBox(QWidget *parent=NULL): QComboBox(parent) {}
	virtual void showPopup();
	virtual void hidePopup();
signals:
	void onPopup();
	void onHidePopup();
};

class EvilValidator: public QValidator {
	Q_OBJECT
public:
    EvilValidator(QObject *parent): QValidator(parent) {}
	virtual QValidator::State validate(QString &input, int &pos) const;
};

#endif
