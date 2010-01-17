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

#include "configurationManager.h"
#include "zListWidget.h"
#include "adown.xpm"
#include "aup.xpm"

zListWidget::zListWidget(ZQWidget *parent): ZQWidget(parent){
	QVBoxLayout *column = new QVBoxLayout(this);
	pList = new QListWidget(this);
	pList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	
	QHBoxLayout *buttonRow = new QHBoxLayout();
	
	btnAdd = new QPushButton("Add", this);
	
	QAction* delact = new QAction(this);
	delact->setShortcut(Qt::Key_Delete);
	delact->setShortcutContext(Qt::WidgetShortcut);
	connect(delact, SIGNAL(triggered()), this, SLOT(removeButton()));
	
	QAction* insact = new QAction(this);
	insact->setShortcut(Qt::Key_Insert);
	insact->setShortcutContext(Qt::WidgetShortcut);
	connect(insact, SIGNAL(triggered()), this, SLOT(addButton()));
	
	pList->addAction(delact);
	pList->addAction(insact);
	
	btnAdd->setMinimumWidth(30);
	
	btnRem = new QPushButton("Rem", this);
	btnRem->setMinimumWidth(30);
	
	btnUp = new QPushButton(this);
	btnUp->setIcon(QPixmap(aup));
	btnUp->setMinimumWidth(10);
	
	btnDn = new QPushButton(this);
	btnDn->setIcon(QPixmap(adown));
	btnDn->setMinimumWidth(10);
	
	buttonRow->addWidget(btnAdd);
	buttonRow->addWidget(btnRem);
	buttonRow->addWidget(btnUp);
	buttonRow->addWidget(btnDn);
	
	//Glue it together
	column->addWidget(pList);
	column->addLayout(buttonRow);
	
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);

	//signal time
	QObject::connect(btnAdd, SIGNAL(clicked()), this, SLOT(addButton()));
	QObject::connect(btnRem, SIGNAL(clicked()), this, SLOT(removeButton()));
	QObject::connect(btnUp, SIGNAL(clicked()), this, SLOT(upButton()));
	QObject::connect(btnDn, SIGNAL(clicked()), this, SLOT(downButton()));
	QObject::connect(pList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editButton(QListWidgetItem*)));
	
}

void zListWidget::insert(ZListable *item, int index){
	if(index < 0){
		pList->addItem(item);
	}else{
		pList->insertItem(index, item);
	}
}
int zListWidget::count(){
	return pList->count();
}
void zListWidget::remove(int index){
	QListWidgetItem *item = pList->takeItem(index);
	if (item){
		delete item;
	}else{
		QMessageBox::warning(this, "ZDLSharp Error", "You didn't make a selection.");
	}
}
ZListable* zListWidget::get(int index){
	if (index >= 0 && index < (int)list.size()){
		return list[index];
	}
	return NULL;
}

void zListWidget::addButton(){

}

void zListWidget::removeButton(){
	QList<QListWidgetItem *> slist = pList->selectedItems();
	for(int i = 0; i < slist.size(); i++){
		int rowid = pList->row(slist[i]);
		remove(rowid);
	}
}

void zListWidget::upButton(){
	if (pList->selectedItems().size() == 1){
		if (pList->currentRow () > 0){
			int oldRow = pList->currentRow ();
			QListWidgetItem *item = pList->takeItem(oldRow);
			insert((ZListable*)item, oldRow-1);
			pList->setCurrentRow(oldRow-1);
		}
	}
}


void zListWidget::downButton(){
	if (pList->selectedItems().size() == 1){
		if (pList->currentRow () < pList->count()-1){
			int oldRow = pList->currentRow ();
			QListWidgetItem *item = pList->takeItem(oldRow);
			insert((ZListable*)item, oldRow+1);
			pList->setCurrentRow(oldRow+1);
		}
	}
}

void zListWidget::editButton(QListWidgetItem * item){
	item = item;
}


