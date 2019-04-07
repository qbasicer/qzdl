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
 
#include <QtGui>
#include <QApplication>

#include "ZDLConfigurationManager.h"
#include "ZDLListWidget.h"
#include "ZDLSettingsTab.h"
#include "ZDLQSplitter.h"

#if defined(Q_WS_WIN)&&!defined(_ZDL_NO_WFA)
#include <windows.h>
#include "ZDLFileAssociations.h"
#endif

ZDLSettingsTab::ZDLSettingsTab(QWidget *parent): ZDLWidget(parent){
	LOGDATAO() << "New ZDLSettingsTab" << endl;
	QVBoxLayout *sections = new QVBoxLayout(this);
	
	alwaysArgs = new QLineEdit(this);
	
	ZDLQSplitter *split = new ZDLQSplitter(this);
	split->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ));
	QSplitter *rsplit = split->getSplit();
	
	//IWAD
	QWidget *rwidget = new QWidget(rsplit);
	QVBoxLayout *rpane = new QVBoxLayout();
	iwadList = new ZDLIWadList(this);
	iwadList->doDragDrop(true);
	rpane->addWidget(new QLabel("IWADs", this));
	rpane->addWidget(iwadList);
	rwidget->setLayout(rpane);
	rpane->setContentsMargins(0,0,0,0);
	
	//Source Port
	QWidget *lwidget = new QWidget(rsplit);
	QVBoxLayout *lpane = new QVBoxLayout();
	sourceList = new ZDLSourcePortList(this);
	sourceList->doDragDrop(true);
	lpane->addWidget(new QLabel("Source ports", this));
	lpane->addWidget(sourceList);
	lwidget->setLayout(lpane);
	lpane->setContentsMargins(0,0,0,0);
	
	split->addChild(lwidget);
	split->addChild(rwidget);
	
	//Add all the sections together
	sections->addWidget(new QLabel("Always add these parameters", this));
	
	launchClose = new QCheckBox("Close on launch",this);
	launchClose->setToolTip("Close ZDL completely when launching a new game");

    showPaths = new QCheckBox("Show file paths in lists",this);
	showPaths->setToolTip("Show the directory path in square brackets in list widgets");
	connect(showPaths,SIGNAL(stateChanged(int)),this,SLOT(pathToggled(int)));
	sections->addWidget(alwaysArgs);

	QHBoxLayout *fileassoc = new QHBoxLayout();
	launchZDL = new QCheckBox("Launch *.ZDL files transparently", this);
	launchZDL->setToolTip("If a .ZDL file is specified on the command line path, launch the configuration without showing the interface");
	fileassoc->addWidget(launchZDL);
	
#if defined(Q_WS_WIN)&&!defined(_ZDL_NO_WFA)
	QPushButton *assoc = new QPushButton("Associations", this);
	assoc->setToolTip("Associate various file types with ZDL");
	fileassoc->addWidget(assoc);
	connect(assoc, SIGNAL(clicked()), this, SLOT(fileAssociations()));
#endif
	
    savePaths = new QCheckBox("Remember external file list", this);
    savePaths->setToolTip("Save external file list on exit and load it on next program launch");

	sections->addLayout(fileassoc);
	sections->addWidget(split);
	sections->addWidget(launchClose);
	sections->addWidget(showPaths);
	sections->addWidget(savePaths);
	setContentsMargins(4,4,4,4);
	layout()->setContentsMargins(0,0,0,0);
}

void ZDLSettingsTab::pathToggled(int state)
{
	Q_UNUSED(state);

	ZDLConf *zconf=ZDLConfigurationManager::getActiveConfiguration();
	if(showPaths->checkState()==Qt::Checked)
		zconf->setValue("zdl.general", "showpaths", "1");
	else
		zconf->setValue("zdl.general", "showpaths", "0");
	iwadList->newConfig();
	sourceList->newConfig();
}

void ZDLSettingsTab::fileAssociations()
{
#if defined(Q_WS_WIN)&&!defined(_ZDL_NO_WFA)
	ZDLFileAssociations assoc(this);
	assoc.exec();
#endif
}

void ZDLSettingsTab::rebuild(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	
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
