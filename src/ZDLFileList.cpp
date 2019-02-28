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

#include "ZDLFileList.h"
#include "ZDLFileListable.h"
#include "ZDLConfigurationManager.h"
#include "gph_fld.xpm"

#include <iostream>
using namespace std;

ZDLFileList::ZDLFileList(ZDLWidget *parent): ZDLListWidget(parent){
	LOGDATAO() << "ZDLFileList" << endl;

	QPushButton *btnFolder = new QPushButton(this);
	btnFolder->setIcon(QPixmap(glyph_folder));
	btnFolder->setToolTip("Add directory");
	buttonRow->insertWidget(0, btnFolder);

	btnAdd->setToolTip("Add files");
	btnRem->setToolTip("Remove selected files and directories");
	btnEdt->setToolTip("Exclude/include selected files and directories");
	btnUp->setToolTip("Move selected files and directories up");
	btnDn->setToolTip("Move selected files and directories down");

	QObject::connect(btnFolder, SIGNAL(clicked()), this, SLOT(folderButton()));
}

void ZDLFileList::newDrop(QStringList fileList){
	LOGDATAO() << "newDrop" << endl;	
	for (int i=0; i<fileList.size(); i++)
		insert(new ZDLFileListable(pList, 1001, fileList[i]), -1);
}

void ZDLFileList::newConfig(){
	LOGDATAO() << "Reading new config" << endl;
	pList->clear();
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.save");
	if (section){
		QVector <ZDLLine*> vctr;
		section->getRegex("^file[0-9]+d?$", vctr);
		//cout << "I got " << vctr.size() << " matches!" << endl;
		for(int i = 0; i < vctr.size(); i++){
			ZDLFileListable *zList = new ZDLFileListable(pList, 1001, vctr[i]->getValue());
			if (vctr[i]->getVariable().endsWith("d", Qt::CaseInsensitive)) {
				QFont item_font=zList->font();
				item_font.setStrikeOut(true);
				zList->setFont(item_font);
			}
			insert(zList, -1);
		}
	}
}

void ZDLFileList::rebuild(){
	LOGDATAO() << "Saving config" << endl;
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.save");
	if (section){
		QVector <ZDLLine*> vctr;
		section->getRegex("^file[0-9]+d?$", vctr);
		//cout << "I got " << vctr.size() << " matches!" << endl;
		for(int i = 0; i < vctr.size(); i++){
			// Can't use the section to perform this operation
			// Section is a clone of the real section
			zconf->deleteValue("zdl.save", vctr[i]->getVariable());
		}
	}

	//cout << "Building lines" << endl;
	for(int i = 0; i < count(); i++){
		QListWidgetItem *itm = pList->item(i);
		ZDLFileListable* fitm = (ZDLFileListable*)itm;
		QString name=QString("file%1").arg(i);
		if (fitm->font().strikeOut()) name.append("d");
		zconf->setValue("zdl.save", name, fitm->getFile());
	}

}

void ZDLFileList::addButton()
{
	LOGDATAO() << "Adding new file" << endl;
	QStringList filters;
	filters << "Doom resource files (*.wad;*.iwad;*.zip;*.pk3;*.ipk3;*.7z;*.pk7;*.p7z;*.pkz;*.bex;*.deh;*.cfg)"
		<< "WAD files (*.wad;*.iwad)"
		<< "Patch files (*.bex;*.deh)"
		<< "Config files (*.cfg)"
		<< "All supported archives (*.zip;*.pk3;*.ipk3;*.7z;*.pk7;*.p7z;*.pkz)"
		<< "Specialized archives (*.pk3;*.ipk3;*.pk7;*.p7z;*.pkz)"
		<< "All files (*.*)";

	QStringList fileNames = QFileDialog::getOpenFileNames(this, "Add files", getWadLastDir(), filters.join(";;"));
	for(int i = 0; i < fileNames.size(); i++){
		LOGDATAO() << "Adding file " << fileNames[i] << endl;
		saveWadLastDir(fileNames[i]);
		ZDLFileListable *zList = new ZDLFileListable(pList, 1001, fileNames[i]);
		insert(zList, -1);
	}
}

void ZDLFileList::editButton(QListWidgetItem * item)
{
	if (item) {
		QFont item_font=item->font();
		if (item_font.strikeOut())
			item_font.setStrikeOut(false);
		else
			item_font.setStrikeOut(true);
		item->setFont(item_font);
	}
}

void ZDLFileList::editButton()
{
    foreach (QListWidgetItem *item, pList->selectedItems()) 
		editButton(item);
}

void ZDLFileList::folderButton()
{
	LOGDATAO() << "Adding new dir" << endl;

	QString dirName = QFileDialog::getExistingDirectory(this, "Add directory", getWadLastDir(), QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
	if (!dirName.isEmpty()) {
		LOGDATAO() << "Adding dir " << dirName << endl;
		saveWadLastDir(dirName, NULL, true);
		ZDLFileListable *zList = new ZDLFileListable(pList, 1001, dirName);
		insert(zList, -1);
	}
}
