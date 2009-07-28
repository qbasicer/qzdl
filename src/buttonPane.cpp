#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include "configurationManager.h"
#include <string.h>

#include "buttonPane.h"
#include "mainWindow.h"

extern mainWindow *mw;

buttonPane::buttonPane(ZQWidget *parent): ZQWidget(parent){
	QHBoxLayout *box = new QHBoxLayout(this);

	QPushButton *btnExit = new QPushButton("Exit", this);
	QPushButton *btnZDL = new QPushButton("ZDL", this);
	QPushButton *btnMSet = new QPushButton("Multi Settings", this);
	btnEpr = new QPushButton("\\/", this);
	QPushButton *btnLaunch = new QPushButton("Launch", this);

	int minBtnWidth = 50;

	btnExit->setMinimumWidth(minBtnWidth-20);
	btnZDL->setMinimumWidth(minBtnWidth-20);
	btnMSet->setMinimumWidth(minBtnWidth+40);
	btnEpr->setMinimumWidth(20);
	btnLaunch->setMinimumWidth(minBtnWidth);

	connect(btnLaunch, SIGNAL( clicked() ), this, SLOT(launch()));

	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(2);
	box->addWidget(btnExit);
	box->addWidget(btnZDL);
	box->addWidget(btnMSet);
	box->addWidget(btnEpr);
	box->addWidget(btnLaunch);
	connect(btnEpr, SIGNAL(clicked()), this, SLOT(mclick()));
}

void buttonPane::launch(){
	mw->launch();
}

void buttonPane::newConfig(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.save");
	if (section){
		vector <ZDLLine*> vctr;
		section->getRegex("^dlgmode$", vctr);
		for(unsigned int i = 0; i < vctr.size(); i++){
			if (strcmp(vctr[i]->getValue(), "open") == 0){
				btnEpr->setText("^");
			}else{
				btnEpr->setText("\\/");
			}
		}
		if (vctr.size() < 1){
			btnEpr->setText("\\/");
		}
	}else{
		btnEpr->setText("\\/");
	}
	
}

void buttonPane::mclick(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	if (btnEpr->text() == "^"){
		zconf->setValue("zdl.save", "dlgmode", "closed");
		btnEpr->setText("\\/");
	}else{
		cout << "closing dialog (down arrow)" << endl;
		zconf->setValue("zdl.save", "dlgmode", "open");
		btnEpr->setText("^");
	}
	configurationManager::getInterface()->newConfig();
}

void buttonPane::sendSignals(){
	emit buildParent(this);
	emit buildChildren(this);
}
