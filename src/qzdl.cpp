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

#include <QtWidgets>
#include <QApplication>
#include <QMainWindow>
#include <QDir>
#include <QCommandLineParser>

#include "ZDLMainWindow.h"
#include "ZDLVersion.h"
#include "zdlcommon.h"
#include "zdlconf.h"
#include <QSettings>

QApplication *qapp;
QString versionString;
ZDLMainWindow *mw;


int main( int argc, char **argv ){

	QStringList args;
	for(int i = 1; i < argc; i++){
		args << QString(argv[i]);
	}

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QApplication a( argc, argv );
	qapp = &a;


#if defined(Q_WS_WIN)
	versionString = ZDL_VERSION_STRING + QString(" (windows/") + QString(ZDL_BUILD)+QString(")");
#elif defined(Q_WS_MAC)
	versionString = ZDL_VERSION_STRING + QString(" (mac/") + QString(ZDL_BUILD)+QString(")");
#elif defined(Q_WS_X11)
	versionString = ZDL_VERSION_STRING + QString(" (linux/") + QString(ZDL_BUILD)+QString(")");
#else
	versionString = ZDL_VERSION_STRING + QString(" (other/") + QString(ZDL_BUILD)+QString(")");
#endif

	QCoreApplication::setApplicationName("qZDL");
	QCoreApplication::setOrganizationName("Vectec Software");
	QCoreApplication::setApplicationVersion(versionString);

	auto iniFormat = QSettings::registerFormat("ini", readZDLConf, writeZDLConf);
	QSettings *tconf{nullptr};
	auto portaConf = QCoreApplication::applicationDirPath() + "/qZDL.ini";
	if (QFileInfo(portaConf).isFile())
	{
		tconf = new QSettings(portaConf, iniFormat);
	}
	else
	{
		tconf = new QSettings(iniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
	}
	ZDLConfigurationManager settingsManager;
	settingsManager.setActiveConfiguration(tconf);

	bool hasZDLFile = false;

	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addPositionalArgument("zdl", "(optional) A .zdl file to load.");
	parser.addHelpOption();
	parser.process(a);
	auto positionalArgs = parser.positionalArguments();
	if (!positionalArgs.isEmpty())
	{
		QFileInfo zdlInfo(positionalArgs.first());
		if (zdlInfo.isFile() && zdlInfo.isReadable() && !QString::compare(zdlInfo.suffix(), "zdl", Qt::CaseInsensitive))
		{
			tconf->beginGroup("zdl.save");
			tconf->remove("");
			QSettings zdlFile(zdlInfo.absoluteFilePath(), iniFormat);
			zdlFile.beginGroup("zdl.save");
			for (auto key: zdlFile.allKeys())
			{
				tconf->setValue(key, zdlFile.value(key));
			}
			zdlFile.endGroup();
			tconf->endGroup();

			hasZDLFile = true;
		}
	}

	mw = new ZDLMainWindow();
	mw->show();

	QObject::connect(&a, &QApplication::lastWindowClosed, &a, &QApplication::quit);
	mw->startRead();

	if(hasZDLFile){
		//  A .zdl file as passed as a command line option
		if(tconf->contains("zdl.general/zdllaunch")){
			QString rc = tconf->value("zdl.general/zdllaunch").toString();
			if(rc.length() > 0){
				if(rc.compare("1") == 0){
					// Launching configuration NOW
					mw->launch();
					// ZDL QUIT
					return 0;
				}
			}
		}
	}

	int ret = a.exec();
	if (ret != 0){
		// ZDL QUIT, ERROR CONDITION
		return ret;
	}


	mw->writeConfig();

	delete mw;

	bool doSave = true;
	if (tconf->contains("zdl.general/rememberFilelist")){
		QString val = tconf->value("zdl.general/rememberFilelist").toString();
		if (val == "0"){
			doSave = false;
		} else {
			doSave = true;
		}
	}
	if (!doSave){
		for (int i = 0; ; i++)
		{
			auto key = QString("zdl.save/file%1").arg(i);
			if (!tconf->contains(key))
			{
				break;
			}
			tconf->remove(key);
		}
	}

	delete tconf;
	return ret;
}

