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
 
#include "ZDMFlagDialog.h"
#include "ZDLConfigurationManager.h"
#include <QDialogButtonBox>

ZDMFlagDialog::ZDMFlagDialog(ZDLWidget *parent):QDialog(parent){
	setWindowTitle(ZDL_ENGINE_NAME " DMFlag Editor");
	QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);
	QVBoxLayout *box = new QVBoxLayout(this);
	
	QHBoxLayout *hbox = new QHBoxLayout();
	QVBoxLayout *lbox = new QVBoxLayout();
	QVBoxLayout *rbox = new QVBoxLayout();
	
	dmflag = new ZDLDMFlagManager(this);
	dmflag2 = new ZDLDMFlagManager(this);
	
	QGroupBox *gDMFlagsL = new QGroupBox("DMFLAGS",this);
	QGroupBox *gDMFlagsR = new QGroupBox("DMFLAGS (Cont.)",this);
	QGroupBox *gDMFlags2 = new QGroupBox("DMFLAGS2",this);
	
	txtDMFlag = new QLineEdit(this);
	txtDMFlag2 = new QLineEdit(this);
	txtDMFlag->setInputMask("900000000");
	txtDMFlag2->setInputMask("900000000");
	
	QVector<ZDLDMFlagCheckbox*> boxes;
	QVBoxLayout *gb1 = new QVBoxLayout();
	gDMFlagsL->setLayout(gb1);
	QVBoxLayout *gb2 = new QVBoxLayout();
	gDMFlagsR->setLayout(gb2);
	QVBoxLayout *gb3 = new QVBoxLayout();
	gDMFlags2->setLayout(gb3);
	
	int last = 0;
	
	boxes.append(new ZDLDMFlagCheckbox(1,0,"Allow Health (DM)",this));
	boxes.append(new ZDLDMFlagCheckbox(2,0,"Allow Powerups (DM)",this));
	boxes.append(new ZDLDMFlagCheckbox(4,1,"Weapons Stay (DM)",this));
	boxes.append(new ZDLDMFlagCheckbox(8,1,"Falling Damage (OLD)",this));
	boxes.append(new ZDLDMFlagCheckbox(16,1,"Falling Damage (HEXEN)",this));
	boxes.append(new ZDLDMFlagCheckbox(64,1,"Same Map (DM)",this));
	boxes.append(new ZDLDMFlagCheckbox(128,1,"Spawn Farthest (DM)",this));
	boxes.append(new ZDLDMFlagCheckbox(256,1,"Force Respawn (DM)",this));
	boxes.append(new ZDLDMFlagCheckbox(512,0,"Allow Armour (DM)",this));
	boxes.append(new ZDLDMFlagCheckbox(1024,0,"Allow Exit (DM)",this));
	boxes.append(new ZDLDMFlagCheckbox(2048,1,"Infinite Ammo",this));
	boxes.append(new ZDLDMFlagCheckbox(4096,1,"No Monsters",this));
	boxes.append(new ZDLDMFlagCheckbox(8192,1,"Monsters Respawn",this));
	boxes.append(new ZDLDMFlagCheckbox(16384,1,"Items Respawn",this));
	boxes.append(new ZDLDMFlagCheckbox(32768,1,"Fast Monsters",this));
	boxes.append(new ZDLDMFlagCheckbox(65536,0,"Allow Jump",this));
	boxes.append(new ZDLDMFlagCheckbox(131072,0,"Allow Freelook",this));
	for(int i = 0; i < boxes.size(); i++){
		gb1->addWidget(boxes[i]);
		dmflag->addCheckbox(boxes[i]);
	}
	
	last = boxes.size();
	boxes.append(new ZDLDMFlagCheckbox(262144,1,"Mega Powerups Respawn",this));
	boxes.append(new ZDLDMFlagCheckbox(524288,0,"Allow FOV",this));
	boxes.append(new ZDLDMFlagCheckbox(1048576,0,"Spawn Multi Weaons (Co-op)",this));
	boxes.append(new ZDLDMFlagCheckbox(2097152,0,"Allow Crouch",this));
	boxes.append(new ZDLDMFlagCheckbox(4194304,1,"Lose Entire Inventory (Co-op)",this));
	boxes.append(new ZDLDMFlagCheckbox(8388608,0,"Keep Keys (Co-op)",this));
	boxes.append(new ZDLDMFlagCheckbox(16777216,0,"Keep Weapons (Co-op)",this));
	boxes.append(new ZDLDMFlagCheckbox(33554432,0,"Keep Armour (Co-op)",this));
	boxes.append(new ZDLDMFlagCheckbox(67108864,0,"Keep Powerups (Co-op)",this));
	boxes.append(new ZDLDMFlagCheckbox(134217728,0,"Keep Ammo (Co-op)",this));
	boxes.append(new ZDLDMFlagCheckbox(268435456,1,"Lose Half Ammo",this));
	for(int i = last; i < boxes.size(); i++){
		gb2->addWidget(boxes[i]);
		dmflag->addCheckbox(boxes[i]);
	}
	
	last = boxes.size();
	boxes.append(new ZDLDMFlagCheckbox(2,1,"Drop Weapon",this));
	boxes.append(new ZDLDMFlagCheckbox(32768,0,"Allow BFG Aiming",this));
	boxes.append(new ZDLDMFlagCheckbox(65536,1,"Barrels Respawn (DM)",this));
	boxes.append(new ZDLDMFlagCheckbox(131072,1,"Respawn Protection",this));
	for(int i = last; i < boxes.size(); i++){
		gb3->addWidget(boxes[i]);
		dmflag2->addCheckbox(boxes[i]);
	}

	lbox->addWidget(gDMFlagsL);
	rbox->addWidget(gDMFlagsR);
	rbox->addWidget(gDMFlags2);
	
	hbox->addLayout(lbox);
	hbox->addLayout(rbox);
	lbox->addWidget(new QLabel("DMFLAGS:",this));
	lbox->addWidget(txtDMFlag);
	rbox->addWidget(new QLabel("DMFLAGS2:",this));
	rbox->addWidget(txtDMFlag2);
	box->addLayout(hbox);
	box->addWidget(btnBox);
	
	connect(btnBox, SIGNAL(accepted()), this, SLOT(save()));
	connect(btnBox, SIGNAL(rejected()), this, SLOT(close()));
	connect(dmflag, SIGNAL(valueChanged(int)),this,SLOT(dmflagChanged(int)));
	connect(dmflag2, SIGNAL(valueChanged(int)),this,SLOT(dmflag2Changed(int)));
	
	dmflag->forceRecalc();
	dmflag2->forceRecalc();
}

void ZDMFlagDialog::setValue(int val){
	dmflag->setValue(val);
}

void ZDMFlagDialog::setValue2(int val){
	dmflag2->setValue(val);
}

int ZDMFlagDialog::value(){
	bool ok;
	return txtDMFlag->text().toInt(&ok, 10);
}

int ZDMFlagDialog::value2(){
	bool ok;
	return txtDMFlag2->text().toInt(&ok, 10);
}

void ZDMFlagDialog::dmflagChanged(int value){
	QString txt = QString::number(value);
	txtDMFlag->setText(txt);
}

void ZDMFlagDialog::dmflag2Changed(int value){
	QString txt = QString::number(value);
	txtDMFlag2->setText(txt);
}

void ZDMFlagDialog::save(){
	done(1);
}

void ZDMFlagDialog::close(){
	done(0);
}

