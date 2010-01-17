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
 
#include "ZIWadList.h"
#include "ZNameListable.h"
#include "configurationManager.h"
#include "ZNameInput.h"

#include <iostream>
using namespace std;

ZIWadList::ZIWadList(ZQWidget *parent): zListWidget(parent){
}

void ZIWadList::newConfig(){
	pList->clear();
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.iwads");
	if (section){
		vector <ZDLLine*> fileVctr;
		section->getRegex("^i[0-9]+f$", fileVctr);
		
		for(unsigned int i = 0; i < fileVctr.size(); i++){
			string value = fileVctr[i]->getVariable();
			
			string number = "^i";
			number.append(value.substr(1, value.length()-2));
			number.append("n$");
			
			vector <ZDLLine*> nameVctr;
			section->getRegex(number.c_str(), nameVctr);
			if (nameVctr.size() == 1){
				QString disName = nameVctr[0]->getValue();
				QString fileName = fileVctr[i]->getValue();
				ZNameListable *zList = new ZNameListable(pList, 1001, fileName, disName);
				insert(zList, -1);
			}
		}
	}
}


void ZIWadList::rebuild(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.iwads");
	if (section){
		vector <ZDLLine*> vctr;
		section->getRegex("^i[0-9]f$", vctr);
		for(unsigned int i = 0; i < vctr.size(); i++){
			section->deleteVariable(vctr[i]->getVariable());
		}
		vctr.clear();
		section->getRegex("^i[0-9]n$", vctr);
		for(unsigned int i = 0; i < vctr.size(); i++){
			section->deleteVariable(vctr[i]->getVariable());
		}
	}
	
	for(int i = 0; i < count(); i++){
		QListWidgetItem *itm = pList->item(i);
		ZNameListable* fitm = (ZNameListable*)itm;
		char szBuffer[256];
		snprintf(szBuffer, 256, "i%dn", i);
		zconf->setValue("zdl.iwads", szBuffer, fitm->getName().toStdString().c_str());
		snprintf(szBuffer, 256, "i%df", i);
		zconf->setValue("zdl.iwads", szBuffer, fitm->getFile().toStdString().c_str());
		
	}
	
}

void ZIWadList::addButton(){
	QStringList filters;
	filters << "WAD/PK3/ZIP (*.wad *.pk3 *.zip)"
         << "WAD Files (*.wad)"
         << "pk3 Files (*.pk3)"
         << "zip Files (*.zip)"
         << "All files (*)";
	
	ZNameInput diag(this);
	diag.setWindowTitle("Add IWAD");
	diag.setFilter(filters);
	if (diag.exec()){
		QString fileName = diag.getFile();
		QString name = diag.getName();
		ZNameListable *zList = new ZNameListable(pList, 1001, fileName, name);
		insert(zList, -1);
	}

}

void ZIWadList::editButton(QListWidgetItem * item){
	if (item){
		QStringList filters;
		filters << "WAD/PK3/ZIP (*.wad *.pk3 *.zip)"
				<< "WAD Files (*.wad)"
				<< "pk3 Files (*.pk3)"
				<< "zip Files (*.zip)"
				<< "All files (*)";
		ZNameListable *zitem = (ZNameListable*)item;
		ZNameInput diag(this);
		diag.setWindowTitle("Add IWAD");
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

