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


static void addFile(QString file, ZDLConf* zconf){
	ZDLSection *section = zconf->getSection("zdl.save");
	if(!section){
		return;
	}
	QVector<ZDLLine*> vctr;
	section->getRegex("^file[0-9]+$", vctr);
	if(vctr.size() == 0){
		zconf->setValue("zdl.save", "file0", file);
		return;
	}
	QVector<int> numbers;
	for(int i = 0; i < vctr.size(); i++){
		bool ok = false;
		QString value = vctr[i]->getVariable();
		value.remove(0,4);
		int val = value.toInt(&ok);
		if(!ok){
			return;
		}
		numbers.push_back(val);
	}
	qSort(numbers);
	int highest = numbers.last();
	zconf->setValue("zdl.save", "file"+QString::number(highest+1), file);
	return;
}

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
	QStringList eatenArgs(args);

	ZDLConfigurationManager::setWhy(ZDLConfigurationManager::UNKNOWN);

	//If the user has specified an alternative .ini
	for(int i = 0; i < eatenArgs.size(); i++){
		if(eatenArgs[i].endsWith(".ini", Qt::CaseInsensitive)){
			ZDLConfigurationManager::setConfigFileName(eatenArgs[i]);
			eatenArgs.removeAt(i);
			ZDLConfigurationManager::setWhy(ZDLConfigurationManager::USER_SPECIFIED);
			break;
		}
	}

	if(ZDLConfigurationManager::getConfigFileName().isEmpty()){
		ZDLConfiguration *conf = ZDLConfigurationManager::getConfiguration();
		if(conf){
			QString userConfPath = conf->getPath(ZDLConfiguration::CONF_USER);
			if(QFile::exists(userConfPath)){
				QFile cfile(userConfPath);
				if(cfile.size() > 20){
					ZDLConf conf;
					conf.readINI(userConfPath);
					if(!conf.hasValue("zdl.general","nouserconf")){
						ZDLConfigurationManager::setConfigFileName(userConfPath);
					}			
				}else{
				}
			}else{
			}
		}else{
		}
	}

	if(ZDLConfigurationManager::getConfigFileName().length() == 0){
		QString exec = argv[0];
#if defined(Q_WS_WIN)
		exec = QString(exec.replace("\\","/"));
#endif
		QStringList path = exec.split("/");
		path.removeLast();
		if(QFile::exists(path.join("/")+"/zdl.ini")){
			ZDLConfigurationManager::setConfigFileName(path.join("/")+"/zdl.ini");
		}else if(QFile::exists("zdl.ini")){
			ZDLConfigurationManager::setConfigFileName("zdl.ini");
		}
	}

	if(ZDLConfigurationManager::getConfigFileName().isEmpty()){
		ZDLConfiguration *conf = ZDLConfigurationManager::getConfiguration();
		if(conf){
			ZDLConfigurationManager::setConfigFileName(conf->getPath(ZDLConfiguration::CONF_USER));
		}else{
			ZDLConfigurationManager::setConfigFileName("zdl.ini");
		}
	}

	tconf->readINI(ZDLConfigurationManager::getConfigFileName());
	ZDLConfigurationManager::setActiveConfiguration(tconf);

	bool hasZDLFile = false;

	for(int i = 0; i < eatenArgs.size(); i++){
		if(eatenArgs[i].endsWith(".zdl", Qt::CaseInsensitive)){
			tconf->deleteSectionByName("zdl.save");
			ZDLConf zdlFile;
			zdlFile.readINI(eatenArgs[i]);
			ZDLSection *section = zdlFile.getSection("zdl.save");
			if(section){
				tconf->addSection(section->clone());
				hasZDLFile = true;
			}
			eatenArgs.removeAt(i);
			break;
		}
	}


	for(int i = 0; i < eatenArgs.size(); i++){
		if(!(eatenArgs[i].endsWith(".zdl", Qt::CaseInsensitive) || eatenArgs[i].endsWith(".ini", Qt::CaseInsensitive))){
			addFile(eatenArgs[i], tconf);
			eatenArgs.removeAt(i);
			i--;
		}
	}


	mw = new ZDLMainWindow();
	mw->setUpdater(zup);
	ZDLConfigurationManager::setUpdater(zup);
	mw->show();
	QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	mw->startRead();

	if(hasZDLFile){
		if(tconf->hasValue("zdl.general", "zdllaunch")){
			int ok = 0;
			QString rc = tconf->getValue("zdl.general", "zdllaunch", &ok);
			if(rc.length() > 0){
				if(rc.compare("1") == 0){
					mw->launch();
					return 0;
				}
			}
		}
	}

	if(zup){	
		zup->fetch();
	}

	mw->handleImport();

	int ret = a.exec();
	if (ret != 0){
		return ret;
	}
	mw->writeConfig();
	QString qscwd = ZDLConfigurationManager::getCurrentDirectory();
	tconf = ZDLConfigurationManager::getActiveConfiguration();
	QDir::setCurrent(qscwd);
	tconf->writeINI(ZDLConfigurationManager::getConfigFileName());
	return ret;
}

