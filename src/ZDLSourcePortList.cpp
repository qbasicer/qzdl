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
 
#include "ZDLSourcePortList.h"
#include "ZDLNameListable.h"
#include "ZDLConfigurationManager.h"
#include "ZDLNameInput.h"

#include <cstdio>
#include <iostream>
using namespace std;

ZDLSourcePortList::ZDLSourcePortList(ZQWidget *parent): zListWidget(parent){
}

void ZDLSourcePortList::newConfig(){
	pList->clear();
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		vector <ZDLLine*> fileVctr;
		section->getRegex("^p[0-9]+f$", fileVctr);
		
		for(unsigned int i = 0; i < fileVctr.size(); i++){
			string value = fileVctr[i]->getVariable();
			
			string number = "^p";
			number.append(value.substr(1, value.length()-2));
			number.append("n$");
			
			vector <ZDLLine*> nameVctr;
			section->getRegex(number.c_str(), nameVctr);
			if (nameVctr.size() == 1){
				QString disName = nameVctr[0]->getValue();
				QString fileName = fileVctr[i]->getValue();
				ZDLNameListable *zList = new ZDLNameListable(pList, 1001, fileName, disName);
				insert(zList, -1);
			}
		}
	}
}


void ZDLSourcePortList::rebuild(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		zconf->deleteSection("zdl.ports");
	}
	
	for(int i = 0; i < count(); i++){
		QListWidgetItem *itm = pList->item(i);
		ZDLNameListable* fitm = (ZDLNameListable*)itm;
		char szBuffer[256];
		snprintf(szBuffer, 256, "p%dn", i);
		zconf->setValue("zdl.ports", szBuffer, fitm->getName().toStdString().c_str());
		snprintf(szBuffer, 256, "p%df", i);
		zconf->setValue("zdl.ports", szBuffer, fitm->getFile().toStdString().c_str());
		
	
	}
	
}

void ZDLSourcePortList::addButton(){
	QStringList filters;
	filters << "Executable (*.exe *.bin)"
			<< "All files (*)";
	
	
	ZDLNameInput diag(this);
	diag.setWindowTitle("Add Source Port/Engine");
	diag.setFilter(filters);
	if (diag.exec()){
		QString fileName = diag.getFile();
		QString name = diag.getName();
		ZDLNameListable *zList = new ZDLNameListable(pList, 1001, fileName, name);
		insert(zList, -1);
	}
	
	
}

void ZDLSourcePortList::editButton(QListWidgetItem * item){
	if (item){
		QStringList filters;
		filters << "Executable (*.exe *.bin)"
				<< "All files (*)";
		ZDLNameListable *zitem = (ZDLNameListable*)item;
		ZDLNameInput diag(this);
		diag.setWindowTitle("Add Source Port/Engine");
		diag.setFilter(filters);
		diag.basedOff(zitem);
		if(diag.exec()){
			QString fileName = diag.getFile();
			QString name = diag.getName();
			zitem->setDisplayName(name);
			zitem->setFile(fileName);
		}	
	}
}