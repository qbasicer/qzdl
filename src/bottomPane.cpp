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

#include "bottomPane.h"
#include "buttonPane.h"

#include "configurationManager.h"

bottomPane::bottomPane(ZQWidget *parent): ZQWidget(parent){
	QVBoxLayout *box = new QVBoxLayout(this);
	QLabel *ecla = new QLabel("Extra command line arguments",this);
	extraArgs = new QLineEdit(this);
	buttonPane *pan = new buttonPane(this);
	layout()->setContentsMargins(0,0,0,0);
	setContentsMargins(0,0,0,0);
	box->setSpacing(2);
	box->addWidget(ecla);
	box->addWidget(extraArgs);
	box->addWidget(pan);
}

void bottomPane::rebuild(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	if(extraArgs->text().length() > 0){
		zconf->setValue("zdl.save", "efirst", extraArgs->text().toStdString().c_str());
	}else{
		zconf->deleteValue("zdl.save", "efirst");
	}
}

void bottomPane::newConfig(){
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
