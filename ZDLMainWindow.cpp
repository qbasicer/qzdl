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

#include <iostream>
#include <QtWidgets>
#include <QApplication>
#include <QMainWindow>

#include "ZDLInterface.h"
#include "ZDLMainWindow.h"
#include "zdlconf.h"
#include "zdlcommon.h"
#include "ZDLInfoBar.h"
#include "ico_icon.xpm"

extern QApplication *qapp;
extern QString versionString;

ZDLMainWindow::~ZDLMainWindow(){
	QSize sze = this->size();
	QPoint pt = this->pos();
	auto zconf = ZDLSettingsManager::getInstance();
	if(zconf){
		QString str = QString("%1,%2").arg(sze.width()).arg(sze.height());
		zconf->setValue("zdl.general/windowsize", str);
		str = QString("%1,%2").arg(pt.x()).arg(pt.y());
		zconf->setValue("zdl.general/windowpos", str);
	}
	// Closing main window
}

QString ZDLMainWindow::getWindowTitle(){
	QString windowTitle = "ZDL";
	windowTitle += " " ZDL_VERSION_STRING;
	// Returning main window title
	return windowTitle;

}

ZDLMainWindow::ZDLMainWindow(QWidget *parent): QMainWindow(parent){
	QString windowTitle = getWindowTitle();
	setWindowTitle(windowTitle);

	setWindowIcon(QPixmap(zdlicon));


	setContentsMargins(2,2,2,2);
	layout()->setContentsMargins(2,2,2,2);
	QTabWidget *widget = new QTabWidget(this);

	auto zconf = ZDLSettingsManager::getInstance();
	if(zconf){
		bool qtok = false;
		if(zconf->contains("zdl.general/windowsize")){
			QString size = zconf->value("zdl.general/windowsize").toString();
			if(size.contains(",")){
				QStringList list = size.split(",");
				int w = list[0].toInt(&qtok);
				if(qtok){
					int h = list[1].toInt(&qtok);
					if(qtok){
						this->resize(QSize(w,h));
					}
				}
			}
		}
		if(zconf->contains("zdl.general/windowpos")){
			QString size = zconf->value("zdl.general/windowpos").toString();
			if(size.contains(",")){
				QStringList list = size.split(",");
				int x = list[0].toInt(&qtok);
				if(qtok){
					int y = list[1].toInt(&qtok);
					if(qtok){
						this->move(QPoint(x,y));
					}
				}
			}

		}
	}


	intr = new ZDLInterface(this);
	settings = new ZDLSettingsTab(this);

	setCentralWidget(widget);
	widget->addTab(intr, "Main");
	widget->addTab(settings, "Settings");

	QAction *qact = new QAction(widget);
	qact->setShortcut(Qt::Key_Return);
	widget->addAction(qact);
	connect(qact, &QAction::triggered, this, &ZDLMainWindow::launch);

	qact2 = new QAction(widget);
	qact2->setShortcut(Qt::Key_Escape);
	widget->addAction(qact2);

	connect(qact2, &QAction::triggered, this, &ZDLMainWindow::quit);

	connect(widget, &QTabWidget::currentChanged, this, &ZDLMainWindow::tabChange);
	// Main window created.
}

void ZDLMainWindow::tabChange(int newTab){
	if(newTab == 0){
		settings->notifyFromParent(NULL);
		intr->readFromParent(NULL);
	}else if (newTab == 1){
		intr->notifyFromParent(NULL);
		settings->readFromParent(NULL);
	}
}

void ZDLMainWindow::quit(){
	writeConfig();
	close();
}

void ZDLMainWindow::launch(){
	writeConfig();
	auto zconf = ZDLSettingsManager::getInstance();

	QString exec = getExecutable();
	if (exec.isNull() || exec.length() < 1){
		QMessageBox::critical(this, "ZDL", "Please select a source port");
		return;
	}
	QStringList args = getArguments();
	if (args.join("").length() < 1){
		return;
	}

	if(exec.contains("\\")){
		exec.replace("\\","/");
	}

	//Find the executable
	QStringList executablePath = exec.split("/");

	//Remove the last item, which will be the .exe
	executablePath.removeLast();

	//Re-create the string
	QString workingDirectory = executablePath.join("/");

	//Resolve the path to an absolute directory
	QDir cwd(workingDirectory);
	workingDirectory = cwd.absolutePath();
	// Turns on launch confirmation
	QProcess *proc = new QProcess(this);

	//Set the working directory to that directory
	proc->setWorkingDirectory(workingDirectory);

	proc->setProcessChannelMode(QProcess::ForwardedChannels);
	proc->start(exec, args);
	procerr = proc->error();
	if (zconf->contains("zdl.general/autoclose")){
		QString append = zconf->value("zdl.general/autoclose").toString();
		if (append == "1" || append == "true"){
			// Asked to exit... closing;
			close();
		}
	}
}

void ZDLMainWindow::badLaunch(){
	if(procerr == QProcess::FailedToStart){
		QMessageBox::warning(NULL,"Failed to Start", "Failed to launch the application executable.",QMessageBox::Ok,QMessageBox::Ok);
	}else if(procerr == QProcess::Crashed){
		QMessageBox::warning(NULL,"Process Crashed", "The application ended abnormally (usually due to a crash or error).",QMessageBox::Ok,QMessageBox::Ok);
	}else{
		QMessageBox::warning(NULL,"Unknown error", "There was a problem running the application.",QMessageBox::Ok,QMessageBox::Ok);
	}
}

QStringList ZDLMainWindow::getArguments(){
	QStringList ourString;
	auto zconf = ZDLSettingsManager::getInstance();

	QString iwadName = "";

	bool ok;
	int doquotes = 1;

	if(zconf->contains("zdl.save/iwad")){
		QString rc = zconf->value("zdl.save/iwad").toString();
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
	for (int i = 0; ; i++)
	{
		auto nameKey = QString("zdl.iwads/i%1n").arg(i);
		auto fileKey = QString("zdl.iwads/i%1f").arg(i);
		if (!zconf->contains(nameKey) || !zconf->contains(fileKey))
		{
			break;
		}

		if (zconf->value(nameKey).toString().compare(iwadName) == 0)
		{
			ourString << "-iwad" << zconf->value(fileKey).toString();
			break;
		}
	}

	if (zconf->contains("zdl.save/skill")){
		ourString << "-skill";
		ourString << zconf->value("zdl.save/skill").toString();
	}

	if (zconf->contains("zdl.save/warp")){
		ourString << "+map";
		ourString << zconf->value("zdl.save/warp").toString();
	}

	if (zconf->contains("zdl.save/dmflags")){
		ourString << "+set";
		ourString << "dmflags";
		ourString << zconf->value("zdl.save/dmflags").toString();
	}

	if (zconf->contains("zdl.save/dmflags2")){
		ourString << "+set";
		ourString << "dmflags2";
		ourString << zconf->value("zdl.save/dmflags2").toString();
	}

	QStringList pwads;
	QStringList dhacked;
	for (int i = 0; ; i++)
	{
		auto key = QString("zdl.save/file%1").arg(i);
		if (!zconf->contains(key))
		{
			break;
		}
		auto file = QFileInfo(zconf->value(key).toString());
		if (file.suffix().compare("deh", Qt::CaseInsensitive) == 0 || file.suffix().compare("bex", Qt::CaseInsensitive) == 0)
		{
			dhacked << file.filePath();
		}
		else
		{
			pwads << file.filePath();
		}

	}

	if(pwads.size() > 0){
		ourString << "-file";
		ourString << pwads;
	}

	if(dhacked.size() > 0){
		ourString << "-deh";
		ourString << dhacked;
	}

	if(zconf->contains("zdl.save/gametype")){
		QString tGameType = zconf->value("zdl.save/gametype").toString();
		if(tGameType != "0"){
			if (tGameType == "2"){
				ourString << "-deathmath";
			}
			int players = 0;
			if(zconf->contains("zdl.save/players")){
				QString tPlayers = zconf->value("zdl.save/players").toString();
				players = tPlayers.toInt(&ok, 10);
			}
			if(players > 0){
				ourString << "-host";
				ourString << QString::number(players);
			}else if(players == 0){
				if(zconf->contains("zdl.save/host")){
					ourString << "-join";
					ourString << zconf->value("zdl.save/host").toString();
				}
			}
			if(zconf->contains("zdl.save/fraglimit")){
				ourString << "+set";
				ourString << "fraglimit";
				ourString << zconf->value("zdl.save/fraglimit").toString();

			}
		}
	}



	if(zconf->contains("zdl.net/advenabled")){
		QString aNetEnabled = zconf->value("zdl.net/advenabled").toString();
		if(aNetEnabled == "enabled"){
			if(zconf->contains("zdl.net/port")){
				ourString << "-port";
				ourString << zconf->value("zdl.net/port").toString();
			}
			if(zconf->contains("zdl.net/extratic")){
				QString tExtratic = zconf->value("zdl.net/extratic").toString();
				if(tExtratic == "enabled"){
					ourString << "-extratic";
				}
			}
			if(zconf->contains("zdl.net/netmode")){
				QString tNetMode = zconf->value("zdl.net/netmode").toString();
				if(tNetMode == "1"){
					ourString << "-netmode";
					ourString << "0";
				}else if(tNetMode == "2"){
					ourString << "-netmode";
					ourString << "1";
				}
			}
			if(zconf->contains("zdl.net/dup")){
				QString tDup = zconf->value("zdl.net/dup").toString();
				if(tDup != "0"){
					ourString << "-dup";
					ourString << tDup;
				}
			}
		}
	}

	// This is always true. There was supposed to be a zdl.general/quotefiles key, which could have a value of "disabled", but it
	// was never implemented.
	if(doquotes){
		for(int i = 0; i < ourString.size(); i++){
			if(ourString[i].contains(" ")){
				QString newString = QString("\"") + ourString[i] + QString("\"");
				ourString[i] = newString;
			}
		}
	}

	if (zconf->contains("zdl.general/alwaysadd")){
		ourString << zconf->value("zdl.general/alwaysadd").toString();
	}

	if (zconf->contains("zdl.save/extra")){
		ourString << zconf->value("zdl.save/extra").toString();
	}

	return ourString;
}

QString ZDLMainWindow::getExecutable(){
	// Getting exec
	auto zconf = ZDLSettingsManager::getInstance();
	QString portName = "";
	if(zconf->contains("zdl.save/port")){
		portName = zconf->value("zdl.save/port").toString();
	}

	for (int i = 0; ; i++)
	{
		auto nameKey = QString("zdl.ports/p%1n").arg(i);
		auto fileKey = QString("zdl.ports/p%1f").arg(i);
		if (!zconf->contains(fileKey) ||!zconf->contains(nameKey))
		{
			break;
		}
		if (zconf->value(nameKey).toString().compare(portName) == 0)
		{
			return zconf->value(fileKey).toString();
		}
	}
	// No executable;
	return QString();
}


//Pass through functions.
void ZDLMainWindow::startRead(){
	intr->startRead();
	settings->startRead();
	QString windowTitle = getWindowTitle();
	setWindowTitle(windowTitle);
}

void ZDLMainWindow::writeConfig(){
	intr->writeConfig();
	settings->writeConfig();
}

QString ZDLMainWindow::getExtraArgs() const
{
	return intr->getExtraArgs();
}

QString ZDLMainWindow::getMode()
{
	return intr->getMode();
}

QString ZDLMainWindow::getHostAddy()
{
	return intr->getHostAddy();
}

QString ZDLMainWindow::getPlayers()
{
	return intr->getPlayers();
}

 QString ZDLMainWindow::getFragLmit()
 {
	 return intr->getFragLmit();
 }

 QString ZDLMainWindow::getDMFlags()
 {
	 return intr->getDMFlags();
 }

 QString ZDLMainWindow::getDMFlags2()
 {
	 return intr->getDMFlags2();
 }

 QString ZDLMainWindow::getAlwaysArgs()
 {
	 return settings->getAlwaysArgs();
 }

 Qt::CheckState ZDLMainWindow::getLaunchClose()
 {
	 return settings->getLaunchClose();
 }

 Qt::CheckState ZDLMainWindow::getShowPaths()
 {
	 return settings->getShowPaths();
 }

 Qt::CheckState ZDLMainWindow::getLaunchZDL()
 {
	 return settings->getLaunchZDL();
 }

 Qt::CheckState ZDLMainWindow::getSavePaths()
 {
	 return settings->getSavePaths();
 }
