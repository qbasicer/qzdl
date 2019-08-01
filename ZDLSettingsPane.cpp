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
#include <QComboBox>
#include "ZDLMapFile.h"

#include "zdlconf.h"
#include "ZDLSettingsPane.h"

ZDLSettingsPane::ZDLSettingsPane(QWidget *parent):ZDLWidget(parent){
	QVBoxLayout *box = new QVBoxLayout(this);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(2);

	box->addWidget(new QLabel("Source Port",this));

	sourceList = new QComboBox(this);


	box->addWidget(sourceList);

	box->addWidget(new QLabel("IWAD",this));

	IWADList = new QListWidget(this);
	box->addWidget(IWADList);

	QHBoxLayout *box2 = new QHBoxLayout();
	box->addLayout(box2);

	QVBoxLayout *warpBox = new QVBoxLayout();
	box2->addLayout(warpBox);

	QVBoxLayout *skillBox = new QVBoxLayout();
	box2->addLayout(skillBox);

	warpCombo = new QComboBox(this);
	warpCombo->setEditable(true);
	warpBox->addWidget(new QLabel("Map",this));
	warpBox->addWidget(warpCombo);

	diffList = new QComboBox(this);
	skillBox->addWidget(new QLabel("Skill",this));
	skillBox->addWidget(diffList);
	diffList->addItem("None");
	diffList->addItem("V. Easy");
	diffList->addItem("Easy");
	diffList->addItem("Medium");
	diffList->addItem("Hard");
	diffList->addItem("V. Hard");
	connect(IWADList, &QListWidget::currentRowChanged, this, &ZDLSettingsPane::currentRowChanged);

}

void ZDLSettingsPane::currentRowChanged(int idx){
	Q_UNUSED(idx);
	// Don't overwrite the user's text
	QString current;
	if(warpCombo->currentText().length() > 0){
		current = warpCombo->currentText();
	}
	reloadMapList();
	if(!current.isNull()){
		QLineEdit *edit = warpCombo->lineEdit();
		if(edit){
			edit->setText(current);
		}

	}

}

QStringList ZDLSettingsPane::getFilesMaps(){
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(!zconf){
		return QStringList();
	}
	QStringList maps;
	for (int i = 0; ; i++){
		auto key = QString("zdl.save/file%1").arg(i);
		if (!zconf->contains(key)){
			break;
		}
		ZDLMapFile *mapfile = ZDLMapFile::getMapFile(zconf->value(key).toString());
		if(!mapfile){
			continue;
		}
		if(!mapfile->open()){
			delete mapfile;
			continue;
		}
		maps += mapfile->getMapNames();
		delete mapfile;
	}
	return maps;
}

void ZDLSettingsPane::reloadMapList(){
	QStringList iwadMaps;
	QListWidgetItem *item = IWADList->currentItem();
	if(!item){
		return;
	}
	QVariant data = item->data(32);
	if(!data.isValid() && !data.isNull()){
		// Item data is NULL
		return;
	}
	QString file = data.toString();
	if(file.isNull() || file.isEmpty()){
		// Bad string
		return;
	}
	QFileInfo fi(file);
	if(fi.exists() && file.endsWith(".wad", Qt::CaseInsensitive)){
		//Getting iwad maps from file
		ZDLMapFile *mapfile = ZDLMapFile::getMapFile(file);
		if(mapfile){
			if(mapfile->open()){
				iwadMaps = mapfile->getMapNames();
			}
			delete mapfile;
		}
	}
	// Getting files maps
	QStringList filesMaps = getFilesMaps();
	if(filesMaps.size() + iwadMaps.size() > 0){
		warpCombo->clear();
		warpCombo->addItem("");
		warpCombo->addItems(filesMaps);
		warpCombo->addItems(iwadMaps);
	}
}

void ZDLSettingsPane::rebuild(){
	// Saving config
	auto zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(diffList->currentIndex() > 0){
		zconf->setValue("zdl.save/skill", QString::number(diffList->currentIndex()));
	}else{
		zconf->remove("zdl.save/skill");
	}

	if(warpCombo->currentText().length() > 0){
		zconf->setValue("zdl.save/warp", warpCombo->currentText());
	}else{
		zconf->remove("zdl.save/warp");
	}

	for (int i = 0; ; i++)
	{
		auto key = QString("zdl.ports/p%1n").arg(i);
		if (!zconf->contains(key))
		{
			break;
		}

		auto port = zconf->value(key).toString();
		if (sourceList->currentText() == port)
		{
			zconf->setValue("zdl.save/port", port);
		}
	}

	for (int i = 0; ; i++)
	{
		auto key = QString("zdl.iwads/i%1n").arg(i);
		if (!zconf->contains(key))
		{
			break;
		}

		auto iwad = zconf->value(key).toString();
		if (IWADList->currentItem() && IWADList->currentItem()->text() == iwad)
		{
			zconf->setValue("zdl.save/iwad", iwad);
		}
	}

}

void ZDLSettingsPane::newConfig(){
	QSettings *zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(zconf->contains("zdl.save/skill")){

		int index = 0;
		QString rc = zconf->value("zdl.save/skill").toString();
		if (rc.length() > 0){
			index = atoi((char*)rc.toStdString().c_str());
		}
		if (index >= 0 && index <= 5){
			diffList->setCurrentIndex(index);
		}else{
			zconf->setValue("zdl.save/skill", "0");
			diffList->setCurrentIndex(0);
		}

	}else{
		diffList->setCurrentIndex(0);
	}
	// Reloading map list
	reloadMapList();
	// Loading map selection
	if(zconf->contains("zdl.save/warp")){
		QString rc = zconf->value("zdl.save/warp").toString();
		if(rc.length() > 0){
			QLineEdit *edit = warpCombo->lineEdit();
			if(edit){
				edit->setText(rc);
			}
		}else{
			// Length is zero or less
			QLineEdit *edit = warpCombo->lineEdit();
			if(edit){
				edit->setText("");
			}
		}
	}else{
		// No map selection
		QLineEdit *edit = warpCombo->lineEdit();
		if(edit){
			edit->setText("");
		}
	}


	sourceList->clear();

	for (int i = 0; ; i++){
		auto key = QString("zdl.ports/p%1n").arg(i);
		if (!zconf->contains(key)){
			break;
		}
		sourceList->addItem(zconf->value(key).toString());
	}

	if(zconf->contains("zdl.save/port")){
		int set = 0;
		int stat = 0;
		QString rc = zconf->value("zdl.save/port").toString();

		if(rc.length() > 0){
			for(int i = 0; i < sourceList->count(); i++){
				if(sourceList->itemText(i).compare(rc) == 0){
					sourceList->setCurrentIndex(i);
					set = 1;
					break;
				}
			}
			if(set == 0){
				sourceList->setCurrentIndex(0);
			}
		}
	}

	IWADList->clear();
	for (int i = 0; ; i++){
		auto nameKey = QString("zdl.iwads/i%1n").arg(i);
		auto fileKey = QString("zdl.iwads/i%1f").arg(i);
		if (!zconf->contains(fileKey) || !zconf->contains(nameKey)){
			break;
		}
		QListWidgetItem *item = new QListWidgetItem(zconf->value(nameKey).toString(),IWADList, 1001);
		item->setData(32,zconf->value(fileKey).toString());
		IWADList->addItem(item);
	}

	if(zconf->contains("zdl.save/iwad")){
		int set = 0;
		QString rc = zconf->value("zdl.save/iwad").toString();
		if (rc.length() > 0){
			for(int i = 0; i < IWADList->count(); i++){
				QListWidgetItem *item = IWADList->item(i);
				QString text = item->text();
				if(text.compare(rc) == 0){
					set = 1;
					IWADList->setCurrentRow(i);
					break;
				}
			}
		}
		if(!set){
			if(IWADList->count() == 0){
				zconf->remove("zdl.save/iwad");
			}else{
				IWADList->setCurrentRow(0);
				QListWidgetItem *item = IWADList->item(0);
				zconf->setValue("zdl.save/iwad",item->text());
			}
		}
	}

}
