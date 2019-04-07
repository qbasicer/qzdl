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

#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QDir>

#include "ZDLConfigurationManager.h"
#include "ZDLMainWindow.h"

#include "ZDLNullDevice.h"

#if defined(Q_WS_WIN)
#include "windows.h"
#endif

QApplication *qapp;
ZDLMainWindow *mw;

void clearFiles(ZDLConf* zconf)
{
	ZDLSection *section = zconf->getSection("zdl.save");
	if (section){
		QVector <ZDLLine*> vctr;
		section->getRegex("^file[0-9]+d?$", vctr);
		for(int i = 0; i < vctr.size(); i++){
			zconf->deleteValue("zdl.save", vctr[i]->getVariable());
		}
	}
}

void addFile(QString file, ZDLConf* zconf)
{
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
}

QDebug *zdlDebug;

#if defined(Q_WS_WIN)
	extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

int main( int argc, char **argv ){
	QStringList eatenArgs;
	for(int i = 1; i < argc; i++){
		eatenArgs << argv[i];
	}
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
	LOGDATA() << "ZDL" << " booting at " << QDateTime::currentDateTime().toString() << endl;

#if defined(Q_WS_MAC)
	QFont::insertSubstitution(".Lucida Grande UI", "Lucida Grande");
#endif

    QApplication::setGraphicsSystem("native");
	QApplication a( argc, argv );
	qapp = &a;
	ZDLConfigurationManager::setArgv(eatenArgs);
	{
		QFileInfo fullPath(argv[0]);
		LOGDATA() << "Executable path: " << fullPath.canonicalFilePath() << endl;
		ZDLConfigurationManager::setExec(fullPath.canonicalFilePath());
	}

	LOGDATA() << "ZDL Version: " << ZDL_PRIVATE_VERSION_STRING << endl;
	LOGDATA() << "Built on " << __DATE__ << " at " << __TIME__ <<endl;
#if ZDL_DEV_BUILD==1
	LOGDATA() << "This is development build" <<endl;
#endif

	QDir cwd = QDir::current();
	ZDLConfigurationManager::init();
	ZDLConfigurationManager::setCurrentDirectory(cwd.absolutePath());

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

    if(ZDLConfigurationManager::getConfigFileName().isEmpty()){
        QDir ini_dir(QFileInfo(ZDLConfigurationManager::getExec()).dir());
        if (ini_dir.exists("zdl.ini")) {
            LOGDATA() << "Using zdl.ini at " << ini_dir.filePath("zdl.ini") << endl;
            ZDLConfigurationManager::setConfigFileName(ini_dir.filePath("zdl.ini"));
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

	bool clear_on_args=true;
	bool hasZDLFile=false;
	foreach (const QString &item, eatenArgs) {
		if (item.endsWith(".zdl", Qt::CaseInsensitive)) {
			LOGDATA() << "Found a .zdl on the command line, replacing current zdl.save" << endl;
			tconf->deleteSectionByName("zdl.save");
			ZDLConf zdlFile;
			zdlFile.readINI(item);
			ZDLSection *section = zdlFile.getSection("zdl.save");
			if(section){
				tconf->addSection(section->clone());
				hasZDLFile=true;
				clear_on_args=false;
			}
			break;
		}
	}

	foreach (const QString &item, eatenArgs) {
		if (item.endsWith(".zdl", Qt::CaseInsensitive)||item.endsWith(".ini", Qt::CaseInsensitive)||item.startsWith("-"))
			continue;

		if (clear_on_args) {
			clearFiles(tconf);
			clear_on_args=false;
		}

		addFile(item, tconf);
	}

	mw = new ZDLMainWindow();
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
	// Set version information
	tconf->setValue("zdl.general", "engine", "ZDL");
	tconf->setValue("zdl.general", "version", ZDL_PRIVATE_VERSION_STRING);

	bool doSave = true;
	if (tconf->hasValue("zdl.general", "rememberFilelist")){
		int ok = 0;
		QString val = tconf->getValue("zdl.general", "rememberFilelist", &ok);
		if (val == "0"){
			doSave = false;
		} else {
			doSave = true;
		}
	}
	if (!doSave){
		tconf->deleteRegex("zdl.save", "^file[0-9]+$");
	}

	tconf->writeINI(ZDLConfigurationManager::getConfigFileName());
	LOGDATA() << "ZDL QUIT" << endl;
	return ret;
}

