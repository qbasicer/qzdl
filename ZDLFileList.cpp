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

#include "ZDLFileList.h"
#include "ZDLFileListable.h"
#include "zdlconf.h"
#include "ZDLMapFile.h"

extern QString getLastDir();
extern void saveLastDir(QString fileName);

ZDLFileList::ZDLFileList(ZDLWidget *parent): ZDLListWidget(parent){
}

void ZDLFileList::newDrop(QStringList fileList){
	for (int i = 0; i < fileList.size(); i++) {
		ZDLFileListable *zList = new ZDLFileListable(pList, 1001, fileList[i]);
		insert(zList, -1);
	}
}

void ZDLFileList::newConfig(){
	pList->clear();
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	for (int i = 0; ; i++)
	{
		QString key{"zdl.save/file" + QString::number(i)};
		if (!zconf->contains(key))
		{
			break;
		}
		ZDLFileListable *zList = new ZDLFileListable(pList, 1001, zconf->value(key).toString());
		insert(zList, -1);
	}
}

void ZDLFileList::rebuild(){

	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	for (int i = 0; ; i++)
	{
		QString key{"zdl.save/file" + QString::number(i)};
		if (!zconf->contains(key))
		{
			break;
		}
		zconf->remove(key);
	}

	for(int i = 0; i < count(); i++){
		QListWidgetItem *itm = pList->item(i);
		ZDLFileListable* fitm = (ZDLFileListable*)itm;
		QString name = QString("file").append(QString::number(i));
		zconf->setValue("zdl.save/" + name, fitm->getFile());
	}
}

void ZDLFileList::addButton(){
	QStringList filters;
	filters << "WAD/PK3/ZIP/PK7/PKZ/P7Z (*.wad *.pk3 *.zip *.pk7 *.pkz *.p7z *.ipk3)"
		<< "WAD Files (*.wad)"
		<< "PK3 Files (*.pk3)"
		<< "IPK3 Files (*.ipk3)"
		<< "Patch Files (*.bex *.deh)"
		<< "PK7 Files (*.pk7)"
		<< "PKZ Files (*.pkz)"
		<< "P7Z Files (*.p7z)"
		<< "zip Files (*.zip)"
		<< "Any files (*)";
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "Add File", getLastDir(), filters.join(";;"));
	for(int i = 0; i < fileNames.size(); i++){
		saveLastDir(fileNames[i]);
		ZDLFileListable *zList = new ZDLFileListable(pList, 1001, fileNames[i]);
		insert(zList, -1);
	}
}

