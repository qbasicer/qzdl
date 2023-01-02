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

#include "ZDLInterface.h"
#include "ZDLMainWindow.h"
#include "zdlconf.h"
#include "zdlcommon.h"
#include "ZDLInfoBar.h"
#include "xpm/ico_icon.xpm"
#include <QDebug>

#include "disabled.h"

extern QApplication *qapp;
extern QString versionString;

ZDLMainWindow::~ZDLMainWindow(){
	QSize sze = this->size();
	QPoint pt = this->pos();
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
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

	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
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
        settings->notifyFromParent(nullptr);
        intr->readFromParent(nullptr);
	}else if (newTab == 1){
        intr->notifyFromParent(nullptr);
        settings->readFromParent(nullptr);
	}
}

void ZDLMainWindow::quit(){
	writeConfig();
	close();
}

void ZDLMainWindow::launch(){
	writeConfig();
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();

	QString exec = getExecutable();
	if (exec.isNull() || exec.length() < 1){
		QMessageBox::critical(this, "ZDL", "Please select a source port");
		return;
	}
	int customArgStart;
	QStringList args = getArguments(&customArgStart);
	if (args.join("").length() < 1){
		return;
	}

	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

	// Custom arguments appear at the end of the list
	int commandIndex = 0;

	for (int i = customArgStart; i < args.size(); i++)
	{
		const QString arg = args[i];
		if (arg.toLower() == "%command%")
		{
			if (commandIndex == 0) { // Only accept the first "%command%"
				commandIndex = i - customArgStart;
				// qInfo() << "commandIndex:" << commandIndex;
			}
			args.removeAt(i--);
		}
	}

	QString gameExecutable = exec;
	if (commandIndex) {
		bool parsingEnvVars = true;
		int preInsertIndex = 0;
		// Given extra arguments: DXVK_HUD=api,fps mangohud --dlsym %command% abc
		// Given "always add" arguments: +sv_cheats 1
		// Expected executable: mangohud
		// Expected arguments: --dlsym <gzdoom> -iwad <IWAD> abc +sv_cheats 1
		for (int argi = customArgStart; commandIndex; argi++, commandIndex--)
		{
			const QString arg = args[argi];
			int eqpos = arg.indexOf('=');
			if (eqpos != -1 && parsingEnvVars)
			{
				QString enkey = arg.left(eqpos);
				QString enval = arg.mid(eqpos + 1);
				env.insert(enkey, enval);
				// qInfo() << enkey << ":" << enval;
				args.removeAt(argi--);
			}
			else if (gameExecutable == exec)
			{
				// Insert executable into args
				gameExecutable = exec;
				exec = arg;
				parsingEnvVars = false;
				args.removeAt(argi--);
			}
			else // if (commandIndex)
			{ // Before the %command%
				args.removeAt(argi);
				args.insert(preInsertIndex++, arg);
			}
			// Nothing else needs to be done, since the arguments after the
			// first %command% are already at the end of the list.
		}
		args.insert(preInsertIndex++, gameExecutable);
	}

	/*
	qInfo() << "Environment variables:" << env.toStringList();
	qInfo() << "args:";
	for(const QString arg : args) {
		qInfo() << arg;
	}
	qInfo() << "exec:" << exec;
	qInfo() << "gameExecutable:" << gameExecutable;
	return;
	*/

	//Find the executable
	QStringList executablePath = gameExecutable.split(QDir::separator());

	//Remove the last item, which will be the .exe
	executablePath.removeLast();

	//Re-create the string
	QString workingDirectory = executablePath.join(QDir::separator());

	//Resolve the path to an absolute directory
	QDir cwd(workingDirectory);
	workingDirectory = cwd.absolutePath();
	// Turns on launch confirmation
	QProcess *proc = new QProcess(this);

	//Set the working directory to that directory
	proc->setWorkingDirectory(workingDirectory);
	proc->setProcessEnvironment(env);
	proc->setProcessChannelMode(QProcess::ForwardedChannels);


	auto bar = intr->getInfobar();
	bar->setVisible(false);

	setEnabled(false);

    connect(proc, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) {
        Q_UNUSED(exitStatus)
        setEnabled(true);

		if (exitCode)
		{
			intr->setInfobarMessage("Failed to launch the process!", 1);
			connect(bar, &ZDLInfoBar::moreclicked, [=]() {
				// badLaunch message box
                badLaunch(proc->error());
			});
		}
		else if (zconf->contains("zdl.general/autoclose"))
		{
			QString append = zconf->value("zdl.general/autoclose").toString();
			if (append == "1" || append == "true")
			{
				// Asked to exit... closing
				close();
			}
		}
	});

    connect(proc, static_cast<void(QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred), [=](QProcess::ProcessError error) {
        Q_UNUSED(error)
        setEnabled(true);
        intr->setInfobarMessage("Failed to launch the process!", 1);
        connect(bar, &ZDLInfoBar::moreclicked, [=]() {
            // badLaunch message box
            badLaunch(proc->error());
        });
    });


	proc->start(exec, args);
}

void ZDLMainWindow::badLaunch(QProcess::ProcessError procerr){
    switch(procerr)
    {
    case QProcess::FailedToStart:
        QMessageBox::warning(nullptr, "Failed to Start", "Failed to launch the application executable.", QMessageBox::Ok, QMessageBox::Ok);
        break;

    case QProcess::Crashed:
        QMessageBox::warning(nullptr, "Process Crashed", "The application ended abnormally (usually due to a crash or error).", QMessageBox::Ok, QMessageBox::Ok);
        break;

    default:
        QMessageBox::warning(nullptr, "Unknown error", "There was a problem running the application.", QMessageBox::Ok, QMessageBox::Ok);
    }
}

QStringList ZDLMainWindow::getArguments(int* customArgStart){
	QStringList ourString;
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();

	QString iwadName = "";

	bool ok;

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

	QString disabled;
	if (zconf->contains(disabledKey))
	{
		disabled = zconf->value(disabledKey).toString();
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

		if (disabledScan(disabled, i))
		{
			continue;
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
				ourString << "-deathmatch";
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

	if (customArgStart)
	{
		*customArgStart = ourString.size();
	}

	if (zconf->contains("zdl.save/extra")){
        ourString += parseExtraArgs(zconf->value("zdl.save/extra").toString());
	}

	if (zconf->contains("zdl.general/alwaysadd")){
        ourString += parseExtraArgs(zconf->value("zdl.general/alwaysadd").toString());
	}

	return ourString;
}

QString ZDLMainWindow::getExecutable(){
	// Getting exec
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
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


QStringList parseExtraArgs(QString arg_str)
{
    // Parses the -alwaysAdd and -extraArgs strings into
    // a list of tokens that can be added to a QProcess' arguments array.
    // Note: no shell variable expansion or backslash escapes.

    int length = arg_str.length();
    QStringList tokens;
    QStack<QChar> quotes;
    QString token{""};

    for (int i = 0; i < length; i++)
    {
        QChar c = arg_str.at(i);

        if (quotes.size() == 0 && (c == ' ' || c == '\t'))
        {
            if (token.length() != 0)
            {
                tokens.push_back(token);
            }
            token = "";
            continue;
        }

        if (c == '"' || c == '"')
        {
            if (!quotes.isEmpty() && c == quotes.top())
            {
                quotes.pop();
                tokens.push_back(token);
                continue;
            }
            else
            {
                quotes.push(c);
                if (quotes.size() == 1)
                {
                    continue;
                }
            }
        }

        token.append(c);

        if (i == length - 1)
        {
            tokens.push_back(token);
        }
    }

    return tokens;
}

