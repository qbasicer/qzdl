#include <QtGui>
#include <QApplication>
#include <QComboBox>

#include "configurationManager.h"
#include "settingPane.h"

settingPane::settingPane(ZQWidget *parent): ZQWidget(parent){
	QVBoxLayout *box = new QVBoxLayout(this);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(2);

	box->addWidget(new QLabel("Source Port",this));

	QComboBox *sourceList = new QComboBox(this);
	box->addWidget(sourceList);

	box->addWidget(new QLabel("IWAD",this));

	QListWidget *IWADList = new QListWidget(this);
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

}
