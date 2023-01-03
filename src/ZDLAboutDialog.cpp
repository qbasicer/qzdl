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

#include "ZDLAboutDialog.h"
#include "zdlcommon.h"
#include "zdlconf.h"
#include <QDialogButtonBox>
#include "ZDLVersion.h"
#include "xpm/bmp_logo.xpm"

extern QString versionString;

ZDLAboutDialog::ZDLAboutDialog(ZDLWidget *parent):QDialog(parent){
	setWindowTitle("About ZDL");
	QVBoxLayout *box = new QVBoxLayout(this);
	QHBoxLayout *hbox = new QHBoxLayout();
	box->addLayout(hbox);
	QVBoxLayout *vbox = new QVBoxLayout();
	QLabel *title = new QLabel(QString("ZDL")+" "+ZDL_VERSION_STRING,this);

	QFont font;
	font.setPointSize(24);
	title->setFont(font);
	title->setAlignment(Qt::AlignHCenter);

	vbox->addWidget(title);

	QLabel *pic = new QLabel(this);
	QPixmap map(":zdl3.svg");
	if(!map.isNull()){
	        pic->setPixmap(map);
	}else{
		pic->setPixmap(QPixmap(aboutImg));
	}

	hbox->addWidget(pic);
	hbox->addLayout(vbox);
	QFrame *hrTop = new QFrame(this);
	hrTop->setFrameStyle(QFrame::HLine);
	box->addWidget(hrTop);
	box->addWidget(new QLabel(QString("Source: ")+QString(ZDL_SOURCE),this));
	box->addWidget(new QLabel("Copyright (C) ZDL Software Foundation 2004-2012", this));
	QLabel *url = new QLabel("<a href=http://zdl.vectec.net>http://zdl.vectec.net</a>",this);
	url->setOpenExternalLinks(true);
	box->addWidget(url);
	box->addWidget(new QLabel(QString("Source: ")+QString(ZDL_SOURCE),this));
	box->addWidget(new QLabel(QString("Build: ")+QString(ZDL_BUILD),this));
	box->addWidget(new QLabel(QString("Revision: ")+QString(ZDL_REVISION),this));
	QFrame *hrMid = new QFrame(this);
	hrMid->setFrameStyle(QFrame::HLine);
	box->addWidget(hrMid);

	box->addWidget(new QLabel("Special thanks to BioHazard for the original version",this));
	box->addWidget(new QLabel("Huge thanks to NeuralStunner.  Without his help, none of this would be possible.", this));
	box->addWidget(new QLabel("Special thanks to Blzut3, Risen, Enjay, DRDTeam.org, ZDoom.org, Dugan Chen",this));

	auto conf = ZDLConfigurationManager::getActiveConfiguration();
	QString userConfPath = conf->fileName();

	QFrame *hrBot = new QFrame(this);
	hrBot->setFrameStyle(QFrame::HLine);
	box->addWidget(hrBot);

	QLabel *userConf = new QLabel("User Configuration File: "+userConfPath,this);
	userConf->setCursor(Qt::IBeamCursor);
	userConf->setTextInteractionFlags(Qt::TextSelectableByMouse);

	box->addWidget(userConf);


	QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok,Qt::Horizontal,this);
	box->addWidget(btnBox);
	connect(btnBox, &QDialogButtonBox::accepted, this, &ZDLAboutDialog::close);
}

void ZDLAboutDialog::close(){
	done(0);
}
