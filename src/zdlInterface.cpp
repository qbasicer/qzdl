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
 
#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include "configurationManager.h"

#include "multiPane.h"
#include "zdlInterface.h"
#include "bottomPane.h"
#include "topPane.h"
#include "ZInfoBar.h"

zdlInterface::zdlInterface(QWidget *parent):ZQWidget(parent){
	configurationManager::setInterface(this);
	
	box = new QVBoxLayout(this);
	topPane *tpane = new topPane(this);
	bottomPane *bpane = new bottomPane(this);
	bpane->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Maximum ));
	mpane = new multiPane(this);
	mpane->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Maximum ));
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(0);
	ZInfoBar *zib = new ZInfoBar(this);
	box->addWidget(zib);
	
	configurationManager::setInfobar(zib);
	box->addWidget(tpane);
	box->addWidget(bpane);
	
}

//Called when there's a change to the configuration that we need to look at.
//The button changed the configuration, and then notifies us that we need
//to look at the configuration to see what we need to do.
void zdlInterface::newConfig(){
	//Grab our configuration
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	//Grab our section in the configuration
	ZDLSection *section = zconf->getSection("zdl.save");
	//Do we have it?
	if (section){
		vector <ZDLLine*> vctr;
		//Search for our string
		section->getRegex("^dlgmode$", vctr);
		for(unsigned int i = 0; i < vctr.size(); i++){
			//Are we open?
			if (strcmp(vctr[i]->getValue(), "open") == 0){
				//cout << "Opening config" << endl;
				box->addWidget(mpane);
				mpane->setVisible(true);
			//No?
			}else{
				//cout << "Removing config" << endl;
				box->removeWidget(mpane);
				mpane->setVisible(false);
				mpane->hide();
			}
		}
		//Do we have the section, but not the key?
		if (vctr.size() < 1){
			//cout << "Removing config (no line present)" << endl;
			box->removeWidget(mpane);
			mpane->setVisible(false);
		}
	//Do we not even have the section?
	}else{
		//cout << "Removing config (no section present)" << endl;
		box->removeWidget(mpane);
	}
	this->update();
}



void zdlInterface::startRead(){
	emit readChildren(this);
	newConfig();
}

void zdlInterface::writeConfig(){
	emit buildChildren(this);
}
