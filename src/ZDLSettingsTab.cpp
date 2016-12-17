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

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include "ZDLConfigurationManager.h"
#include "ZDLListWidget.h"
#include "ZDLSettingsTab.h"

#if defined(ASSOCIATE_FILETYPES_AVAILBLE)
#include "ZDLFileAssociations.h"
#endif

ZDLSettingsTab::ZDLSettingsTab(QWidget *parent): ZDLWidget(parent){
	LOGDATAO() << "New ZDLSettingsTab" << endl;
	QVBoxLayout *sections = new QVBoxLayout(this);
	
	QVBoxLayout *iwadl = new QVBoxLayout();
	QVBoxLayout *spl = new QVBoxLayout();
	
	alwaysArgs = new QLineEdit(this);
	
	QHBoxLayout *lrpane = new QHBoxLayout();
	
	//IWAD
	QVBoxLayout *lpane = new QVBoxLayout();
	iwadList = new ZDLIWadList(this);
	iwadl->addWidget(new QLabel("IWADs:", this));
	iwadl->addWidget(iwadList);
	
	//Source Port
	QVBoxLayout *rpane = new QVBoxLayout();
	sourceList = new ZDLSourcePortList(this);
	spl->addWidget(new QLabel("Source Ports/Engines:", this));
	spl->addWidget(sourceList);
	
	rpane->addLayout(iwadl);
	lpane->addLayout(spl);
	
	//Add the left and right panes
	lrpane->addLayout(lpane);
	lrpane->addLayout(rpane);
	
	//Add all the sections together
	sections->addWidget(new QLabel("Always Add These Parameters", this));
	
	launchClose = new QCheckBox("Close on launch",this);
	launchClose->setToolTip("Close "ZDL_ENGINE_NAME" completely when launching a new game");

	showPaths = new QCheckBox("Show files paths in lists",this);
	showPaths->setToolTip("Show the directory path in square brackets in list widgets");
	connect(showPaths,SIGNAL(toggled(bool)),this,SLOT(pathToggled(bool)));
	sections->addWidget(alwaysArgs);

	QHBoxLayout *fileassoc = new QHBoxLayout();
	launchZDL = new QCheckBox("Launch *.ZDL files transparently", this);
	launchZDL->setToolTip("If a .ZDL file is specified on the command line path, launch the configuration without showing the interface");
	fileassoc->addWidget(launchZDL);
	
#if defined(ASSOCIATE_FILETYPES_AVAILBLE)
	QPushButton *assoc = new QPushButton("Associations", this);
	assoc->setToolTip("Associate various file types with "ZDL_ENGINE_NAME);
	fileassoc->addWidget(assoc);
	connect(assoc, SIGNAL(clicked()), this, SLOT(fileAssociations()));
#endif
	
	savePaths = new QCheckBox("Save/Load PWAD list automatically", this);
	savePaths->setToolTip("Save the zdl.save section (PWADS) when closing");

	sections->addLayout(fileassoc);
	sections->addLayout(lrpane);
	sections->addWidget(launchClose);
	sections->addWidget(showPaths);
	sections->addWidget(savePaths);
	
#if !defined(NO_UPDATER)	
	QHBoxLayout *hbox = new QHBoxLayout();
	updater = new QCheckBox("Enable Update Notifier", this);
	updater->setToolTip("Check for updates at launch at most once a day");
	QPushButton *btnCheckNow = new QPushButton("Check now",this);
	connect(btnCheckNow, SIGNAL( clicked() ), this, SLOT(checkNow()));
	
	hbox->addWidget(updater);
	hbox->addWidget(btnCheckNow);
	
	sections->addLayout(hbox);

#if defined(ZDL_STABLE)
	sections->addWidget(new QLabel("You're running ZDL Stable", this));
	QLabel *web = new QLabel("Visit <a href=http://zdl.vectec.net>zdl.vectec.net</a> to try a beta</a>");
	web->setOpenExternalLinks(true);
	sections->addWidget(web);
	
#endif

#endif
	
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
}

void ZDLSettingsTab::pathToggled(bool enabled){
	if(enabled){
		LOGDATAO() << "Path bracketing enabled" << endl;
	}else{
		LOGDATAO() << "Path bracketing disabled" << endl;
	}
	iwadList->rebuild();
	sourceList->rebuild();
}

void ZDLSettingsTab::fileAssociations(){
#if defined(ASSOCIATE_FILETYPES_AVAILBLE)
	ZDLFileAssociations assoc(this);
	assoc.exec();
#endif
}

void ZDLSettingsTab::rebuild(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
#if !defined(NO_UPDATER)
	ZDLSection *section = zconf->getSection("zdl.net");
	//Delete old configs
	if (section){
		QVector<ZDLLine*> vctr;
		section->getRegex("^updateManager$", vctr);
		for(int i = 0; i < vctr.size(); i++){
			section->deleteVariable(vctr[i]->getVariable());
		}
	}
	
	if (updater->checkState() == Qt::Unchecked){
		zconf->setValue("zdl.net", "updateManager", "disabled");
	}
#endif
	
	if(launchClose->checkState() == Qt::Checked){
		zconf->setValue("zdl.general","autoclose", "1");
	}else{
		zconf->setValue("zdl.general","autoclose", "0");
	}
	if(launchZDL->checkState() == Qt::Checked){
		zconf->setValue("zdl.general","zdllaunch", "1");
	}else{
		zconf->setValue("zdl.general","zdllaunch", "0");
	}
	if(alwaysArgs->text().isEmpty()){
		zconf->deleteValue("zdl.general", "alwaysadd");
	}else{
		zconf->setValue("zdl.general", "alwaysadd", alwaysArgs->text());
	}
	if(showPaths->checkState() == Qt::Checked){
		zconf->setValue("zdl.general", "showpaths", "1");
	}else{
		zconf->setValue("zdl.general", "showpaths", "0");
	}
	if(savePaths->checkState() == Qt::Checked){
		zconf->setValue("zdl.general", "rememberFilelist", "1");
	}else{
		zconf->setValue("zdl.general", "rememberFilelist", "0");
	}
}

void ZDLSettingsTab::newConfig(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
#if !defined(NO_UPDATER)
	ZDLSection *section = zconf->getSection("zdl.net");
	if (section){
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^updateManager$", fileVctr);
		
		for(int i = 0; i < fileVctr.size(); i++){
			if (fileVctr[i]->getValue().compare("disabled") == 0){
				updater->setCheckState(Qt::Unchecked);
			}else{
				updater->setCheckState(Qt::Checked);
			}
		}
		//Make sure that it's on by default, if it's not listed
		if (fileVctr.size() < 1){
			updater->setCheckState(Qt::Checked);
		}
	}else{
		//Default to on if it's not listed
		updater->setCheckState(Qt::Checked);
	}
#endif

	if(zconf->hasValue("zdl.general","showpaths")){
		int ok = 0;
		QString setting = zconf->getValue("zdl.general","showpaths", &ok);
		if(!setting.isNull()){
			if(setting == "0"){
				showPaths->setCheckState(Qt::Unchecked);
			}else{
				showPaths->setCheckState(Qt::Checked);
			}
		}else{
			showPaths->setCheckState(Qt::Checked);
		}
	}else{
		showPaths->setCheckState(Qt::Checked);
	}

	if(zconf->hasValue("zdl.general","alwaysadd")){
		int ok;
		QString rc = zconf->getValue("zdl.general","alwaysadd", &ok);
		if(!rc.isNull()){
			alwaysArgs->setText(rc);
			LOGDATAO() << "Set alwaysadd as " << rc << endl;
		}else{
			LOGDATAO() << "alwaysadd was null" << endl;
		}
	}else{
		LOGDATAO() << "No alwaysadd" << endl;
	}
	
	if(zconf->hasValue("zdl.general","autoclose")){
		int ok;
		QString closeSetting = zconf->getValue("zdl.general","autoclose",&ok);
		if(closeSetting == "1"){
			launchClose->setCheckState(Qt::Checked);
		}else{
			launchClose->setCheckState(Qt::Unchecked);
		}
	}else{
		launchClose->setCheckState(Qt::Unchecked);
	}

	if(zconf->hasValue("zdl.general","zdllaunch")){
		int ok;
		QString closeSetting = zconf->getValue("zdl.general","zdllaunch",&ok);
		if(closeSetting == "1"){
			launchZDL->setCheckState(Qt::Checked);
		}else{
			launchZDL->setCheckState(Qt::Unchecked);
		}
	}else{
		launchZDL->setCheckState(Qt::Unchecked);
	}
	bool rememberFilelist = true;
	if(zconf->hasValue("zdl.general", "rememberFilelist")){
		int ok;
		QString val = zconf->getValue("zdl.general", "rememberFilelist", &ok);
		if(val == "0"){
			rememberFilelist = false;
		}
	}
	if (rememberFilelist){
		savePaths->setCheckState(Qt::Checked);
	}else{
		savePaths->setCheckState(Qt::Unchecked);
	}
	
}

void ZDLSettingsTab::reloadConfig(){
	LOGDATAO() << "Reloading config" << endl;
	writeConfig();
	startRead();
	LOGDATAO() << "Reload complete" << endl;
}

void ZDLSettingsTab::checkNow(){
	LOGDATAO() << "checking for updates" << endl;
	ZDLUpdater *zup = ZDLConfigurationManager::getUpdater();
	if(zup){
		
		zup->fetch(1);
	}
}

void ZDLSettingsTab::startRead(){
	LOGDATAO() << "Reading new configuration" << endl;
	emit readChildren(this);
	newConfig();
}

void ZDLSettingsTab::writeConfig(){
	LOGDATAO() << "Writing configuration" << endl;
	emit buildChildren(this);
	rebuild();
}
