#include <QtGui>
#include <QApplication>
#include <QComboBox>

#include "configurationManager.h"
#include "settingPane.h"

settingPane::settingPane(QWidget *parent){
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

	QLineEdit *warpText = new QLineEdit(this);
	warpBox->addWidget(new QLabel("Warp",this));
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
	
	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		int count = 0;
		vector <ZDLLine*> fileVctr;
		section->getRegex("^p[0-9]+f$", fileVctr);
		
		for(int i = 0; i < fileVctr.size(); i++){
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
		
		for(int i = 0; i < fileVctr.size(); i++){
			string value = fileVctr[i]->getVariable();
			
			string number = "^i";
			number.append(value.substr(1, value.length()-2));
			number.append("n$");
			
			vector <ZDLLine*> nameVctr;
			section->getRegex(number.c_str(), nameVctr);
			if (nameVctr.size() == 1){
				if (sourceList->currentIndex() == count){
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
		string rc = zconf->getValue("zdl.save", "skill");
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
	
	sourceList->clear();
	
	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		vector <ZDLLine*> fileVctr;
		section->getRegex("^p[0-9]+f$", fileVctr);
		
		for(int i = 0; i < fileVctr.size(); i++){
			string value = fileVctr[i]->getVariable();
			
			string number = "^p";
			number.append(value.substr(1, value.length()-2));
			number.append("n$");
			
			vector <ZDLLine*> nameVctr;
			section->getRegex(number.c_str(), nameVctr);
			if (nameVctr.size() == 1){
				sourceList->addItem(nameVctr[0]->getValue());
			}
		}
	}
	
	if(zconf->hasValue("zdl.save", "port")){
		int index = 0;
		string rc = zconf->getValue("zdl.save", "port");
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
		cout << "Don't have port" << endl;
	}
	
	IWADList->clear();
	section = zconf->getSection("zdl.iwads");
	if (section){
		vector <ZDLLine*> fileVctr;
		section->getRegex("^i[0-9]+f$", fileVctr);
		
		for(int i = 0; i < fileVctr.size(); i++){
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
		string rc = zconf->getValue("zdl.save", "iwad");
		if (rc.length() > 0){
			index = atoi((char*)rc.c_str());
		}
		if (index >= 0 && index < sourceList->count()){
			IWADList->setCurrentRow(index);
		}else{
			zconf->setValue("zdl.save", "iwad", 0);
			IWADList->setCurrentRow(0);
		}
	}
	

}
