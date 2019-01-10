/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
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

#include <QtGui>
#include <QApplication>
#include <QComboBox>
#include "ZDLMapFile.h"

#include "ZDLConfigurationManager.h"
#include "ZDLSettingsPane.h"

void AlwaysFocusedDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const 
{
	QStyleOptionViewItem new_option(option);
	if (new_option.state&QStyle::State_Selected) new_option.state=new_option.state|QStyle::State_Active;
	QItemDelegate::paint(painter, new_option, index);
}

ZDLSettingsPane::ZDLSettingsPane(QWidget *parent):ZDLWidget(parent){
	LOGDATAO() << "New ZDLSettingsPane" << endl;
	QVBoxLayout *box = new QVBoxLayout(this);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(2);

	box->addWidget(new QLabel("Source port",this));

	sourceList = new QComboBox(this);


	box->addWidget(sourceList);

	box->addWidget(new QLabel("IWAD",this));

	IWADList = new QListWidget(this);
	IWADList->setItemDelegate(new AlwaysFocusedDelegate());
	box->addWidget(IWADList);

	QHBoxLayout *box2 = new QHBoxLayout();
	box->addLayout(box2);

	QVBoxLayout *warpBox = new QVBoxLayout();
	box2->addLayout(warpBox);

	QVBoxLayout *skillBox = new QVBoxLayout();
	box2->addLayout(skillBox);

	warpCombo = new VerboseComboBox(this);
	connect(warpCombo, SIGNAL(activated(int)), this, SLOT(currentRowChanged(int)));
	connect(warpCombo, SIGNAL(onPopup()), this, SLOT(VerbosePopup()));
	connect(warpCombo, SIGNAL(onHidePopup()), this, SLOT(HidePopup()));
	warpCombo->setInsertPolicy(QComboBox::NoInsert);
	warpCombo->setEditable(true);
	warpCombo->setValidator(new EvilValidator(this));
	warpCombo->setCompleter(NULL);
	warpCombo->lineEdit()->setPlaceholderText("(Default)");
	warpBox->addWidget(new QLabel("Map",this));
	warpBox->addWidget(warpCombo);
	warpBox->setSpacing(2);

	diffList = new QComboBox(this);
	diffList->addItem("(Default)");
	diffList->addItem("V. Easy");
	diffList->addItem("Easy");
	diffList->addItem("Medium");
	diffList->addItem("Hard");
	diffList->addItem("V. Hard");
	skillBox->addWidget(new QLabel("Skill",this));
	skillBox->addWidget(diffList);
	LOGDATAO() << "Done" << endl;
}

void ZDLSettingsPane::VerbosePopup()
{
	warpCombo->lineEdit()->setPlaceholderText("");
	QString current=warpCombo->currentText();
	int idx;
	emit buildParent(this);
	warpCombo->setUpdatesEnabled(false);
	reloadMapList();
	if (current.isEmpty()) {
		warpCombo->setCurrentIndex(0);
		warpCombo->clearEditText();
	} else if ((idx=warpCombo->findText(current, Qt::MatchFixedString))>0) {
		warpCombo->setCurrentIndex(idx);
	} else {
		warpCombo->setEditText(current);
	}
	warpCombo->setUpdatesEnabled(true);
}

void ZDLSettingsPane::HidePopup()
{
	warpCombo->lineEdit()->setPlaceholderText("(Default)");
}


void ZDLSettingsPane::currentRowChanged(int idx){
	if (!idx) {
		warpCombo->setCurrentIndex(-1);
	}
}

QStringList ZDLSettingsPane::getFilesMaps(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(!zconf){
		return QStringList();
	}
	ZDLSection *section = zconf->getSection("zdl.save");
	if(!section){
		return QStringList();
	}
	QVector<ZDLLine*> vctr;
	section->getRegex("^file[0-9]+$", vctr);
	if(vctr.size() <= 0){
		return QStringList();
	}
	QStringList maps;
	for(int i = vctr.size()-1; i >= 0; i--){
		LOGDATAO() << "Getting maps for " << vctr[i]->getValue() << endl;
		ZDLMapFile *mapfile = ZDLMapFile::getMapFile(vctr[i]->getValue());
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

bool ZDLSettingsPane::naturalSortLess(const QString &left, const QString &right)
{
	//Less comparison algorithm for natural sorting
	//Based on "The Alphanum Algorithm" by David Koelle
	//http://www.davekoelle.com/alphanum.html
	//Released under MIT License (https://opensource.org/licenses/MIT)

    bool mode_letter=true;	//If it's not letter mode, then it's digit mode
	QString::const_iterator li=left.begin();
	QString::const_iterator ri=right.begin();
	bool l_is_digit, r_is_digit;
	unsigned int l_as_uint, r_as_uint, l_digits, r_digits;

	while (li!=left.end()&&ri!=right.end()) {
		if (mode_letter) {
			while (li!=left.end()&&ri!=right.end()) {
				//Check if these are digit characters
				l_is_digit=li->isDigit();
				r_is_digit=ri->isDigit();
				
				//If both characters are digits, we continue in digit mode
				if (l_is_digit&&r_is_digit) {
					mode_letter=false;
					break;
				}
				
				//If one of the characters is a digit, we have a result
				if (l_is_digit) return true;
				if (r_is_digit) return false;

				//Else, compare both characters and if they differ we have a result
				if (*li<*ri) return true;
				if (*li>*ri) return false;

				//Otherwise, process next characters
				++li;
				++ri;
			}
		} else {
			//Get left and right numbers
			//To prevent overflow we process maximum of 9 digits (ignoring leading zeroes)
			//It's ok for WAD map names because they are limited to 8 characters anyway

			l_as_uint=0;
			l_digits=0;
			while (li!=left.end()&&li->isDigit()&&l_digits<9) {
				l_as_uint=l_as_uint*10+li->digitValue();
				if (l_as_uint) l_digits++;
				++li;
			}

			r_as_uint=0;
			r_digits=0;
			while (ri!=right.end()&&ri->isDigit()&&r_digits<9) {
				r_as_uint=r_as_uint*10+ri->digitValue();
				if (r_as_uint) r_digits++;
				++ri;
			}

			//If numbers differ, we have a comparison result
			if (l_as_uint<r_as_uint) return true;
			if (l_as_uint>r_as_uint) return false;

			//Otherwise we process the next substring in letter mode
			mode_letter=true;
		}
	}

	//We got here, so one of the strings (or both) is out of characters
	//If right string still has some characters left, then left string is out of characters, so it is "less" than right
	if (ri!=right.end()) return true;

	//Otherwise right is "less" then left or "equal" to it
	return false;
}

void ZDLSettingsPane::reloadMapList(){
	LOGDATAO() << "reloadMapList START" << endl;
	
	warpCombo->clear();
	warpCombo->addItem("(Default)");

	if(QListWidgetItem *item=IWADList->currentItem()) {
		QString file=item->data(32).toString();
		QFileInfo fi(file);

		QRegExp exts("\\.(zip|wad|iwad|pk3|ipk3|pkz)$");
		exts.setCaseSensitivity(Qt::CaseInsensitive);
		if(fi.exists()&&exts.indexIn(file)>=0) {
			LOGDATAO()<<"Getting iwad maps from "<<file<<endl;
			QStringList wadMaps;
			ZDLMapFile *mapfile=ZDLMapFile::getMapFile(file);

			if(mapfile) {
				if(mapfile->open()) {
					wadMaps=mapfile->getMapNames();
					LOGDATAO()<<"Maps: "<<wadMaps<<endl;
				} else {
					LOGDATAO()<<"Failed to open file"<<endl;
				}
				delete mapfile;
			}else{
				LOGDATAO()<<"Failed to read map names"<<endl;
			}

			LOGDATAO()<<"Getting files maps"<<endl;
			wadMaps.append(getFilesMaps());

			if (wadMaps.size()) {
				qSort(wadMaps.begin(), wadMaps.end(), naturalSortLess);
				wadMaps.removeDuplicates();
				warpCombo->addItems(wadMaps);
			}

			LOGDATAO()<<"reloadMapList END"<<endl;
		}else{
			LOGDATAO()<<"File doesn't exist - "<<file<<endl;
		}
	}

	warpCombo->setCurrentIndex(-1);
}

void ZDLSettingsPane::rebuild(){
	LOGDATAO() << "Saving config" << endl;
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(diffList->currentIndex() > 0){
		zconf->setValue("zdl.save", "skill", diffList->currentIndex());
	}else{
		zconf->deleteValue("zdl.save", "skill");
	}

	if(!warpCombo->currentText().isEmpty()){
		zconf->setValue("zdl.save", "warp", warpCombo->currentText());
	}else{
		zconf->deleteValue("zdl.save", "warp");
	}

	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		int count = 0;
		QVector<ZDLLine*> fileVctr;
		section->getRegex(QString("^p[0-9]+f$"), fileVctr);

		for(int i = 0; i < fileVctr.size(); i++){
			QString value = fileVctr[i]->getVariable();

			QString number = "^p";
			number.append(value.mid(1, value.length()-2));
			number.append("n$");

			QVector<ZDLLine*> nameVctr;
			section->getRegex(number, nameVctr);
			if (nameVctr.size() == 1){
				if (sourceList->currentIndex() == count){
					zconf->setValue("zdl.save", "port", nameVctr[0]->getValue());
					break;
				}
				count++;
			}
		}
	}

	section = zconf->getSection("zdl.iwads");
	if (section){
		int count = 0;
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^i[0-9]+f$", fileVctr);

		for(int i = 0; i < fileVctr.size(); i++){
			QString value = fileVctr[i]->getVariable();

			QString number = "^i";
			number.append(value.mid(1, value.length()-2));
			number.append("n$");

			QVector <ZDLLine*> nameVctr;
			section->getRegex(number, nameVctr);
			if (nameVctr.size() == 1){
				if (IWADList->currentRow() == count){
					zconf->setValue("zdl.save", "iwad", nameVctr[0]->getValue());

					break;
				}
				count++;
			}
		}
	}

}

void ZDLSettingsPane::newConfig(){
	LOGDATAO() << "Loading new config" << endl;
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(zconf->hasValue("zdl.save", "skill")){
		int index = 0;
		int stat = 0;
		QString rc = zconf->getValue("zdl.save", "skill", &stat);
		if (rc.length() > 0){
			index = atoi((char*)rc.toStdString().c_str());
		}
		if (index >= 0 && index <= 5){
			diffList->setCurrentIndex(index);
		}else{
			zconf->setValue("zdl.save", "skill", 0);
			diffList->setCurrentIndex(0);
		}

	}else{
		diffList->setCurrentIndex(0);
	}

	LOGDATAO() << "Reloading map list" << endl;
	reloadMapList();

	{
		LOGDATAO()<<"Loading map selection"<<endl;
		int idx=0;
		QString rc=zconf->getValue("zdl.save", "warp", &idx);

		if ((idx=warpCombo->findText(rc, Qt::MatchFixedString))!=-1) {
			LOGDATAO()<<"Setting to (idx) "<<idx<<endl;
			warpCombo->setCurrentIndex(idx);
		} else {
			LOGDATAO()<<"Setting to (text) "<<rc<<endl;
			warpCombo->setEditText(rc);
		}
	}


	sourceList->clear();
	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^p[0-9]+f$", fileVctr);

		for(int i = 0; i < fileVctr.size(); i++){
			QString value = fileVctr[i]->getVariable();

			QString number = "^p";
			number.append(value.mid(1, value.length()-2));
			number.append("n$");
			int stat = 0;
			QVector<ZDLLine*> nameVctr;
			section->getRegex(number, nameVctr);
			if (nameVctr.size() == 1){
				sourceList->addItem(nameVctr[0]->getValue(),stat);
			}
		}
	}

	if(zconf->hasValue("zdl.save", "port")){
		int set = 0;
		int stat = 0;
		QString rc = zconf->getValue("zdl.save", "port", &stat);

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
	}else{
		//cout << "Don't have port" << endl;
	}

	IWADList->clear();
	section = zconf->getSection("zdl.iwads");
	if (section){
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^i[0-9]+f$", fileVctr);

		for(int i = 0; i < fileVctr.size(); i++){
			QString value = fileVctr[i]->getVariable();

			QString number = "^i";
			number.append(value.mid(1, value.length()-2));
			number.append("n$");

			QVector<ZDLLine*> nameVctr;
			section->getRegex(number, nameVctr);
			if (nameVctr.size() == 1){
				QListWidgetItem *item = new QListWidgetItem(nameVctr[0]->getValue(),IWADList, 1001);
				item->setData(32,fileVctr[i]->getValue());
				IWADList->addItem(item);
			}
		}
	}

	if(zconf->hasValue("zdl.save", "iwad")){
		int set = 0;
		int stat = 0;
		QString rc = zconf->getValue("zdl.save", "iwad", &stat);
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
				zconf->deleteValue("zdl.save", "iwad");
			}else{
				IWADList->setCurrentRow(0);
				QListWidgetItem *item = IWADList->item(0);
				zconf->setValue("zdl.save","iwad",item->text());
			}
		}
	}

}
