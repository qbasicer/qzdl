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

#include "ZDLIWadList.h"
#include "ZDLNameListable.h"
#include "zdlconf.h"
#include "ZDLNameInput.h"


ZDLIWadList::ZDLIWadList(ZDLWidget *parent): ZDLListWidget(parent){
}

void ZDLIWadList::newConfig(){
	pList->clear();
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	for (int i = 0; ; i++)
	{
		auto nameKey = QString("zdl.iwads/i%1n").arg(i);
		auto fileKey = QString("zdl.iwads/i%1f").arg(i);
		if (!zconf->contains(nameKey) || !zconf->contains(fileKey))
		{
			break;
		}

		auto fileName = zconf->value(fileKey).toString();
		auto disName = zconf->value(nameKey).toString();
		ZDLNameListable *zList = new ZDLNameListable(pList, 1001, fileName, disName);
		insert(zList, -1);
	}
}


void ZDLIWadList::rebuild(){
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	zconf->beginGroup("zdl.iwads");
	zconf->remove("");


	for(int i = 0; i < count(); i++){
		QListWidgetItem *itm = pList->item(i);
		ZDLNameListable* fitm = (ZDLNameListable*)itm;

		zconf->setValue(QString("i%1n").arg(i), fitm->getName());
		zconf->setValue(QString("i%1f").arg(i), fitm->getFile());
	}

	zconf->endGroup();
}

void ZDLIWadList::newDrop(QStringList fileList){
	for(int i = 0; i < fileList.size(); i++){
		ZDLNameListable *zList = NULL;
		QString entry = fileList[i];
		QStringList pathParts = entry.split("/");
		if(pathParts.size() > 1){
			QString file = pathParts.last();
			QStringList fileParts = file.split(".");
			if(fileParts.size() > 1){
				QString name = fileParts[0];
				zList = new ZDLNameListable(pList, 1001, entry, name);
			}else{
				zList = new ZDLNameListable(pList, 1001, entry, file);
			}
		}else{
			zList = new ZDLNameListable(pList, 1001, entry, entry);
		}
		if(zList){
			insert(zList, -1);
		}
	}
}

void ZDLIWadList::addButton(){
	QStringList filters;
	filters << "WAD/PK3/ZIP (*.WAD *.pk3 *.zip)"
         << "WAD Files (*.WAD)"
         << "pk3 Files (*.pk3)"
         << "zip Files (*.zip)"
         << "All files (*)";

	ZDLNameInput diag(this);
	diag.setWindowTitle("Add IWAD");
	diag.setFilter(filters);
	if (diag.exec()){
		QString fileName = diag.getFile();
		QString name = diag.getName();
		ZDLNameListable *zList = new ZDLNameListable(pList, 1001, fileName, name);
		insert(zList, -1);
	}

}

void ZDLIWadList::editButton(QListWidgetItem * item){
	if (item){
		QStringList filters;
		filters << "WAD/PK3/ZIP (*.wad *.pk3 *.zip)"
				<< "WAD Files (*.wad)"
				<< "pk3 Files (*.pk3)"
				<< "zip Files (*.zip)"
				<< "All files (*)";
		ZDLNameListable *zitem = (ZDLNameListable*)item;
		ZDLNameInput diag(this);
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

