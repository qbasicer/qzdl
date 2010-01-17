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
#include "ZNameInput.h"
#include <string>

ZNameInput::ZNameInput(QWidget *parent):QDialog(parent){
	QVBoxLayout *lays = new QVBoxLayout(this);
	QHBoxLayout *files = new QHBoxLayout();
	
	QHBoxLayout *buttons = new QHBoxLayout();
	
	QPushButton *btnOK = new QPushButton("OK", this);
	QPushButton *btnCancel = new QPushButton("Cancel", this);
	
	lname = new QLineEdit(this);
	lfile = new QLineEdit(this);
	btnBrowse = new QPushButton("Browse...", this);
	
	lays->addWidget(new QLabel("Name", this));
	lays->addWidget(lname);
	
	lays->addWidget(new QLabel("File", this));
	
	files->addWidget(lfile);
	files->addWidget(btnBrowse);
	
	lays->addLayout(files);
	
	buttons->addWidget(btnOK);
	buttons->addWidget(btnCancel);
	
	lays->addLayout(buttons);
	
	connect(btnBrowse, SIGNAL(clicked()), this, SLOT(browse()));
	connect(btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));	
	
	
}

void ZNameInput::browse(){
	QFileDialog dialog(this);
	dialog.setFilters(filters);
	dialog.setFileMode(QFileDialog::ExistingFile);
	QStringList fileNames;
	if(dialog.exec()){
		fileNames = dialog.selectedFiles();
		for(int i = 0; i < fileNames.size(); i++){
			lfile->setText(fileNames[i]);
			
		}

	}
}

void ZNameInput::basedOff(ZNameListable *listable){
	if (listable){
		lfile->setText(listable->getFile());
		lname->setText(listable->getName());
		
	}
}

void ZNameInput::setFilter(QStringList inFilters){
	filters = inFilters;
}

const char* ZNameInput::getName(){
	if (lname->text().length() > 0){
		return lname->text().toStdString().c_str();
	}else{
		return lfile->text().toStdString().c_str();
	}
}

const char* ZNameInput::getFile(){
	return lfile->text().toStdString().c_str();
}
