#include <QtGui>
#include <QApplication>

#include "configurationManager.h"
#include "zListWidget.h"

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
	
	btnUp = new QPushButton("/\\", this);
	btnUp->setMinimumWidth(10);
	
	btnDn = new QPushButton("\\/", this);
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
}


