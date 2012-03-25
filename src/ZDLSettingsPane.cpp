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
#include <QtGui>
#include <QApplication>

#include "ZDLConfigurationManager.h"
#include "zListWidget.h"
#include "ZDLSettingsPane.h"

ZDLSettingsPane::ZDLSettingsPane(QWidget *parent): ZQWidget(parent){
	QVBoxLayout *sections = new QVBoxLayout(this);
	
	QVBoxLayout *iwadl = new QVBoxLayout();
	QVBoxLayout *spl = new QVBoxLayout();
	
	alwaysArgs = new QLineEdit(this);
	
	QHBoxLayout *lrpane = new QHBoxLayout();
	
	//IWAD
	QVBoxLayout *lpane = new QVBoxLayout();
	iwadList = new ZIWadList(this);
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
	pathQuote = new QCheckBox("Quote paths",this);
	sections->addWidget(alwaysArgs);
	sections->addLayout(lrpane);
	sections->addWidget(launchClose);
	sections->addWidget(pathQuote);
	
	QHBoxLayout *hbox = new QHBoxLayout();
	updater = new QCheckBox("Enable Update Notifier", this);
	QPushButton *btnCheckNow = new QPushButton("Check now",this);
	connect(btnCheckNow, SIGNAL( clicked() ), this, SLOT(checkNow()));
	
	hbox->addWidget(updater);
	hbox->addWidget(btnCheckNow);
	
	sections->addLayout(hbox);
	
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
}

void ZDLSettingsPane::rebuild(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.net");
	//Delete old configs
	if (section){
		vector <ZDLLine*> vctr;
		section->getRegex("^updateManager$", vctr);
		for(unsigned int i = 0; i < vctr.size(); i++){
			section->deleteVariable(vctr[i]->getVariable());
		}
	}
	
	if (updater->checkState() == Qt::Unchecked){
		zconf->setValue("zdl.net", "updateManager", "disabled");
	}
	
	if (pathQuote->checkState() == Qt::Unchecked){
		zconf->setValue("zdl.general", "quotefiles", "disabled");
	}else{
		zconf->setValue("zdl.general", "quotefiles", "enabled");
	}
	
	if(launchClose->checkState() == Qt::Checked){
		zconf->setValue("zdl.general","autoclose", "1");
	}else{
		zconf->setValue("zdl.general","autoclose", "0");
	}
}

void ZDLSettingsPane::newConfig(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.net");
	if (section){
		vector <ZDLLine*> fileVctr;
		section->getRegex("^updateManager$", fileVctr);
		
		for(unsigned int i = 0; i < fileVctr.size(); i++){
			if (strcmp(fileVctr[i]->getValue(), "disabled") == 0){
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
	
	
	if(zconf->hasValue("zdl.general","quotefiles")){
		int ok;
		string rc = zconf->getValue("zdl.general","quotefiles",&ok);
		if(rc == "enabled"){
			pathQuote->setCheckState(Qt::Checked);
		}else{
			pathQuote->setCheckState(Qt::Unchecked);
		}	
	}else{
		pathQuote->setCheckState(Qt::Unchecked);
	}
	
	if(zconf->hasValue("zdl.general","autoclose")){
		int ok;
		QString closeSetting = zconf->getValue("zdl.general","autoclose",&ok);
		if(closeSetting == "1"){
			launchClose->setCheckState(Qt::Checked);
		}else{
			launchClose->setCheckState(Qt::Unchecked);
		}
	}
	
}

void ZDLSettingsPane::checkNow(){
	ZDLUpdater *zup = ZDLConfigurationManager::getUpdater();
	zup->fetch(1);
}

void ZDLSettingsPane::startRead(){
	emit readChildren(this);
	newConfig();
}

void ZDLSettingsPane::writeConfig(){
	emit buildChildren(this);
	rebuild();
}
