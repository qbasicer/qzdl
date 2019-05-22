/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * Copyright (C) 2018-2019  Lcferrum
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
#include "ZDLNameInput.h"
#include "ZDLConfigurationManager.h"

ZDLNameInput::ZDLNameInput(QWidget *parent, const QString &last_used_dir, ZDLFileInfo *zdl_fi, bool alllow_dirs, bool allow_params):
	QDialog(parent), zdl_fi(zdl_fi), last_used_dir(last_used_dir), alllow_dirs(alllow_dirs), params_offset(allow_params?2:0)
{
	setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint); 
	QVBoxLayout *lays = new QVBoxLayout(this);
	QGridLayout *inputGrid = new QGridLayout();
	QHBoxLayout *ctrlButtons = new QHBoxLayout();
		
	QPushButton *btnOK = new QPushButton("OK", this);
	QPushButton *btnCancel = new QPushButton("Cancel", this);
	
	lname = new QLineEdit(this);
	lfile = new QLineEdit(this);
	btnBrowse = new QPushButton("...", this);
	btnBrowse->setMaximumWidth(26);

	inputGrid->addWidget(new QLabel("Name", this),0,0,1,2);
	inputGrid->addWidget(lname,1,0,1,2);
	inputGrid->addWidget(new QLabel("File", this),params_offset+2,0,1,2);
	inputGrid->addWidget(lfile,params_offset+3,0);
	inputGrid->addWidget(btnBrowse,params_offset+3,1);

	if (params_offset) {
		lparams = new QLineEdit(this);
		lparams->setPlaceholderText("(Optional)");
		inputGrid->addWidget(new QLabel("Parameters", this),2,0,1,2);
		inputGrid->addWidget(lparams,3,0,1,2);
	}

	inputGrid->setSpacing(2);
	inputGrid->setContentsMargins(0,0,0,0);

	ctrlButtons->addStretch();
	ctrlButtons->addWidget(btnOK);
	ctrlButtons->addWidget(btnCancel);
	ctrlButtons->setSpacing(4);
	ctrlButtons->setContentsMargins(0,0,0,0);

	lays->addLayout(inputGrid);
	lays->addLayout(ctrlButtons);
	lays->setSpacing(4);

	setContentsMargins(4,4,4,4);
	layout()->setContentsMargins(0,0,0,0);
	setFixedHeight(sizeHint().height());
	resize(350, sizeHint().height());
	
	connect(btnBrowse, SIGNAL(clicked()), this, SLOT(browse()));
	connect(btnOK, SIGNAL(clicked()), this, SLOT(okClick()));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void ZDLNameInput::browse(){
	QString fileName = QFileDialog::getOpenFileName(this, "Add file", last_used_dir, filters);
	if (!fileName.isEmpty()) {
		lfile->setText(QFD_QT_SEP(fileName));
		if (zdl_fi) {
			zdl_fi->setFile(fileName);
			lname->setText(zdl_fi->GetFileDescription());
		}
	}
}

void ZDLNameInput::okClick(){
	QFileInfo selected_file(lfile->text());

	if (selected_file.exists()&&(alllow_dirs||selected_file.isFile())) {
		if (lname->text().length()) {
			accept();
		} else {
			QMessageBox::warning(this, "ZDL", "Name can't be empty.");
		}
	} else {
		QMessageBox::warning(this, "ZDL", "File path is invalid.");
	}
}

void ZDLNameInput::fromUrl(QUrl url){
	lfile->setText(url.path());
}

void ZDLNameInput::basedOff(ZDLNameListable *listable){
	if (listable){
		lfile->setText(listable->getFile());
		lname->setText(listable->getName());
	}
}

void ZDLNameInput::setFilter(const QString &inFilters){
	filters = inFilters;
}

QString ZDLNameInput::getName() {
	return lname->text();
}

QString ZDLNameInput::getParams() {
	return lparams->text();
}

QString ZDLNameInput::getFile() {
	return lfile->text();
}
