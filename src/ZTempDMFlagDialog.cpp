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
 
#include "ZTempDMFlagDialog.h"
#include "configurationManager.h"
#include <QDialogButtonBox>

ZTempDMFlagDialog::ZTempDMFlagDialog(ZQWidget *parent):QDialog(parent){
	setWindowTitle("ZDLSharp DMFlag Editor");
	QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);
	QVBoxLayout *box = new QVBoxLayout(this);
	flagVal = new QLineEdit(this);
	box->addWidget(flagVal);
	box->addWidget(btnBox);
	dmflags = 0;
	connect(btnBox, SIGNAL(accepted()), this, SLOT(save()));
	connect(btnBox, SIGNAL(rejected()), this, SLOT(close()));
	flagVal->setFocus();
}

void ZTempDMFlagDialog::setValue(int val){
	dmflags = val;
	flagVal->setText(QString::number(dmflags));
}

int ZTempDMFlagDialog::value(){
	return dmflags;
}

void ZTempDMFlagDialog::save(){
	bool ok;
	dmflags = flagVal->text().toInt(&ok, 10);
	done(1);
}

void ZTempDMFlagDialog::close(){
	done(0);
}

