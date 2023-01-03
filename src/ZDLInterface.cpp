/*
 * This file is part of qZDL
 * Copyright (C) 2007-2011  Cody Harris
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
#include "zdlconf.h"
#include "ZDLAdvancedMultiplayerDialog.h"
#include "ZDLAboutDialog.h"

#include "ZDLMultiPane.h"
#include "ZDLInterface.h"
#include "ZDLInfoBar.h"
#include "ZDLMainWindow.h"
#include "ZDLFilePane.h"
#include "ZDLSettingsPane.h"
#include "ZDLQSplitter.h"
#include "ZDMFlagPicker.h"

#include "xpm/aup.xpm"
#include "xpm/adown.xpm"

extern ZDLMainWindow *mw;
extern QApplication *qapp;

ZDLInterface::ZDLInterface(QWidget *parent):ZDLWidget(parent){

	box = new QVBoxLayout(this);


	QLayout *tpane = getTopPane();
	QLayout *bpane = getBottomPane();

	mpane = NULL;
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(0);
	zib = new ZDLInfoBar(this);
	box->addWidget(zib);

	box->addLayout(tpane);
	box->addLayout(bpane);
}

QLayout *ZDLInterface::getTopPane(){
	QHBoxLayout *box = new QHBoxLayout();

	ZDLQSplitter *split = new ZDLQSplitter(this);
	split->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ));
	QSplitter *rsplit = split->getSplit();


	ZDLFilePane *fpane = new ZDLFilePane(rsplit);
	ZDLSettingsPane *spane = new ZDLSettingsPane(rsplit);

	split->addChild(fpane);
	split->addChild(spane);
	box->setSpacing(2);
	box->addWidget(rsplit);
	return box;
}

QLayout *ZDLInterface::getBottomPane(){
	QVBoxLayout *box = new QVBoxLayout();
	QLabel *ecla = new QLabel("Extra command line arguments",this);
	ecla->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed));
	extraArgs = new QLineEdit(this);
	QLayout *pan = getButtonPane();
	layout()->setContentsMargins(0,0,0,0);
	setContentsMargins(0,0,0,0);
	box->setSpacing(2);
	box->addWidget(ecla);
	box->addWidget(extraArgs);
	box->addLayout(pan);
	return box;
}

void ZDLInterface::exitzdl(){
	mw->close();
}

QLayout *ZDLInterface::getButtonPane(){
	QHBoxLayout *box = new QHBoxLayout();

	QPushButton *btnExit = new QPushButton("Exit", this);
	btnZDL = new QPushButton("ZDL", this);
	QPushButton *btnMSet = new QPushButton("Multi Settings", this);
	btnEpr = new QPushButton(this);
	QPushButton *btnLaunch = new QPushButton("Launch", this);

	QMenu *context = new QMenu(btnZDL);

	QAction *showCommandline = context->addAction("Show Command Line");
	context->addSeparator();

	QAction *loadZdlFileAction = context->addAction("Load .zdl");
	loadZdlFileAction->setShortcut(QKeySequence::Open);
	QAction *saveZdlFileAction = context->addAction("Save .zdl");
	saveZdlFileAction->setShortcut(QKeySequence::Save);
	context->addSeparator();
	QAction *aboutAction = context->addAction("About");
	aboutAction->setShortcut(QKeySequence::HelpContents);

	connect(loadZdlFileAction, &QAction::triggered, this, &ZDLInterface::loadZdlFile);
	connect(saveZdlFileAction, &QAction::triggered, this, &ZDLInterface::saveZdlFile);
	connect(aboutAction, &QAction::triggered, this, &ZDLInterface::aboutClick);

	connect(showCommandline, &QAction::triggered, this, &ZDLInterface::showCommandline);
	connect(btnExit, &QPushButton::clicked, this, &ZDLInterface::exitzdl);

	btnZDL->setMenu(context);

	int minBtnWidth = 50;

	btnExit->setMinimumWidth(minBtnWidth-15);
	btnZDL->setMinimumWidth(minBtnWidth-15);
	btnMSet->setMinimumWidth(minBtnWidth+30);
	btnEpr->setMinimumWidth(20);
	btnLaunch->setMinimumWidth(minBtnWidth);

	connect(btnLaunch, &QPushButton::clicked, this, &ZDLInterface::launch);

	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(2);
	box->addWidget(btnExit);
	box->addWidget(btnZDL);
	box->addWidget(btnMSet);
	box->addWidget(btnEpr);
	box->addWidget(btnLaunch);
	box->setSpacing(1);
	connect(btnEpr, &QPushButton::clicked, this, &ZDLInterface::mclick);

	connect(btnMSet, &QPushButton::clicked, this, &ZDLInterface::ampclick);
	return box;
}

void ZDLInterface::showNewDMFlagger(){
	ZDMFlagPicker dialog(this);
	dialog.exec();
}

void ZDLInterface::launch(){
	mw->launch();
}

void ZDLInterface::buttonPaneNewConfig(){
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	if (zconf->contains("zdl.save/dlgmode")){
		if (zconf->value("zdl.save/dlgmode").toString().compare("open",Qt::CaseInsensitive) == 0){
			btnEpr->setIcon(QPixmap(adown));
		}else{
			btnEpr->setIcon(QPixmap(aup));
		}
	}else{
		btnEpr->setIcon(QPixmap(aup));
	}
}

void ZDLInterface::mclick(){
	writeConfig();
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(zconf->contains("zdl.save/dlgmode")){
		QString txt = zconf->value("zdl.save/dlgmode").toString();
		if(txt == "closed"){
			btnEpr->setIcon(QPixmap(adown));
			zconf->setValue("zdl.save/dlgmode", "open");
		}else{
			zconf->setValue("zdl.save/dlgmode", "closed");
			btnEpr->setIcon(QPixmap(aup));
		}
	}else{
		btnEpr->setIcon(QPixmap(adown));
		zconf->setValue("zdl.save/dlgmode", "open");
	}
	startRead();
}

void ZDLInterface::ampclick(){
	ZDLAdvancedMultiplayerDialog *zampd = new ZDLAdvancedMultiplayerDialog(this);
	zampd->exec();
	delete zampd;
}

void ZDLInterface::sendSignals(){
	rebuild();
	emit buildParent(this);
	emit buildChildren(this);
}

static QString getLastDir(){
	QString lastDir;
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	if (zconf->contains("zdl.general/lastDir")) {
		lastDir = zconf->value("zdl.general/lastDir").toString();
	} else {
		lastDir = QDir::currentPath();
	}
	return lastDir;
}

static void saveLastDir(QString fileName){
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	QFileInfo fi(fileName);
	zconf->setValue("zdl.general/lastDir", fi.absolutePath());
}

void ZDLInterface::loadZdlFile(){
	QStringList filters;
	QString filter;
	filters << "ZDL (*.zdl)" << "Any files (*)";
	filter = filters.join(";;");
	QString lastDir = getLastDir();
	QString fileName = QFileDialog::getOpenFileName(this, "Load ZDL", lastDir, filter);
	if(!fileName.isNull() && !fileName.isEmpty()){
		auto current = ZDLConfigurationManager::getActiveConfiguration();
		current->beginGroup("zdl.save");
		current->remove("");
		current->endGroup();
		auto iniFormat = QSettings::registerFormat("ini", readZDLConf, writeZDLConf);
		QSettings newConf(fileName, iniFormat);
		newConf.beginGroup("zdl.save");
		for (auto key : newConf.childKeys())
		{
			auto settingKey = "zdl.save/" + key;
			current->setValue(settingKey, newConf.value(key).toString());
		}
		newConf.endGroup();
		saveLastDir(fileName);
		mw->startRead();
	}
}

void ZDLInterface::saveZdlFile(){
	sendSignals();
	QStringList filters;
	filters << "ZDL (*.zdl)" << "Any files (*)";
	QString filter = filters.join(";;");
	QString lastDir = getLastDir();
	QString fileName = QFileDialog::getSaveFileName(this, "Save ZDL", lastDir, filter);
	if(!fileName.isNull() && !fileName.isEmpty()){
		auto current = ZDLConfigurationManager::getActiveConfiguration();
		auto iniFormat = QSettings::registerFormat("ini", readZDLConf, writeZDLConf);
		QSettings copy(fileName, iniFormat);
		current->beginGroup("zdl.save");
		for (auto key: current->childKeys())
		{
			QString settingKey{"zdl.save/" + key};
			copy.setValue(settingKey, current->value(key).toString());
		}
		current->endGroup();
		saveLastDir(fileName);
	}
}

void ZDLInterface::aboutClick(){
	ZDLAboutDialog zad(this);
	zad.exec();
}

void ZDLInterface::showCommandline(){
	writeConfig();

	QString exec = mw->getExecutable();
	if (exec.length() < 1){
		QMessageBox::critical(this, "ZDL", "Please select a source port");
		return;
	}
	QStringList args = mw->getArguments();
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
    QMessageBox launchConfirm(this);
    launchConfirm.setWindowTitle("Would you like to continue?");
    launchConfirm.setText("Executable: "+exec+"\n\nArguments: "+args.join(" ")+"\n\nWorking Directory: "+workingDirectory);
    launchConfirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    launchConfirm.setDefaultButton(QMessageBox::Yes);
    launchConfirm.setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    int btnrc = launchConfirm.exec();
    if(btnrc == QMessageBox::Yes){
        mw->launch();
        return;
    }
}

void ZDLInterface::rebuild(){
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(extraArgs->text().length() > 0){
		zconf->setValue("zdl.save/extra", extraArgs->text());

	}else{
		zconf->remove("zdl.save/extra");
	}
}

void ZDLInterface::bottomPaneNewConfig(){
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(zconf->contains("zdl.save/extra")){
		QString rc = zconf->value("zdl.save/extra").toString();
		if(rc.length() > 0){
			extraArgs->setText(rc);
		}
	}else{
		extraArgs->setText("");
	}
}

//Called when there's a change to the configuration that we need to look at.
//The button changed the configuration, and then notifies us that we need
//to look at the configuration to see what we need to do.
void ZDLInterface::newConfig(){
	buttonPaneNewConfig();
	bottomPaneNewConfig();
	//Grab our configuration
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	//Grab our section in the configuration
	//Do we have it?
	if (zconf->childGroups().contains("zdl.save")){
		if (zconf->contains("zdl.save/dlgmode")){
			//Are we open?
			if (zconf->value("zdl.save/dlgmode").toString().compare("open", Qt::CaseInsensitive) == 0){
				// If the multiplayer pane has not been created, create one
				if(mpane == NULL){
					mpane = new ZDLMultiPane(this);
					mpane->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Maximum ));
					mpane->newConfig();
				}
				box->addWidget(mpane);
				mpane->setVisible(true);
				//No?
			}else{
				if(mpane){
					box->removeWidget(mpane);
					mpane->setVisible(false);
					mpane->hide();
				}
			}
		}
		//Do we have the section, but not the key?
		else{
			if(mpane){
				box->removeWidget(mpane);
				mpane->setVisible(false);
			}
		}
		//Do we not even have the section?
	}else{
		if(mpane){
			box->removeWidget(mpane);
		}
	}
	this->update();
}



void ZDLInterface::startRead(){
	emit readChildren(this);
	newConfig();
}

void ZDLInterface::writeConfig(){
	rebuild();
	emit buildChildren(this);
}

ZDLInfoBar* ZDLInterface::getInfobar()
{
	return zib;
}

void ZDLInterface::setInfobarMessage(const char* message, int icon)
{
	zib->setMessage(message, icon);
}

QString ZDLInterface::getExtraArgs() const
{
	return extraArgs->text();
}

QString ZDLInterface::getMode()
{
	return mpane->getMode();
}

QString ZDLInterface::getHostAddy()
{
	return mpane->getHostAddy();
}

QString ZDLInterface::getPlayers()
{
	return mpane->getPlayers();
}

 QString ZDLInterface::getFragLmit()
 {
	 return mpane->getFragLmit();
 }

 QString ZDLInterface::getDMFlags()
 {
	 return mpane->getDMFlags();
 }

 QString ZDLInterface::getDMFlags2()
 {
	 return mpane->getDMFlags2();
 }
