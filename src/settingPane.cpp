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

#include "configurationManager.h"
#include "settingPane.h"

settingPane::settingPane(QWidget *parent):ZQWidget(parent){
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

	warpText = new QLineEdit(this);
	warpBox->addWidget(new QLabel("Map",this));
	warpBox->addWidget(warpText);

	diffList = new QComboBox(this);
	skillBox->addWidget(new QLabel("Skill",this));
	skillBox->addWidget(diffList);
	diffList->addItem("None");
	diffList->addItem("V. Easy");
	diffList->addItem("Easy");
	diffList->addItem("Medium");
	diffList->addItem("Hard");
	diffList->addItem("V. Hard");

	
}

void settingPane::rebuild(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	if(diffList->currentIndex() > 0){
		zconf->setValue("zdl.save", "skill", diffList->currentIndex());
	}else{
		zconf->deleteValue("zdl.save", "skill");
	}
	
	if(warpText->text().length() > 0){
		zconf->setValue("zdl.save", "warp", warpText->text().toStdString().c_str());
	}else{
		zconf->deleteValue("zdl.save", "warp");
	}
	
	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		int count = 0;
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
				if (sourceList->currentIndex() == count){
					zconf->setValue("zdl.save", "port", value.substr(1, value.length()-2).c_str());
					break;
				}
				count++;
			}
		}
	}
	
	section = zconf->getSection("zdl.iwads");
	if (section){
		int count = 0;
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
				if (IWADList->currentRow() == count){
					zconf->setValue("zdl.save", "iwad", value.substr(1, value.length()-2).c_str());
					break;
				}
				count++;
			}
		}
	}

}

void settingPane::newConfig(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	if(zconf->hasValue("zdl.save", "skill")){
		int index = 0;
		int stat;
		string rc = zconf->getValue("zdl.save", "skill", &stat);
		if (rc.length() > 0){
			index = atoi((char*)rc.c_str());
		}
		if (index >= 0 && index <= 5){
			diffList->setCurrentIndex(index);
		}else{
			zconf->setValue("zdl.save", "skill", 0);
			diffList->setCurrentIndex(0);
		}
	
	}
	
	if(zconf->hasValue("zdl.save", "warp")){
		int stat;
		string rc = zconf->getValue("zdl.save","warp",&stat);
		if(rc.length() > 0){
			warpText->setText(QString(rc.c_str()));
		}else{
			warpText->setText("");
		}
	}else{
		warpText->setText("");
	}
	
	
	sourceList->clear();
	
	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		vector <ZDLLine*> fileVctr;
		section->getRegex("^p[0-9]+f$", fileVctr);
		
		for(unsigned int i = 0; i < fileVctr.size(); i++){
			string value = fileVctr[i]->getVariable();
			
			string number = "^p";
			number.append(value.substr(1, value.length()-2));
			number.append("n$");
			int stat;
			vector <ZDLLine*> nameVctr;
			section->getRegex(number.c_str(), nameVctr);
			if (nameVctr.size() == 1){
				sourceList->addItem(nameVctr[0]->getValue(),stat);
			}
		}
	}
	
	if(zconf->hasValue("zdl.save", "port")){
		int index = 0;
		int stat;
		string rc = zconf->getValue("zdl.save", "port", &stat);
		if (rc.length() > 0){
			index = atoi((char*)rc.c_str());
		}
		if (index >= 0 && index < sourceList->count()){
			sourceList->setCurrentIndex(index);
		}else{
			zconf->setValue("zdl.save", "port", 0);
			sourceList->setCurrentIndex(0);
		}
	}else{
		//cout << "Don't have port" << endl;
	}
	
	IWADList->clear();
	section = zconf->getSection("zdl.iwads");
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
				IWADList->addItem(nameVctr[0]->getValue());
			}
		}
	}
	
	if(zconf->hasValue("zdl.save", "iwad")){
		int index = 0;
		int stat;
		string rc = zconf->getValue("zdl.save", "iwad", &stat);
		if (rc.length() > 0){
			index = atoi((char*)rc.c_str());
		}
		if (index >= 0 && index < IWADList->count()){
			IWADList->setCurrentRow(index);
		}else{
			zconf->setValue("zdl.save", "iwad", 0);
			IWADList->setCurrentRow(0);
		}
	}
	

}
