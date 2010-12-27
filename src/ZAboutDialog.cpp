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
 
#include "ZAboutDialog.h"
#include "configurationManager.h"
#include <QDialogButtonBox>
#include "bmp_logo.xpm"

extern QString versionString;

ZAboutDialog::ZAboutDialog(ZQWidget *parent):QDialog(parent){
	setWindowTitle("About ZDLSharp");
	QVBoxLayout *box = new QVBoxLayout(this);
	QHBoxLayout *hbox = new QHBoxLayout();
	box->addLayout(hbox);
	QVBoxLayout *vbox = new QVBoxLayout();
	QLabel *title = new QLabel("ZDL<i>Sharp</i> "+versionString,this);
	
	QFont font;
	font.setPointSize(24);
	title->setFont(font);
	title->setAlignment(Qt::AlignHCenter);
	
	vbox->addWidget(title);
	vbox->addWidget(new QLabel("C Version Copyright (C) BioHazard 2005",this));
	vbox->addWidget(new QLabel("C# Version Copyright (C) QBasicer 2007",this));
	vbox->addWidget(new QLabel("Qt Version Copyright (C) Cody Harris 2007-2011",this));
	QLabel *url = new QLabel("<a href=http://zdlsharp.vectec.net>http://zdlsharp.vectec.net</a>",this);
	url->setOpenExternalLinks(true);
	vbox->addWidget(url);
	
	QLabel *pic = new QLabel(this);
	pic->setPixmap(QPixmap(aboutImg));
	
	hbox->addWidget(pic);
	hbox->addLayout(vbox);
	
	
	box->addWidget(new QLabel("Special thanks to BioHazard for the original version",this));
	box->addWidget(new QLabel("Special thanks to Risen, Enjay, DRDTeam.org, ZDoom.org",this));
	
	ZDLConfiguration *conf = configurationManager::getConfiguration();
	if(conf){
		QString systemConfPath = conf->getPath(ZDLConfiguration::CONF_SYSTEM);
		QString userConfPath = conf->getPath(ZDLConfiguration::CONF_USER);
		
		box->addWidget(new QLabel(" ",this));
		
		QLabel *systemConf = new QLabel("System Configuration File: "+systemConfPath,this);
		systemConf->setTextInteractionFlags(Qt::TextSelectableByMouse);
		
		QLabel *userConf = new QLabel("User Configuration File: "+userConfPath,this);
		userConf->setTextInteractionFlags(Qt::TextSelectableByMouse);
		
		box->addWidget(systemConf);
		box->addWidget(userConf);
	}
	
	
	
	QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok,Qt::Horizontal,this);
	box->addWidget(btnBox);
	connect(btnBox, SIGNAL(accepted()), this, SLOT(close()));
}

void ZAboutDialog::close(){
	done(0);
}