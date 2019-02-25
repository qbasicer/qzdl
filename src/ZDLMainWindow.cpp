/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * Copyright (C) 2018  Lcferrum
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

#include <iostream>
#include <QtGui>
#include <QApplication>
#include <QMainWindow>

#include "ZDLInterface.h"
#include "ZDLMainWindow.h"
#include "ZDLConfigurationManager.h"
#include "ZDLImportDialog.h"

#ifdef Q_WS_WIN
#include <windows.h>
#endif

extern QApplication *qapp;

ZDLMainWindow::~ZDLMainWindow(){
	QSize sze = this->size();
	QPoint pt = this->pos();
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(zconf){
		QString str = QString("%1,%2").arg(sze.width()).arg(sze.height());
		zconf->setValue("zdl.general", "windowsize", str);
		str = QString("%1,%2").arg(pt.x()).arg(pt.y());
		zconf->setValue("zdl.general", "windowpos", str);
	}
	LOGDATAO() << "Closing main window" << endl;
}

QString ZDLMainWindow::getWindowTitle(){
	QString windowTitle = "ZDL";
	windowTitle += " " ZDL_VERSION_STRING;
	ZDLConfiguration *conf = ZDLConfigurationManager::getConfiguration();
	if(conf){
		QString userConfPath = conf->getPath(ZDLConfiguration::CONF_USER);
		QString currentConf = ZDLConfigurationManager::getConfigFileName();
		if(userConfPath != currentConf){
			windowTitle += " [" + ZDLConfigurationManager::getConfigFileName() + "]";
		}
	}else{
		windowTitle += ZDLConfigurationManager::getConfigFileName();
	}
	LOGDATAO() << "Returning main window title " << windowTitle << endl;
	return windowTitle;

}

ZDLMainWindow::ZDLMainWindow(QWidget *parent): QMainWindow(parent){
	LOGDATAO() << "New main window " << DPTR(this) << endl;
	QString windowTitle = getWindowTitle();
	setWindowTitle(windowTitle);

	setWindowIcon(ZDLConfigurationManager::getIcon());

	setContentsMargins(0,2,0,0);
	layout()->setContentsMargins(0,0,0,0);
	QTabWidget *widget = new QTabWidget(this);

	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(zconf){
		int ok = 0;
		bool qtok = false;
		if(zconf->hasValue("zdl.general", "windowsize")){
			QString size = zconf->getValue("zdl.general", "windowsize", &ok);
			if(size.contains(",")){
				QStringList list = size.split(",");
				int w = list[0].toInt(&qtok);
				if(qtok){
					int h = list[1].toInt(&qtok);
					if(qtok){
						LOGDATAO() << "Resizing to w:" << w << " h:" << h << endl;
						this->resize(QSize(w,h));
					}
				}
			}
		}
		if(zconf->hasValue("zdl.general", "windowpos")){
			QString size = zconf->getValue("zdl.general", "windowpos", &ok);
			if(size.contains(",")){
				QStringList list = size.split(",");
				int x = list[0].toInt(&qtok);
				if(qtok){
					int y = list[1].toInt(&qtok);
					if(qtok){
						LOGDATAO() << "Moving to x:" << x << " y:" << y << endl;
						this->move(QPoint(x,y));
					}
				}
			}

		}
	}


	intr = new ZDLInterface(this);
	settings = new ZDLSettingsTab(this);

	//widget->setTabShape(QTabWidget::Triangular);
	//widget->setWindowFlags(Qt::FramelessWindowHint);
	widget->setDocumentMode(true);
	widget->addTab(intr, "Launch config");
	widget->addTab(settings, "General settings");
	setCentralWidget(widget);

	QAction *qact = new QAction(widget);
	qact->setShortcut(Qt::Key_Return);
	widget->addAction(qact);
	connect(qact, SIGNAL(triggered()), this, SLOT(launch()));

	qact2 = new QAction(widget);
	qact2->setShortcut(Qt::Key_Escape);
	widget->addAction(qact2);

	connect(qact2, SIGNAL(triggered()), this, SLOT(quit()));

	connect(widget, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
	LOGDATAO() << "Main window created." << endl;
}

void ZDLMainWindow::handleImport(){
#if !defined(NO_IMPORT)
	ZDLConfiguration *conf = ZDLConfigurationManager::getConfiguration();
	if(conf){
		QString userConfPath = conf->getPath(ZDLConfiguration::CONF_USER);
		QString currentConf = ZDLConfigurationManager::getConfigFileName();
		if(userConfPath != currentConf){
			LOGDATAO() << "Not currently using user conf" << endl;
			ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
			if(zconf->hasValue("zdl.general", "donotimportthis")){
				int ok = 0;
				if(zconf->getValue("zdl.general", "donotimportthis", &ok) == "1"){
					LOGDATAO() << "Don't import current config" << endl;
					return;
				}
			}
			QFile userFile(userConfPath);
			ZDLConf userconf;
			if(userFile.exists()){
				LOGDATAO() << "Reading user conf" << endl;
				userconf.readINI(userConfPath);
			}
			if(userconf.hasValue("zdl.general", "nouserconf")){
				int ok = 0;
				if(userconf.getValue("zdl.general", "nouserconf", &ok) == "1"){
					LOGDATAO() << "Do not use user conf" << endl;
					return;
				}
			}
			if(ZDLConfigurationManager::getWhy() == ZDLConfigurationManager::USER_SPECIFIED){
				LOGDATA() << "The user asked for this ini, don't go forward" << endl;
				return;
			}
			if(userFile.size() < 10){
				LOGDATA() << "User conf is small, assuming empty" << endl;
				ZDLImportDialog importd(this);
				importd.exec();
				if(importd.result() == QDialog::Accepted){
					switch(importd.getImportAction()){
						case ZDLImportDialog::IMPORTNOW:
							LOGDATAO() << "Importing now" << endl;
							if(!userFile.exists()){
								QStringList path = userConfPath.split("/");
								path.removeLast();
								QDir dir;
								if(!dir.mkpath(path.join("/"))){
									break;
								}
							}

							zconf->setValue("zdl.general", "importedfrom", currentConf);
							zconf->setValue("zdl.general", "isimported", "1");
							zconf->setValue("zdl.general", "importdate", QDateTime::currentDateTime().toString(Qt::ISODate));

							zconf->writeINI(userConfPath);
							ZDLConfigurationManager::setConfigFileName(userConfPath);
							break;
						case ZDLImportDialog::DONOTIMPORTTHIS:
							LOGDATAO() << "Tagging this config as not importable" << endl;
							zconf->setValue("zdl.general", "donotimportthis", "1");
							break;
						case ZDLImportDialog::NEVERIMPORT:
							LOGDATAO() << "Setting NEVERi IMPORT" << endl;
							userconf.setValue("zdl.general", "nouserconf", "1");
							if(!userFile.exists()){
								QStringList path = userConfPath.split("/");
								path.removeLast();
								QDir dir;
								if(!dir.mkpath(path.join("/"))){
									break;
								}

							}	
							userconf.writeINI(userConfPath);
							break;
						case ZDLImportDialog::ASKLATER:

						case ZDLImportDialog::UNKNOWN:
						default:
							LOGDATAO() << "Not setting anything" << endl;
							break;
					}
				}
			}
		}
	}
#endif
}

void ZDLMainWindow::tabChange(int newTab){
	LOGDATAO() << "Tab changed to " << newTab << endl;
	if(newTab == 0){
		settings->notifyFromParent(NULL);
		intr->readFromParent(NULL);
	}else if (newTab == 1){
		intr->notifyFromParent(NULL);
		settings->readFromParent(NULL);
	}
}

void ZDLMainWindow::quit(){
	LOGDATAO() << "quitting" << endl;
	writeConfig();
	close();
}

void ZDLMainWindow::launch(){
	LOGDATAO() << "Launching" << endl;
	writeConfig();
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();

	QString exec=getExecutable();
	if (exec.length() < 1){
		QMessageBox::critical(this, "ZDL", "Please select a source port");
		return;
	}
	QFileInfo exec_fi(exec);
	bool no_err=true;

#ifdef Q_WS_WIN
	PROCESS_INFORMATION pi={};
	STARTUPINFO si={sizeof(STARTUPINFO), NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, STARTF_USESHOWWINDOW, SW_SHOWNORMAL};

	QString cmdline="\""+QDir::toNativeSeparators(exec_fi.absoluteFilePath())+"\" "+getArguments(true).join(" ");
	QString cwd=QDir::toNativeSeparators(exec_fi.absolutePath());

	if (CreateProcess(NULL, const_cast<wchar_t*>(cmdline.toStdWString().c_str()), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, cwd.toStdWString().c_str(), &si, &pi)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	} else {
		QMessageBox::warning(NULL, "Failed to Start", "Failed to launch the application executable.", QMessageBox::Ok, QMessageBox::Ok);
		no_err=false;
	}
#else
	QProcess *proc=new QProcess(this);
	proc->setWorkingDirectory(exec_fi.absolutePath());
	proc->setProcessChannelMode(QProcess::ForwardedChannels);
	if (!proc->startDetached(exec_fi.absoluteFilePath(), getArguments())) {
		QMessageBox::warning(NULL, "Failed to Start", "Failed to launch the application executable.", QMessageBox::Ok, QMessageBox::Ok);
		no_err=false;
	}
#endif
	if (no_err) {
		QString aclose=zconf->getValue("zdl.general", "autoclose");
		if (aclose=="1"||aclose=="true"){
			LOGDATAO()<<"Asked to exit... closing"<<endl;
			close();
		}
	}
}

#define IF_NATIVE_SEP(p)	(native_sep?QDir::toNativeSeparators(p):p)

QStringList ZDLMainWindow::getArguments(bool native_sep){
	LOGDATAO() << "Getting arguments" << endl;
	QStringList ourString;
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = NULL;

	QString iwadName = "";

	bool ok;
	int stat;

	if(zconf->hasValue("zdl.save", "iwad")){
		QString rc = zconf->getValue("zdl.save", "iwad", &stat);
		if (rc.length() > 0){
			iwadName = rc;
		}else{
			QMessageBox::critical(this, "ZDL", "Please select an IWAD");
			return ourString;
		}
	}else{
		QMessageBox::critical(this, "ZDL", "Please select an IWAD");
		return ourString;
	}

	section = zconf->getSection("zdl.iwads");
	if (section){
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^i[0-9]+n$", fileVctr);

		for(int i = 0; i < fileVctr.size(); i++){
			ZDLLine *line = fileVctr[i];
			if(line->getValue().compare(iwadName) == 0){
				QString var = line->getVariable();
				if(var.length() >= 3){
					var = var.mid(1,var.length()-2);
					QVector<ZDLLine*> nameVctr;
					var = QString("i") + var + QString("f");
					section->getRegex("^" + var + "$",nameVctr);
					if(nameVctr.size() == 1){
						ourString << "-iwad";
						ourString << IF_NATIVE_SEP(nameVctr[0]->getValue());
					}
				}
			}
		}
	}

	if (zconf->hasValue("zdl.save", "skill")){
		ourString << "-skill";
		ourString << zconf->getValue("zdl.save", "skill", &stat);
	}

	if (zconf->hasValue("zdl.save", "warp")){
		ourString << "+map";
		ourString << zconf->getValue("zdl.save", "warp", &stat);
	}

	section = zconf->getSection("zdl.save");
	QStringList pwads;
	QStringList dehs;
	QStringList bexs;
	char deh_last=1;
	QStringList autoexecs;
	if (section){
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^file[0-9]+$", fileVctr);

		if (fileVctr.size() > 0){
			for(int i = 0; i < fileVctr.size(); i++){
				if(fileVctr[i]->getValue().endsWith(".bex",Qt::CaseInsensitive)) {
					deh_last=0;
					bexs << fileVctr[i]->getValue();
				} else if(fileVctr[i]->getValue().endsWith(".deh",Qt::CaseInsensitive)) {
					deh_last=1;
					dehs << fileVctr[i]->getValue();
				} else if(fileVctr[i]->getValue().endsWith(".cfg",Qt::CaseInsensitive)) {
					autoexecs << fileVctr[i]->getValue();
				} else {
					pwads << fileVctr[i]->getValue();
				}
			}
		}
	}

	if(pwads.size() > 0){
		ourString << "-file";
		foreach (const QString &str, pwads) {
			ourString << IF_NATIVE_SEP(str);
		}
	}

	do {
		if (deh_last%2) {
			foreach (const QString &str, bexs) {
				ourString << "-bex";
				ourString << IF_NATIVE_SEP(str);
			}
		} else {
			foreach (const QString &str, dehs) {
				ourString << "-deh";
				ourString << IF_NATIVE_SEP(str);
			}
		}
		deh_last+=3;
	} while (deh_last<=4);

    foreach (const QString &str, autoexecs) {
		ourString << "+exec";
		ourString << IF_NATIVE_SEP(str);
    }

	if(zconf->hasValue("zdl.save","gametype")){
		QString tGameType = zconf->getValue("zdl.save","gametype",&stat);
		if(tGameType != "0"){
			if (zconf->hasValue("zdl.save", "dmflags")){
				ourString << "+set";
				ourString << "dmflags";
				ourString << zconf->getValue("zdl.save", "dmflags", &stat);
			}

			if (zconf->hasValue("zdl.save", "dmflags2")){
				ourString << "+set";
				ourString << "dmflags2";
				ourString << zconf->getValue("zdl.save", "dmflags2", &stat);
			}

			if (tGameType == "2"){
				ourString << "-deathmath";
			} else if (tGameType == "3"){
				ourString << "-altdeath";
			}

			int players = 0;
			if(zconf->hasValue("zdl.save","players")){
				QString tPlayers = zconf->getValue("zdl.save","players",&stat);
				players = tPlayers.toInt(&ok, 10);
			}
			if(players > 0){
				ourString << "-host";
				ourString << QString::number(players);
				if(zconf->hasValue("zdl.save","mp_port")){
					ourString << "-port";
					ourString << zconf->getValue("zdl.save","mp_port",&stat);
				}
			}else if(players == 0){
				if(zconf->hasValue("zdl.save", "host")) {
					ourString<<"-join";
					if (zconf->hasValue("zdl.save", "mp_port")) {
						QRegExp trailing_port(":\\d*\\s*$");
						ourString<<zconf->getValue("zdl.save", "host", &stat).remove(trailing_port)+":"+zconf->getValue("zdl.save", "mp_port", &stat);
					} else {
						ourString<<zconf->getValue("zdl.save", "host", &stat);
					}
				}
			}
			if(zconf->hasValue("zdl.save","fraglimit")){
				ourString << "+set";
				ourString << "fraglimit";
				ourString << zconf->getValue("zdl.save","fraglimit",&stat);
			}
			if(zconf->hasValue("zdl.save","timelimit")){
				ourString << "+set";
				ourString << "timelimit";
				ourString << zconf->getValue("zdl.save","timelimit",&stat);
			}
			if(zconf->hasValue("zdl.save","extratic")){
				QString tVal = zconf->getValue("zdl.save","extratic",&stat);
				if(tVal == "1"){
					ourString << "-extratic";
				}
			}
			if(zconf->hasValue("zdl.save","netmode")){
				QString tVal = zconf->getValue("zdl.save","netmode",&stat);
				if(tVal != "-1"){
					ourString << "-netmode";
					ourString << tVal;
				}
			}
			if(zconf->hasValue("zdl.save","dup")){
				QString tVal = zconf->getValue("zdl.save","dup",&stat);
				if(tVal != "0"){
					ourString << "-dup";
					ourString << tVal;
				}
			}
			if(zconf->hasValue("zdl.save","savegame")){
				ourString << "-loadgame";
				ourString << IF_NATIVE_SEP(zconf->getValue("zdl.save","savegame",&stat));
			}
		}
	}

	for(int i = 0; i < ourString.size(); i++){
		if(ourString[i].contains(" ")){
			QString newString = "\"" + ourString[i] + "\"";
			ourString[i] = newString;
		}
	}

	if (zconf->hasValue("zdl.general", "alwaysadd")){
		ourString << zconf->getValue("zdl.general", "alwaysadd", &stat);
	}

	if (zconf->hasValue("zdl.save", "extra")){
		ourString << zconf->getValue("zdl.save", "extra", &stat);
	}
	LOGDATAO() << "args: " << ourString << endl;
	return ourString;
}

QString ZDLMainWindow::getExecutable(){
	LOGDATAO() << "Getting exec" << endl;
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	int stat;
	QString portName = "";
	if(zconf->hasValue("zdl.save", "port")){
		ZDLSection *section = zconf->getSection("zdl.ports");
		portName = zconf->getValue("zdl.save", "port", &stat);
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^p[0-9]+n$", fileVctr);

		for(int i = 0; i < fileVctr.size(); i++){
			ZDLLine *line = fileVctr[i];
			if(line->getValue().compare(portName) == 0){
				QString var = line->getVariable();
				if(var.length() >= 3){
					var = var.mid(1,var.length()-2);
					QVector<ZDLLine*> nameVctr;
					var = QString("p") + var + QString("f");
					section->getRegex("^" + var + "$",nameVctr);
					if(nameVctr.size() == 1){
						LOGDATAO() << "Executable: " << nameVctr[0]->getValue() << endl;
						return QString(nameVctr[0]->getValue());
					}
				}
			}
		}
	}
	LOGDATAO() << "No executable" << endl;
	return QString("");
}


//Pass through functions.
void ZDLMainWindow::startRead(){
	LOGDATAO() << "Starting to read configuration" << endl;
	intr->startRead();
	settings->startRead();
	QString windowTitle = getWindowTitle();
	setWindowTitle(windowTitle);
}

void ZDLMainWindow::writeConfig(){
	LOGDATAO() << "Writing configuration" << endl;
	intr->writeConfig();
	settings->writeConfig();
}
