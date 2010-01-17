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

#include "configurationManager.h"
#include "mainWindow.h"

QApplication *qapp;
QString versionString;
mainWindow *mw;

int main( int argc, char **argv ){
    QApplication a( argc, argv );
	qapp = &a;


	versionString = ZDL_VERSION_STRING;
	
	QDir cwd = QDir::current();
	configurationManager::init();
	configurationManager::setCurrentDirectory(cwd.absolutePath().toStdString());

	ZUpdater *zup = new ZUpdater();
	
	ZDLConf* tconf = new ZDLConf();
	if (argc == 2){
		configurationManager::setConfigFileName(argv[1]);
	}else{
		configurationManager::setConfigFileName("zdl.ini");
	}
	tconf->readINI(configurationManager::getConfigFileName().toStdString().c_str());
	configurationManager::setActiveConfiguration(tconf);
	
	mw = new mainWindow();
	mw->setUpdater(zup);
	configurationManager::setUpdater(zup);
	mw->show();
	QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	mw->startRead();
	
	zup->fetch();
	
    int ret = a.exec();
	if (ret != 0){
		return ret;
	}
	mw->writeConfig();
	QString qscwd = configurationManager::getCurrentDirectory();
	tconf = configurationManager::getActiveConfiguration();
	QDir::setCurrent(qscwd);
	tconf->writeINI(configurationManager::getConfigFileName().toStdString().c_str());
	return ret;
}

