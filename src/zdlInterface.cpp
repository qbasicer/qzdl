#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include "configurationManager.h"

#include "multiPane.h"
#include "zdlInterface.h"
#include "bottomPane.h"
#include "topPane.h"

zdlInterface::zdlInterface(QWidget *parent):ZQWidget(parent){
	configurationManager::setInterface(this);
	box = new QVBoxLayout(this);
	topPane *tpane = new topPane(this);
	bottomPane *bpane = new bottomPane(this);
	mpane = new multiPane(this);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(0);
	box->addWidget(tpane);
	box->addWidget(bpane);
}

void zdlInterface::newConfig(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.save");
	if (section){
		vector <ZDLLine*> vctr;
		section->getRegex("^dlgmode$", vctr);
		for(int i = 0; i < vctr.size(); i++){
			if (strcmp(vctr[i]->getValue(), "open") == 0){
				cout << "Opening config" << endl;
				box->addWidget(mpane);
				
			}else{
				cout << "Removing config" << endl;
				box->removeWidget(mpane);
			}
		}
		//506-472 6459
		if (vctr.size() < 1){
			cout << "Removing config (no line present)" << endl;
			box->removeWidget(mpane);
		}
	}else{
		cout << "Removing config (no section present)" << endl;
		box->removeWidget(mpane);
	}
	box->update();
}



void zdlInterface::startRead(){
	emit readChildren(this);
	newConfig();
}

void zdlInterface::writeConfig(){
	emit buildChildren(this);
}
