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
#include <QMainWindow>
#include "configurationManager.h"
#include "ZAdvancedMultiplayerDialog.h"
#include "ZAboutDialog.h"

#include "multiPane.h"
#include "zdlInterface.h"
#include "topPane.h"
#include "ZInfoBar.h"
#include "mainWindow.h"

#include "aup.xpm"
#include "adown.xpm"

extern mainWindow *mw;

zdlInterface::zdlInterface(QWidget *parent):ZQWidget(parent){
	configurationManager::setInterface(this);
	
	box = new QVBoxLayout(this);
	
	
	topPane *tpane = new topPane(this);
	QLayout *bpane = getBottomPane();
	
	mpane = new multiPane(this);
	mpane->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Maximum ));
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(0);
	ZInfoBar *zib = new ZInfoBar(this);
	box->addWidget(zib);
	
	configurationManager::setInfobar(zib);
	box->addWidget(tpane);
	box->addLayout(bpane);
	
}

QLayout *zdlInterface::getBottomPane(){
	QVBoxLayout *box = new QVBoxLayout();
	QLabel *ecla = new QLabel("Extra command line arguments",this);
	extraArgs = new QLineEdit(this);
	QLayout *pan = getButtonPane();
	layout()->setContentsMargins(0,0,0,0);
	setContentsMargins(0,0,0,0);
	box->setSpacing(2);
	box->addWidget(ecla);
	box->addWidget(extraArgs);
	box->addLayout(pan);
	return box;
}

QLayout *zdlInterface::getButtonPane(){
	QHBoxLayout *box = new QHBoxLayout();

	QPushButton *btnExit = new QPushButton("Exit", this);
	btnZDL = new QPushButton("ZDL", this);
	QPushButton *btnMSet = new QPushButton("Multi Settings", this);
	btnEpr = new QPushButton(this);
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
	return box;
}

void zdlInterface::launch(){
	mw->launch();
}

void zdlInterface::buttonPaneNewConfig(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.save");
	if (section){
		vector <ZDLLine*> vctr;
		section->getRegex("^dlgmode$", vctr);
		for(unsigned int i = 0; i < vctr.size(); i++){
			if (strcmp(vctr[i]->getValue(), "open") == 0){
				btnEpr->setIcon(QPixmap(adown));
			}else{
				btnEpr->setIcon(QPixmap(aup));
			}
		}
		if (vctr.size() < 1){
			btnEpr->setIcon(QPixmap(aup));
		}
	}else{
		btnEpr->setIcon(QPixmap(aup));
	}
	
}

void zdlInterface::mclick(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	int stat;
	if(zconf->hasValue("zdl.save","dlgmode")){
		QString txt = zconf->getValue("zdl.save","dlgmode",&stat);
		if(txt == "closed"){
			btnEpr->setIcon(QPixmap(adown));
			zconf->setValue("zdl.save", "dlgmode", "open");
		}else{
			zconf->setValue("zdl.save", "dlgmode", "closed");
			btnEpr->setIcon(QPixmap(aup));
		}
	}else{
		btnEpr->setIcon(QPixmap(adown));
		zconf->setValue("zdl.save", "dlgmode", "open");
	}
	configurationManager::getInterface()->newConfig();
}

void zdlInterface::ampclick(){
	ZAdvancedMultiplayerDialog *zampd = new ZAdvancedMultiplayerDialog(this);
	zampd->exec();
	delete zampd;
}

void zdlInterface::sendSignals(){
	emit buildParent(this);
	emit buildChildren(this);
}

void zdlInterface::saveConfigFile(){
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

void zdlInterface::loadConfigFile(){
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

void zdlInterface::aboutClick(){
	ZAboutDialog zad(this);
	zad.exec();
}

void zdlInterface::showCommandline(){
	sendSignals();
	QStringList cmdLst = mw->getArguments();
	QString cmd = cmdLst.join(" ");
	QMessageBox::information(this,"ZDLSharp Commandline", "Command line to be executed:\n\n"+mw->getExecutable()+" " +cmd,QMessageBox::Ok,QMessageBox::Ok);
}

void zdlInterface::rebuild(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	if(extraArgs->text().length() > 0){
		zconf->setValue("zdl.save", "efirst", extraArgs->text().toStdString().c_str());
	}else{
		zconf->deleteValue("zdl.save", "efirst");
	}
}

void zdlInterface::bottomPaneNewConfig(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	if(zconf->hasValue("zdl.save", "efirst")){
		int stat;
		string rc = zconf->getValue("zdl.save", "efirst", &stat);
		if(rc.length() > 0){
			extraArgs->setText(rc.c_str());
		}
	}else{
		extraArgs->setText("");
	}
}

//Called when there's a change to the configuration that we need to look at.
//The button changed the configuration, and then notifies us that we need
//to look at the configuration to see what we need to do.
void zdlInterface::newConfig(){
	buttonPaneNewConfig();
	bottomPaneNewConfig();
	//Grab our configuration
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	//Grab our section in the configuration
	ZDLSection *section = zconf->getSection("zdl.save");
	//Do we have it?
	if (section){
		vector <ZDLLine*> vctr;
		//Search for our string
		section->getRegex("^dlgmode$", vctr);
		for(unsigned int i = 0; i < vctr.size(); i++){
			//Are we open?
			if (strcmp(vctr[i]->getValue(), "open") == 0){
				//cout << "Opening config" << endl;
				box->addWidget(mpane);
				mpane->setVisible(true);
			//No?
			}else{
				//cout << "Removing config" << endl;
				box->removeWidget(mpane);
				mpane->setVisible(false);
				mpane->hide();
			}
		}
		//Do we have the section, but not the key?
		if (vctr.size() < 1){
			//cout << "Removing config (no line present)" << endl;
			box->removeWidget(mpane);
			mpane->setVisible(false);
		}
	//Do we not even have the section?
	}else{
		//cout << "Removing config (no section present)" << endl;
		box->removeWidget(mpane);
	}
	this->update();
}



void zdlInterface::startRead(){
	emit readChildren(this);
	newConfig();
}

void zdlInterface::writeConfig(){
	emit buildChildren(this);
}
