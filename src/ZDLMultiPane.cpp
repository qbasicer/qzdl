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


#include <QtWidgets>
#include <QApplication>
#include "zdlconf.h"
#include "ZDLMultiPane.h"


ZDLMultiPane::ZDLMultiPane(ZDLWidget *parent): ZDLWidget(parent){
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

	bDMFlags = new QLineEdit("0", this);
	bDMFlags2 = new QLineEdit("0", this);

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
}

void ZDLMultiPane::newConfig(){
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	if (zconf->contains("zdl.save/host")){
		auto hostName = zconf->value("zdl.save/host").toString();
		tHostAddy->setText(hostName);
	}else{
		tHostAddy->setText("");
	}
	if (zconf->contains("zdl.save/players")) {
		auto strPlayers = zconf->value("zdl.save/players").toString();
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
	if (zconf->contains("zdl.save/gametype")){
		auto strGType = zconf->value("zdl.save/gametype").toString();
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
	if (zconf->contains("zdl.save/dmflags")){
		auto dmFlags = zconf->value("zdl.save/dmflags").toString();
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
	if (zconf->contains("zdl.save/dmflags2")){
		auto dmFlags = zconf->value("zdl.save/dmflags2").toString();
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
	if (zconf->contains("zdl.save/fraglimit")){
		auto dmFlags = zconf->value("zdl.save/fraglimit").toString();
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

void ZDLMultiPane::rebuild(){
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	if (tHostAddy->text().length() > 0){
		zconf->setValue("zdl.save/host", tHostAddy->text());
	}else{
		zconf->remove("zdl.save/host");
	}

	if (tFragLimit->text().length() > 0){
		zconf->setValue("zdl.save/fraglimit", tFragLimit->text());
	}else{
		zconf->remove("zdl.save/fraglimit");
	}

	if(bDMFlags->text() != "0"){
		zconf->setValue("zdl.save/dmflags", bDMFlags->text());
	}else{
		zconf->remove("zdl.save/dmflags");
	}

	if(bDMFlags2->text() != "0"){
		zconf->setValue("zdl.save/dmflags2", bDMFlags2->text());
	}else{
		zconf->remove("zdl.save/dmflags2");
	}

	zconf->setValue("zdl.save/gametype", gMode->currentIndex());
	zconf->setValue("zdl.save/players", gPlayers->currentIndex());
}


QString ZDLMultiPane::getMode()
{
	return gMode->currentText();
}

QString ZDLMultiPane::getHostAddy()
{
	return tHostAddy->text();
}

QString ZDLMultiPane::getPlayers()
{
	return gPlayers->currentText();
}

QString ZDLMultiPane::getFragLmit()
{
	return tFragLimit->text();
}

QString ZDLMultiPane::getDMFlags()
{
	return bDMFlags->text();
}

QString ZDLMultiPane::getDMFlags2()
{
	return bDMFlags2->text();
}
