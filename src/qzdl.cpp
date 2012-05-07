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

#include "ZDLNullDevice.h"

QApplication *qapp;
QString versionString;
ZDLMainWindow *mw;

static void addFile(QString file, ZDLConf* zconf){
	LOGDATA() << "Adding " << file << " to " << (void*)zconf << endl;
	ZDLSection *section = zconf->getSection("zdl.save");
	if(!section){
		zconf->setValue("zdl.save", "file0", file);
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

QDebug *zdlDebug;

#if defined(Q_WS_WIN)
__declspec(dllimport) int qt_ntfs_permission_lookup;
#endif

int main( int argc, char **argv ){
	QStringList args;
	for(int i = 1; i < argc; i++){
		args << QString(argv[i]);
	}
	QStringList eatenArgs(args);
	ZDLNullDevice nullDev;
#if defined(ZDL_BLACKBOX)
	QFile *loggingFile = NULL;
	zdlDebug = NULL;
	int logger = eatenArgs.indexOf("--enable-logger");
	
	if(logger >= 0){
		eatenArgs.removeAt(logger);
		loggingFile = new QFile("zdl.log");
		if(loggingFile->exists()){
			loggingFile->remove();
		}
		loggingFile->open(QIODevice::ReadWrite);
		zdlDebug = new QDebug(loggingFile);
		qDebug() << "Logger is enabled";
	}else{
		zdlDebug = new QDebug(&nullDev);
	}
#else
	zdlDebug = new QDebug(&nullDev);
#endif

	LOGDATA() << ZDL_ENGINE_NAME << " booting at " << QDateTime::currentDateTime().toString() << endl;

#if defined(Q_WS_WIN)
	qt_ntfs_permission_lookup = 0;
#endif

	QApplication a( argc, argv );
	qapp = &a;
	ZDLConfigurationManager::setArgv(args);
	{
		QString execuatble(argv[0]);
#if defined(Q_WS_WIN)
		execuatble.replace("\\", "/");
#endif
		QFileInfo fullPath(execuatble);
		LOGDATA() << "Executable path: " << fullPath.absoluteFilePath() << endl;
		ZDLConfigurationManager::setExec(fullPath.absoluteFilePath());
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
	LOGDATA() << "ZDL Version" << versionString << endl;
	LOGDATA() << "Source: " << ZDL_SOURCE << endl;
	LOGDATA() << "Build: " << ZDL_BUILD << endl;
	LOGDATA() << "Revision: " << ZDL_REVISION << endl;
#if defined(ZDL_BUILD_NUMBER)
	if(ZDL_BUILD_NUMBER > 0){
		LOGDATA() << "Build #: " << QString::number(ZDL_BUILD_NUMBER) << endl;
	}
#endif
#if defined(ZDL_BUILD_JOB)
	LOGDATA() << "Build job: " << ZDL_BUILD_JOB << endl;
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

	ZDLConfigurationManager::setWhy(ZDLConfigurationManager::UNKNOWN);

	//If the user has specified an alternative .ini
	for(int i = 0; i < eatenArgs.size(); i++){
		if(eatenArgs[i].endsWith(".ini", Qt::CaseInsensitive)){
			LOGDATA() << "Loading command line INI configuration " << eatenArgs[i] << endl;
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
						LOGDATA() << "Using user-level config file at " << userConfPath << endl;
					}else{
						LOGDATA() << "Config file specified nouserconf" << endl;
					}			
				}else{
					LOGDATA() << "User config file is small" << endl;
				}
			}else{
				LOGDATA() << "User conf file doesn't exist" << endl;
			}
		}else{
			LOGDATA() << "No conf yet" << endl;
		}
	}

	if(ZDLConfigurationManager::getConfigFileName().length() == 0){
		QString exec = ZDLConfigurationManager::getExec();
		QStringList path = exec.split("/");
		path.removeLast();
		if(QFile::exists(path.join("/")+"/zdl.ini")){
			LOGDATA() << "Using zdl.ini at " << (path.join("/")+"/zdl.ini") << endl;
			ZDLConfigurationManager::setConfigFileName(path.join("/")+"/zdl.ini");
		}
	}

	if(ZDLConfigurationManager::getConfigFileName().isEmpty()){
		ZDLConfiguration *conf = ZDLConfigurationManager::getConfiguration();
		if(conf){
			ZDLConfigurationManager::setConfigFileName(conf->getPath(ZDLConfiguration::CONF_USER));
			LOGDATA() << "Falling back on user config at " << conf->getPath(ZDLConfiguration::CONF_USER) << endl;
		}else{
			ZDLConfigurationManager::setConfigFileName("zdl.ini");
			LOGDATA() << "No conf, going to have to use local zdl.ini" << endl;
		}
	}

	tconf->readINI(ZDLConfigurationManager::getConfigFileName());
	ZDLConfigurationManager::setActiveConfiguration(tconf);

	bool hasZDLFile = false;

	for(int i = 0; i < eatenArgs.size(); i++){
		if(eatenArgs[i].endsWith(".zdl", Qt::CaseInsensitive)){
			LOGDATA() << "Found a .zdl on the command line, replacing current zdl.save" << endl;
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
		LOGDATA() << "A .zdl file as passed as a command line option" << endl;
		if(tconf->hasValue("zdl.general", "zdllaunch")){
			int ok = 0;
			QString rc = tconf->getValue("zdl.general", "zdllaunch", &ok);
			if(rc.length() > 0){
				if(rc.compare("1") == 0){
					LOGDATA() << "Launching configuration NOW" << endl;
					mw->launch();
					LOGDATA() << "ZDL QUIT" << endl;
					return 0;
				}
			}
		}
	}

	if(zup){
		LOGDATA() << "Checking for updates" << endl;
		zup->fetch();
	}

	mw->handleImport();
	LOGDATA() << "-----------------------------------" << endl;
	int ret = a.exec();
	LOGDATA() << "-----------------------------------" << endl;
	LOGDATA() << "Starting shutdown" << endl;
	if (ret != 0){
		LOGDATA() << "ZDL QUIT, ERROR CONDITION" << endl;
		return ret;
	}
	mw->writeConfig();
	QString qscwd = ZDLConfigurationManager::getCurrentDirectory();
	tconf = ZDLConfigurationManager::getActiveConfiguration();
	QDir::setCurrent(qscwd);
	delete mw;
	tconf->writeINI(ZDLConfigurationManager::getConfigFileName());
	LOGDATA() << "ZDL QUIT" << endl;
	return ret;
}

