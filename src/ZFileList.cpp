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
 
#include "ZFileList.h"
#include "ZFileListable.h"
#include "ZDLConfigurationManager.h"

#include <iostream>
using namespace std;

ZFileList::ZFileList(ZQWidget *parent): ZDLListWidget(parent){
}

void ZFileList::newDrop(QList<QUrl> urlList){
	
	for (int i = 0; i < urlList.size() && i < 32; ++i) {
		QUrl url = urlList.at(i);
		if(url.scheme() == "file"){
			ZFileListable *zList = new ZFileListable(pList, 1001, url.path());
			insert(zList, -1);
		}
	}
}

void ZFileList::newConfig(){
	pList->clear();
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.save");
	if (section){
		vector <ZDLLine*> vctr;
		section->getRegex("^file[0-9]+$", vctr);
		//cout << "I got " << vctr.size() << " matches!" << endl;
		for(unsigned int i = 0; i < vctr.size(); i++){
			ZFileListable *zList = new ZFileListable(pList, 1001, vctr[i]->getValue());
			insert(zList, -1);
		}
	}
}

void ZFileList::rebuild(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.save");
	if (section){
		vector <ZDLLine*> vctr;
		section->getRegex("^file[0-9]+$", vctr);
		//cout << "I got " << vctr.size() << " matches!" << endl;
		for(unsigned int i = 0; i < vctr.size(); i++){
			section->deleteVariable(vctr[i]->getVariable());
		}
	}
	
	//cout << "Building lines" << endl;
	for(int i = 0; i < count(); i++){
		QListWidgetItem *itm = pList->item(i);
		ZFileListable* fitm = (ZFileListable*)itm;
		char szBuffer[256];
		snprintf(szBuffer, 256, "file%d", i);
		zconf->setValue("zdl.save", szBuffer, fitm->getFile());
	
	}
	
}

void ZFileList::addButton(){
	QStringList filters;
	filters << "WAD/PK3/ZIP (*.wad *.pk3 *.zip)"
         << "WAD Files (*.wad)"
         << "pk3 Files (*.pk3)"
         << "zip Files (*.zip)"
         << "Any files (*)";
	QFileDialog dialog(this);
	dialog.setFilters(filters);
	dialog.setFileMode(QFileDialog::ExistingFiles);
	QStringList fileNames;
	if(dialog.exec()){
		fileNames = dialog.selectedFiles();
		for(int i = 0; i < fileNames.size(); i++){
			ZFileListable *zList = new ZFileListable(pList, 1001, fileNames[i]);
			insert(zList, -1);
		}

	}


	


}

