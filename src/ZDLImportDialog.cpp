/*
 * This file is part of qZDL
 * Copyright (C) 2007-2012  Cody Harris
 * Copyright (C) 2018  Lcferrum
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

#include "ZDLImportDialog.h"
#include "zdlcommon.h"

ZDLImportDialog::ZDLImportDialog(QWidget *parent):QDialog(parent){
	QVBoxLayout *layout = new QVBoxLayout(this);
	this->setLayout(layout);
	setWindowTitle("Configuration Import");

	QString preamble = "ZDL";
	preamble += " has detected you have no user-level configuration!";
	preamble += "We encourage you to use the user-level configuration so";
	preamble += "that no matter where you are on your system, you will be";
	preamble += "able to access your ZDL configuration.  You don't have to,";
	preamble += "but we detected you haven't been asked this before, so we're";
	preamble += "asking this one time question. You have a few options:\n\n";
	preamble += "- Import the current .INI configuration as the new global\n";
	preamble += "- Do not import the current .INI and ask again for another\n";
	preamble += "- Never import and never ask this question again\n";
	preamble += "- Ask again later, do nothing now";
	QLabel *walloftext = new QLabel(preamble, this);
	walloftext->setWordWrap(true);	
	layout->addWidget(walloftext);
	QDialogButtonBox *btnGrp = new QDialogButtonBox(this);
	QPushButton *importNow = new QPushButton("Import Now", this);
	QPushButton *notThis = new QPushButton("Don't import this", this);
	QPushButton *noImport = new QPushButton("Never import", this);
	QPushButton *askLater = new QPushButton("Ask later", this);
	connect(importNow, SIGNAL(clicked()), this, SLOT(importNowClicked()));
	connect(notThis, SIGNAL(clicked()), this, SLOT(doNotImportThisClicked()));
	connect(noImport, SIGNAL(clicked()), this, SLOT(neverImportClicked()));
	connect(askLater, SIGNAL(clicked()), this, SLOT(askLaterClicked()));
	askLater->setDefault(true);
	askLater->setAutoDefault(true);
	importNow->setAutoDefault(false);
	importNow->setDefault(false);
	notThis->setAutoDefault(false);
	notThis->setDefault(false);
	noImport->setAutoDefault(false);
	noImport->setDefault(false);
	btnGrp->addButton(importNow, QDialogButtonBox::YesRole);
	btnGrp->addButton(notThis, QDialogButtonBox::ActionRole);
	btnGrp->addButton(noImport, QDialogButtonBox::ActionRole);
	btnGrp->addButton(askLater, QDialogButtonBox::NoRole);
	layout->addWidget(btnGrp);
	askLater->setDefault(true);
	askLater->setFocus(Qt::PopupFocusReason);
	importAction = UNKNOWN;
}

void ZDLImportDialog::askLaterClicked(){
	importAction = ASKLATER;
	reject();
}

void ZDLImportDialog::importNowClicked(){
	importAction = IMPORTNOW;
	accept();
}

void ZDLImportDialog::doNotImportThisClicked(){
	importAction = DONOTIMPORTTHIS;
	accept();
}

void ZDLImportDialog::neverImportClicked(){
	importAction = NEVERIMPORT;
	accept();
}

