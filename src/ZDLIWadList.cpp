/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * Copyright (C) 2018-2019  Lcferrum
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

#include "zdlcommon.h"
#include "ZDLIWadList.h"
#include "ZDLNameListable.h"
#include "ZDLConfigurationManager.h"
#include "ZDLNameInput.h"
#include "ZDLFileInfo.h"
#include "gph_ast.xpm"

#include <iostream>
using namespace std;

ZDLIWadList::ZDLIWadList(ZDLWidget *parent): ZDLListWidget(parent){
    QPushButton *btnWizardAdd = new QPushButton(this);
    btnWizardAdd->setIcon(QPixmap(glyph_asterisk));
    btnWizardAdd->setToolTip("Add and name item");
    buttonRow->insertWidget(0, btnWizardAdd);

    QObject::connect(btnWizardAdd, SIGNAL(clicked()), this, SLOT(wizardAddButton()));
}

void ZDLIWadList::wizardAddButton(){
    QString filters =
        "WAD files (*.wad" QFD_FILTER_DELIM "*.iwad);;"
        "All supported archives (*.zip" QFD_FILTER_DELIM "*.pk3" QFD_FILTER_DELIM "*.ipk3" QFD_FILTER_DELIM "*.7z" QFD_FILTER_DELIM "*.pk7" QFD_FILTER_DELIM "*.p7z" QFD_FILTER_DELIM "*.pkz);;"
        "Specialized archives (*.pk3" QFD_FILTER_DELIM "*.ipk3" QFD_FILTER_DELIM "*.pk7" QFD_FILTER_DELIM "*.p7z" QFD_FILTER_DELIM "*.pkz);;"
        "All files (" QFD_FILTER_ALL ")";

    ZDLIwadInfo zdl_fi;
    ZDLNameInput diag(this, getWadLastDir(NULL, true), &zdl_fi);
    diag.setWindowTitle("Add IWAD");
    diag.setFilter(filters);
    if (diag.exec()){
        saveWadLastDir(diag.getFile());
        insert(new ZDLNameListable(pList, 1001, diag.getFile(), diag.getName()), -1);
    }
}

void ZDLIWadList::newConfig(){
	pList->clear();
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.iwads");
	if (section){
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^i[0-9]+f$", fileVctr);
		
		for(int i = 0; i < fileVctr.size(); i++){
			QString value = fileVctr[i]->getVariable();
			
			QString number = "^i";
			number.append(value.mid(1, value.length()-2));
			number.append("n$");
			
			QVector<ZDLLine*> nameVctr;
			section->getRegex(number, nameVctr);
			if (nameVctr.size() == 1){
				QString disName = nameVctr[0]->getValue();
				QString fileName = fileVctr[i]->getValue();
				ZDLNameListable *zList = new ZDLNameListable(pList, 1001, fileName, disName);
				insert(zList, -1);
			}
		}
	}
}


void ZDLIWadList::rebuild(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.iwads");
	if (section){
		zconf->deleteSection("zdl.iwads");
	}
	
	for(int i = 0; i < count(); i++){
		QListWidgetItem *itm = pList->item(i);
		ZDLNameListable* fitm = (ZDLNameListable*)itm;
		
		zconf->setValue("zdl.iwads", QString("i").append(QString::number(i)).append("n"), fitm->getName());
		zconf->setValue("zdl.iwads", QString("i").append(QString::number(i)).append("f"), fitm->getFile());
	}
	
}

void ZDLIWadList::newDrop(QStringList fileList){
	LOGDATAO() << "newDrop" << endl;	
	for (int i=0; i<fileList.size(); i++)
		insert(new ZDLNameListable(pList, 1001, fileList[i], ZDLIwadInfo(fileList[i]).GetFileDescription()), -1);
}

void ZDLIWadList::addButton(){
    LOGDATAO() << "Adding new IWADs" << endl;
    QString filters =
        "IWAD files (*.wad" QFD_FILTER_DELIM "*.iwad" QFD_FILTER_DELIM "*.ipk3);;"
        "All supported archives (*.zip" QFD_FILTER_DELIM "*.pk3" QFD_FILTER_DELIM "*.ipk3" QFD_FILTER_DELIM "*.7z" QFD_FILTER_DELIM "*.pk7" QFD_FILTER_DELIM "*.p7z" QFD_FILTER_DELIM "*.pkz);;"
        "Specialized archives (*.pk3" QFD_FILTER_DELIM "*.ipk3" QFD_FILTER_DELIM "*.pk7" QFD_FILTER_DELIM "*.p7z" QFD_FILTER_DELIM "*.pkz);;"
        "All files (" QFD_FILTER_ALL ")";

    QStringList fileNames = QFileDialog::getOpenFileNames(this, "Add IWADs", getWadLastDir(), filters);
    for(int i = 0; i < fileNames.size(); i++){
        LOGDATAO() << "Adding file " << fileNames[i] << endl;
        saveWadLastDir(fileNames[i]);
        insert(new ZDLNameListable(pList, 1001, QFD_QT_SEP(fileNames[i]), ZDLIwadInfo(fileNames[i]).GetFileDescription()), -1);
    }
}

void ZDLIWadList::editButton(QListWidgetItem * item){
	if (item){
        QString filters =
            "IWAD files (*.wad" QFD_FILTER_DELIM "*.iwad" QFD_FILTER_DELIM "*.ipk3);;"
            "All supported archives (*.zip" QFD_FILTER_DELIM "*.pk3" QFD_FILTER_DELIM "*.ipk3" QFD_FILTER_DELIM "*.7z" QFD_FILTER_DELIM "*.pk7" QFD_FILTER_DELIM "*.p7z" QFD_FILTER_DELIM "*.pkz);;"
            "Specialized archives (*.pk3" QFD_FILTER_DELIM "*.ipk3" QFD_FILTER_DELIM "*.pk7" QFD_FILTER_DELIM "*.p7z" QFD_FILTER_DELIM "*.pkz);;"
            "All files (" QFD_FILTER_ALL ")";

		ZDLNameListable *zitem = (ZDLNameListable*)item;
		ZDLIwadInfo zdl_fi;
		ZDLNameInput diag(this, getWadLastDir(NULL, true), &zdl_fi);
		diag.setWindowTitle("Edit IWAD");
		diag.setFilter(filters);
		diag.basedOff(zitem);
		if(diag.exec()){
			saveWadLastDir(diag.getFile());
			zitem->setDisplayName(diag.getName());
			zitem->setFile(diag.getFile());
		}	
	}
}

