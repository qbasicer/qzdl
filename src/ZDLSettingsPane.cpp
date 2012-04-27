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
#include <QComboBox>
#include "ZDLMapFile.h"

#include "ZDLConfigurationManager.h"
#include "ZDLSettingsPane.h"

ZDLSettingsPane::ZDLSettingsPane(QWidget *parent):ZDLWidget(parent){
	LOGDATAO() << "New ZDLSettingsPane" << endl;
	QVBoxLayout *box = new QVBoxLayout(this);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(2);

	box->addWidget(new QLabel("Source Port",this));

	sourceList = new QComboBox(this);
	
	
	box->addWidget(sourceList);

	box->addWidget(new QLabel("IWAD",this));

	IWADList = new QListWidget(this);
	box->addWidget(IWADList);

	QHBoxLayout *box2 = new QHBoxLayout();
	box->addLayout(box2);

	QVBoxLayout *warpBox = new QVBoxLayout();
	box2->addLayout(warpBox);

	QVBoxLayout *skillBox = new QVBoxLayout();
	box2->addLayout(skillBox);

	warpCombo = new QComboBox(this);
	warpCombo->setEditable(true);
	warpBox->addWidget(new QLabel("Map",this));
	warpBox->addWidget(warpCombo);

	diffList = new QComboBox(this);
	skillBox->addWidget(new QLabel("Skill",this));
	skillBox->addWidget(diffList);
	diffList->addItem("None");
	diffList->addItem("V. Easy");
	diffList->addItem("Easy");
	diffList->addItem("Medium");
	diffList->addItem("Hard");
	diffList->addItem("V. Hard");
	LOGDATAO() << "Done" << endl;
	connect(IWADList, SIGNAL(currentRowChanged(int)), this, SLOT(currentRowChanged(int)));
	
}

void ZDLSettingsPane::currentRowChanged(int idx){
	reloadMapList();
}

QStringList ZDLSettingsPane::getFilesMaps(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(!zconf){
		return QStringList();
	}
	ZDLSection *section = zconf->getSection("zdl.save");
	if(!section){
		return QStringList();
	}
	QVector<ZDLLine*> vctr;
	section->getRegex("^file[0-9]+$", vctr);
	if(vctr.size() <= 0){
		return QStringList();
	}
	QStringList maps;
	for(int i = vctr.size()-1; i >= 0; i--){
		ZDLMapFile *mapfile = ZDLMapFile::getMapFile(vctr[i]->getValue());
		if(!mapfile){
			continue;
		}
		if(~mapfile->open()){
			delete mapfile;
			continue;
		}
		maps += mapfile->getMapNames();
	}
	return maps;
}

void ZDLSettingsPane::reloadMapList(){
	
	QStringList iwadMaps;
	QListWidgetItem *item = IWADList->currentItem();
	if(!item){
		return;
	}
	QVariant data = item->data(32);
	if(!data.isValid() && !data.isNull()){
                LOGDATAO() << "Item data is NULL" << endl;
                return;
        }
	QString file = data.toString();
        if(file.isNull() || file.isEmpty()){
                LOGDATAO() << "Bad string" << endl;
                return;
        }
        QFileInfo fi(file);
        if(fi.exists() && file.endsWith(".wad", Qt::CaseInsensitive)){
                iwadMaps = getMapNamesForWad(file);
        }else{
                LOGDATAO() << "File doesn't exist- " << file << endl;
        }
	QStringList filesMaps = getFilesMaps();
	if(filesMaps.size() + iwadMaps.size() > 0){
		warpCombo->clear();
		warpCombo->addItem("");
		warpCombo->addItems(filesMaps);
		warpCombo->addItems(iwadMaps);
	}
}

void ZDLSettingsPane::rebuild(){
	LOGDATAO() << "Saving config" << endl;
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(diffList->currentIndex() > 0){
		zconf->setValue("zdl.save", "skill", diffList->currentIndex());
	}else{
		zconf->deleteValue("zdl.save", "skill");
	}
	
	if(warpCombo->currentText().length() > 0){
		zconf->setValue("zdl.save", "warp", warpCombo->currentText());
	}else{
		zconf->deleteValue("zdl.save", "warp");
	}
	
	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		int count = 0;
		QVector<ZDLLine*> fileVctr;
		section->getRegex(QString("^p[0-9]+f$"), fileVctr);
		
		for(int i = 0; i < fileVctr.size(); i++){
			QString value = fileVctr[i]->getVariable();
			
			QString number = "^p";
			number.append(value.mid(1, value.length()-2));
			number.append("n$");
			
			QVector<ZDLLine*> nameVctr;
			section->getRegex(number, nameVctr);
			if (nameVctr.size() == 1){
				if (sourceList->currentIndex() == count){
					zconf->setValue("zdl.save", "port", nameVctr[0]->getValue());
					break;
				}
				count++;
			}
		}
	}
	
	section = zconf->getSection("zdl.iwads");
	if (section){
		int count = 0;
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^i[0-9]+f$", fileVctr);
			
		for(int i = 0; i < fileVctr.size(); i++){
			QString value = fileVctr[i]->getVariable();
			
			QString number = "^i";
			number.append(value.mid(1, value.length()-2));
			number.append("n$");
			
			QVector <ZDLLine*> nameVctr;
			section->getRegex(number, nameVctr);
			if (nameVctr.size() == 1){
				if (IWADList->currentRow() == count){
					zconf->setValue("zdl.save", "iwad", nameVctr[0]->getValue());
					
					break;
				}
				count++;
			}
		}
	}

}

void ZDLSettingsPane::newConfig(){
	LOGDATAO() << "Loading new config" << endl;
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(zconf->hasValue("zdl.save", "skill")){
		int index = 0;
		int stat = 0;
		QString rc = zconf->getValue("zdl.save", "skill", &stat);
		if (rc.length() > 0){
			index = atoi((char*)rc.toStdString().c_str());
		}
		if (index >= 0 && index <= 5){
			diffList->setCurrentIndex(index);
		}else{
			zconf->setValue("zdl.save", "skill", 0);
			diffList->setCurrentIndex(0);
		}
	
	}else{
		diffList->setCurrentIndex(0);
	}
	reloadMapList();
	if(zconf->hasValue("zdl.save", "warp")){
		int stat = 0;
		QString rc = zconf->getValue("zdl.save","warp",&stat);
		if(rc.length() > 0){
			QLineEdit *edit = warpCombo->lineEdit();
			if(edit){
				edit->setText(rc);
			}
		}else{
			QLineEdit *edit = warpCombo->lineEdit();
			if(edit){
				edit->setText("");
			}
		}
	}else{
		QLineEdit *edit = warpCombo->lineEdit();
		if(edit){
			edit->setText("");
		}
	}
	
	
	sourceList->clear();
	
	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^p[0-9]+f$", fileVctr);
		
		for(int i = 0; i < fileVctr.size(); i++){
			QString value = fileVctr[i]->getVariable();
			
			QString number = "^p";
			number.append(value.mid(1, value.length()-2));
			number.append("n$");
			int stat = 0;
			QVector<ZDLLine*> nameVctr;
			section->getRegex(number, nameVctr);
			if (nameVctr.size() == 1){
				sourceList->addItem(nameVctr[0]->getValue(),stat);
			}
		}
	}
	
	if(zconf->hasValue("zdl.save", "port")){
		int set = 0;
		int stat = 0;
		QString rc = zconf->getValue("zdl.save", "port", &stat);

		if(rc.length() > 0){
			for(int i = 0; i < sourceList->count(); i++){
				if(sourceList->itemText(i).compare(rc) == 0){
					sourceList->setCurrentIndex(i);
					set = 1;
					break;
				}
			}
			if(set == 0){
				sourceList->setCurrentIndex(0);
			}
		}	
	}else{
		//cout << "Don't have port" << endl;
	}
	
	IWADList->clear();
	section = zconf->getSection("zdl.iwads");
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
				QListWidgetItem *item = new QListWidgetItem(nameVctr[0]->getValue(),IWADList, 1001);
				item->setData(32,fileVctr[i]->getValue());
				IWADList->addItem(item);
			}
		}
	}
	
	if(zconf->hasValue("zdl.save", "iwad")){
		int set = 0;
		int stat = 0;
		QString rc = zconf->getValue("zdl.save", "iwad", &stat);
		if (rc.length() > 0){
			for(int i = 0; i < IWADList->count(); i++){
				QListWidgetItem *item = IWADList->item(i);
				QString text = item->text();
				if(text.compare(rc) == 0){
					set = 1;
					IWADList->setCurrentRow(i);
					break;
				}	
			}
		}
		if(!set){
			if(IWADList->count() == 0){
				zconf->deleteValue("zdl.save", "iwad");
			}else{
				IWADList->setCurrentRow(0);
				QListWidgetItem *item = IWADList->item(0);
				zconf->setValue("zdl.save","iwad",item->text());
			}
		}
	}

}
