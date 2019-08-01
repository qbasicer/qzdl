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

#include "zdlconf.h"
#include "ZDLListWidget.h"
#include "ZDLSettingsTab.h"

ZDLSettingsTab::ZDLSettingsTab(QWidget *parent): ZDLWidget(parent){
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
	launchClose->setToolTip("Close ZDL completely when launching a new game");

	showPaths = new QCheckBox("Show files paths in lists",this);
	showPaths->setToolTip("Show the directory path in square brackets in list widgets");
	connect(showPaths, &QCheckBox::toggled, this, &ZDLSettingsTab::pathToggled);
	sections->addWidget(alwaysArgs);

	QHBoxLayout *fileassoc = new QHBoxLayout();
	launchZDL = new QCheckBox("Launch *.ZDL files transparently", this);
	launchZDL->setToolTip("If a .ZDL file is specified on the command line path, launch the configuration without showing the interface");
	fileassoc->addWidget(launchZDL);

	savePaths = new QCheckBox("Save/Load PWAD list automatically", this);
	savePaths->setToolTip("Save the zdl.save section (PWADS) when closing");

	sections->addLayout(fileassoc);
	sections->addLayout(lrpane);
	sections->addWidget(launchClose);
	sections->addWidget(showPaths);
	sections->addWidget(savePaths);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
}

void ZDLSettingsTab::pathToggled(bool enabled){
	// "enabled" is path bracketing.
	iwadList->rebuild();
	sourceList->rebuild();
}

void ZDLSettingsTab::rebuild(){
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();

	if(launchClose->checkState() == Qt::Checked){
		zconf->setValue("zdl.general/autoclose", "1");
	}else{
		zconf->setValue("zdl.general/autoclose", "0");
	}
	if(launchZDL->checkState() == Qt::Checked){
		zconf->setValue("zdl.general/zdllaunch", "1");
	}else{
		zconf->setValue("zdl.general/zdllaunch", "0");
	}
	if(alwaysArgs->text().isEmpty()){
		zconf->remove("zdl.general/alwaysadd");
	}else{
		zconf->setValue("zdl.general/alwaysadd", alwaysArgs->text());
	}
	if(showPaths->checkState() == Qt::Checked){
		zconf->setValue("zdl.general/showpaths", "1");
	}else{
		zconf->setValue("zdl.general/showpaths", "0");
	}
	if(savePaths->checkState() == Qt::Checked){
		zconf->setValue("zdl.general/rememberFilelist", "1");
	}else{
		zconf->setValue("zdl.general/rememberFilelist", "0");
	}
}

void ZDLSettingsTab::newConfig(){
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();

	if(zconf->contains("zdl.general/showpaths")){
		QString setting = zconf->value("zdl.general/showpaths").toString();
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

	if(zconf->contains("zdl.general/alwaysadd")){
		QString rc = zconf->value("zdl.general/alwaysadd").toString();
		if(!rc.isNull()){
			alwaysArgs->setText(rc);
		}
	}

	if(zconf->contains("zdl.general/autoclose")){
		QString closeSetting = zconf->value("zdl.general","autoclose").toString();
		if(closeSetting == "1"){
			launchClose->setCheckState(Qt::Checked);
		}else{
			launchClose->setCheckState(Qt::Unchecked);
		}
	}else{
		launchClose->setCheckState(Qt::Unchecked);
	}

	if(zconf->contains("zdl.general/zdllaunch")){
		QString closeSetting = zconf->value("zdl.general/zdllaunch").toString();
		if(closeSetting == "1"){
			launchZDL->setCheckState(Qt::Checked);
		}else{
			launchZDL->setCheckState(Qt::Unchecked);
		}
	}else{
		launchZDL->setCheckState(Qt::Unchecked);
	}
	bool rememberFilelist = true;
	if(zconf->contains("zdl.general/rememberFilelist")){
		QString val = zconf->value("zdl.general/rememberFilelist").toString();
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
	// Reloading config
	writeConfig();
	startRead();
	// Reload complete
}

void ZDLSettingsTab::startRead(){
	// Reading new configuration
	emit readChildren(this);
	newConfig();
}

void ZDLSettingsTab::writeConfig(){
	// Writing configuration
	emit buildChildren(this);
	rebuild();
}

QString ZDLSettingsTab::getAlwaysArgs()
{
	return alwaysArgs->text();
}

Qt::CheckState ZDLSettingsTab::getLaunchClose()
{
	return launchClose->checkState();
}

Qt::CheckState ZDLSettingsTab::getShowPaths()
{
	return showPaths->checkState();
}
Qt::CheckState ZDLSettingsTab::getLaunchZDL()
{
	return launchZDL->checkState();
}
Qt::CheckState ZDLSettingsTab::getSavePaths()
{
	return savePaths->checkState();
}
