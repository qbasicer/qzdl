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
#include "configurationManager.h"
#include "multiPane.h"
#include "ZTempDMFlagDialog.h"
#include "ZDMFlagDialog.h"


multiPane::multiPane(ZQWidget *parent): ZQWidget(parent){
	QVBoxLayout *box = new QVBoxLayout(this);

	gMode = new QComboBox(this);
	gMode->addItem("Single Player");
	gMode->addItem("Co-op");
	gMode->addItem("Deathmatch");
	
	
	tHostAddy = new QLineEdit(this);
	
	gPlayers = new QComboBox(this);
	gPlayers->addItem("Joining");
	gPlayers->addItem("1");
	gPlayers->addItem("2");
	gPlayers->addItem("3");
	gPlayers->addItem("4");
	gPlayers->addItem("5");
	gPlayers->addItem("6");
	gPlayers->addItem("7");
	gPlayers->addItem("8");
	
	tFragLimit = new QLineEdit(this);
	
	bDMFlags = new QPushButton("0",this);
	
	bDMFlags2 = new QPushButton("0",this);
	
	QGridLayout *topGrid = new QGridLayout();
	topGrid->addWidget(new QLabel("Game Mode",this),0,0,1,2);
	topGrid->addWidget(gMode,1,0,1,2);
	topGrid->addWidget(new QLabel("Hostname/IP Address",this),0,2,1,2);
	topGrid->addWidget(tHostAddy,1,2,1,2);
	topGrid->addWidget(new QLabel("Players",this),2,0);
	topGrid->addWidget(gPlayers,3,0);
	topGrid->addWidget(new QLabel("Frag Limit",this),2,1);
	topGrid->addWidget(tFragLimit,3,1);
	topGrid->addWidget(new QLabel("DMFLAGS",this),2,2);
	topGrid->addWidget(bDMFlags,3,2);
	topGrid->addWidget(new QLabel("DMFLAGS2",this),2,3);
	topGrid->addWidget(bDMFlags2,3,3);
	box->addLayout(topGrid);
	
	topGrid->setSpacing(0);

	layout()->setContentsMargins(0,0,0,0);
	
	connect(bDMFlags, SIGNAL(clicked()), this, SLOT(dmflags()));
	connect(bDMFlags2, SIGNAL(clicked()), this, SLOT(dmflags2()));

}

void multiPane::newConfig(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.save");
	if (section && section->hasVariable("host")){
		int stat;
		QString hostName = zconf->getValue("zdl.save","host",&stat);
		tHostAddy->setText(hostName);
	}else{
		tHostAddy->setText("");
	}
	if (section && section->hasVariable("players")){
		QString strPlayers = section->findVariable("players");
		bool ok;
		int noPlayers = strPlayers.toInt(&ok, 10);
		if (noPlayers >= 0 && noPlayers <= 8 && ok){
			gPlayers->setCurrentIndex(noPlayers);
		}else{
			gPlayers->setCurrentIndex(0);
		}
		
	}else{
		gPlayers->setCurrentIndex(0);
	}
	if (section && section->hasVariable("gametype")){
		QString strGType = section->findVariable("gametype");
		bool ok;
		int gType = strGType.toInt(&ok, 10);
		if (gType >= 0 && gType <= 2 && ok){
			gMode->setCurrentIndex(gType);
		}else{
			gMode->setCurrentIndex(0);
		}
		
	}else{
		gMode->setCurrentIndex(0);
	}
	if (section && section->hasVariable("dmflags")){
		QString dmFlags = section->findVariable("dmflags");
		bool ok;
		int flags = dmFlags.toInt(&ok, 10);
		if (!ok && flags >= 0){
			bDMFlags->setText("0");
		}else{
			bDMFlags->setText(dmFlags);
		}
	}else{
		bDMFlags->setText("0");
	}
	if (section && section->hasVariable("dmflags2")){
		QString dmFlags = section->findVariable("dmflags2");
		bool ok;
		int flags = dmFlags.toInt(&ok, 10);
		if (!ok && flags >= 0){
			bDMFlags2->setText("0");
		}else{
			bDMFlags2->setText(dmFlags);
		}
	}else{
		bDMFlags2->setText("0");
	}
	
	if (section && section->hasVariable("fraglimit")){
		QString dmFlags = section->findVariable("fraglimit");
		bool ok;
		int flags = dmFlags.toInt(&ok, 10);
		if (!ok && flags >= 0){
			tFragLimit->setText("");
		}else{
			tFragLimit->setText(dmFlags);
		}
	}else{
		tFragLimit->setText("");
	}
	
}

void multiPane::rebuild(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	if (tHostAddy->text().length() > 0){
		zconf->setValue("zdl.save", "host", tHostAddy->text().toStdString().c_str());
	}
	if (tFragLimit->text().length() > 0){
		zconf->setValue("zdl.save", "fraglimit", tFragLimit->text().toStdString().c_str());
	}
	zconf->setValue("zdl.save", "dmflags", bDMFlags->text().toStdString().c_str());
	zconf->setValue("zdl.save", "dmflags2", bDMFlags2->text().toStdString().c_str());
	zconf->setValue("zdl.save", "gametype", gMode->currentIndex());
	zconf->setValue("zdl.save", "players", gPlayers->currentIndex());
}


void multiPane::dmflags(){
	ZDMFlagDialog dialog(this);
	bool ok;
	dialog.setValue(bDMFlags->text().toInt(&ok, 10));
	dialog.setValue2(bDMFlags2->text().toInt(&ok, 10));
	int ret = dialog.exec();
	if (ret == 1){
		bDMFlags->setText(QString::number(dialog.value()));
		bDMFlags2->setText(QString::number(dialog.value2()));
	}
}

void multiPane::dmflags2(){
	dmflags();
}











