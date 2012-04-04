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
 
#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QDir>

#include "ZDLConfigurationManager.h"
#include "ZDLMainWindow.h"
#include "ZDLVersion.h"

QApplication *qapp;
QString versionString;
ZDLMainWindow *mw;

int main( int argc, char **argv ){
    	QApplication a( argc, argv );
	qapp = &a;
	QStringList args;
	for(int i = 1; i < argc; i++){
		args << QString(argv[i]);
	}

#if defined(Q_WS_WIN)
	versionString = ZDL_VERSION_STRING + QString(" (windows/") + QString(ZDL_BUILD)+QString(")");
#elif defined(Q_WS_MAC)
	versionString = ZDL_VERSION_STRING + QString(" (mac/") + QString(ZDL_BUILD)+QString(")");
#elif defined(Q_WS_X11)
	versionString = ZDL_VERSION_STRING + QString(" (linux/") + QString(ZDL_BUILD)+QString(")");
#else
	versionString = ZDL_VERSION_STRING + QString(" (other/") + QString(ZDL_BUILD)+QString(")");
#endif
	QDir cwd = QDir::current();
	ZDLConfigurationManager::init();
	ZDLConfigurationManager::setCurrentDirectory(cwd.absolutePath().toStdString());

	ZDLUpdater *zup = NULL;
#if !defined(NO_UPDATER)
	zup = new ZDLUpdater();
#endif
	
	ZDLConf* tconf = new ZDLConf();
	ZDLConfigurationManager::setConfigFileName("");

	//If the user has specified an alternative .ini
	if(args.size() > 0){
		if(args[args.size()-1].endsWith(".ini")){
			ZDLConfigurationManager::setConfigFileName(args[args.size()-1]);
		}
	}

	if(ZDLConfigurationManager::getConfigFileName().length() == 0){
		ZDLConfigurationManager::setConfigFileName("zdl.ini");
	}

	tconf->readINI(ZDLConfigurationManager::getConfigFileName());
	ZDLConfigurationManager::setActiveConfiguration(tconf);
	
	mw = new ZDLMainWindow();
	mw->setUpdater(zup);
	ZDLConfigurationManager::setUpdater(zup);
	mw->show();
	QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	mw->startRead();

	if(zup){	
		zup->fetch();
	}
	
    int ret = a.exec();
	if (ret != 0){
		return ret;
	}
	mw->writeConfig();
	QString qscwd = ZDLConfigurationManager::getCurrentDirectory();
	tconf = ZDLConfigurationManager::getActiveConfiguration();
	QDir::setCurrent(qscwd);
	tconf->writeINI(ZDLConfigurationManager::getConfigFileName().toStdString().c_str());
	return ret;
}

