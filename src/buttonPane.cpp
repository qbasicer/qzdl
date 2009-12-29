#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include "configurationManager.h"
#include <string.h>

#include "buttonPane.h"
#include "mainWindow.h"
#include "ZAdvancedMultiplayerDialog.h"
#include "ZAboutDialog.h"

extern mainWindow *mw;

buttonPane::buttonPane(ZQWidget *parent): ZQWidget(parent){
	QHBoxLayout *box = new QHBoxLayout(this);

	QPushButton *btnExit = new QPushButton("Exit", this);
	btnZDL = new QPushButton("ZDL", this);
	QPushButton *btnMSet = new QPushButton("Multi Settings", this);
	btnEpr = new QPushButton("\\/", this);
	QPushButton *btnLaunch = new QPushButton("Launch", this);

	QMenu *context = new QMenu(btnZDL);
	QMenu *actions = new QMenu("Actions",context);
	
	QAction *showCommandline = actions->addAction("Show Command Line");
	actions->addAction("Clear PWAD list");
	actions->addAction("Clear all fields");
	
	context->addMenu(actions);
	context->addSeparator();
	QAction *loadAction = context->addAction("Load Config");
	QAction *saveAction = context->addAction("Save Config");
	context->addSeparator();
	QAction *aboutAction = context->addAction("About");
	
	connect(loadAction, SIGNAL(triggered()), this, SLOT(loadConfigFile()));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveConfigFile()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutClick()));
	connect(showCommandline, SIGNAL(triggered()),this,SLOT(showCommandline()));
	
	btnZDL->setMenu(context);
	
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
	
	connect(btnMSet, SIGNAL(clicked()), this, SLOT(ampclick()));
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

void buttonPane::ampclick(){
	ZAdvancedMultiplayerDialog *zampd = new ZAdvancedMultiplayerDialog(this);
	zampd->exec();
	delete zampd;
}

void buttonPane::sendSignals(){
	emit buildParent(this);
	emit buildChildren(this);
}

void buttonPane::saveConfigFile(){
	sendSignals();
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	QStringList filters;
	filters << "ZDL/ini (*.zdl *.ini)"
			<< "ZDL files (*.zdl)"
			<< "ini Files (*.ini)"
			<< "Any files (*)";
	QFileDialog dialog(this,"Save");
	dialog.setFilters(filters);
	QStringList fileNames;
	if(dialog.exec()){
		fileNames = dialog.selectedFiles();
		for(int i = 0; i < fileNames.size(); i++){
			configurationManager::setConfigFileName(fileNames[i]);
			zconf->writeINI(fileNames[i].toStdString().c_str());
		}
		mw->startRead();
	}
	
}

void buttonPane::loadConfigFile(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	QStringList filters;
	filters << "ZDL/ini (*.zdl *.ini)"
			<< "ZDL files (*.zdl)"
			<< "ini Files (*.ini)"
			<< "Any files (*)";
	QFileDialog dialog(this);
	dialog.setFilters(filters);
	dialog.setFileMode(QFileDialog::ExistingFile);
	QStringList fileNames;
	if(dialog.exec()){
		fileNames = dialog.selectedFiles();
		for(int i = 0; i < fileNames.size(); i++){
			delete zconf;
			ZDLConf* tconf = new ZDLConf();
			configurationManager::setConfigFileName(fileNames[i]);
			tconf->readINI(fileNames[i].toStdString().c_str());
			configurationManager::setActiveConfiguration(tconf);
			
		}
		mw->startRead();
	}
	
}

void buttonPane::aboutClick(){
	ZAboutDialog zad(this);
	zad.exec();
}

void buttonPane::showCommandline(){
	sendSignals();
	QStringList cmdLst = mw->getArguments();
	QString cmd = cmdLst.join(" ");
	QMessageBox::information(this,"qZDL Commandline", "Command line to be executed:\n\n"+mw->getExecutable()+" " +cmd,QMessageBox::Ok,QMessageBox::Ok);
}
