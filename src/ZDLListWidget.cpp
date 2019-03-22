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

#include "ZDLConfigurationManager.h"
#include "ZDLListWidget.h"
#include "gph_upt.xpm"
#include "gph_dna.xpm"
#include "gph_upa.xpm"
#include "gph_pls.xpm"
#include "gph_mns.xpm"

ZDLListWidget::ZDLListWidget(ZDLWidget *parent): ZDLWidget(parent) {
	QVBoxLayout *column = new QVBoxLayout(this);
	pList = new QListWidget(this);
	pList->setSelectionMode(QAbstractItemView::ExtendedSelection);

	buttonRow = new QHBoxLayout();

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

	btnAdd = new QPushButton(this);
	btnAdd->setIcon(QPixmap(glyph_plus));

	btnRem = new QPushButton(this);
	btnRem->setIcon(QPixmap(glyph_minus));

	btnEdt = new QPushButton(this);
	btnEdt->setIcon(QPixmap(glyph_up_trg));

	btnUp = new QPushButton(this);
	btnUp->setIcon(QPixmap(glyph_up_arr));

	btnDn = new QPushButton(this);
	btnDn->setIcon(QPixmap(glyph_down_arr));

    btnAdd->setToolTip("Add items");
	btnRem->setToolTip("Remove selected items");
	btnEdt->setToolTip("Edit selected item");
	btnUp->setToolTip("Move selected items up");
	btnDn->setToolTip("Move selected items down");

	buttonRow->addWidget(btnAdd);
	buttonRow->addWidget(btnRem);
	buttonRow->addWidget(btnEdt);
	buttonRow->addStretch();
	buttonRow->addWidget(btnUp);
	buttonRow->addWidget(btnDn);
	buttonRow->setSpacing(0);
	
	//Glue it together
	column->addWidget(pList);
	column->addLayout(buttonRow);
	column->setSpacing(0);

	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);

	//signal time
	QObject::connect(btnAdd, SIGNAL(clicked()), this, SLOT(addButton()));
	QObject::connect(btnRem, SIGNAL(clicked()), this, SLOT(removeButton()));
	QObject::connect(btnEdt, SIGNAL(clicked()), this, SLOT(editButton()));
	QObject::connect(btnUp, SIGNAL(clicked()), this, SLOT(upButton()));
	QObject::connect(btnDn, SIGNAL(clicked()), this, SLOT(downButton()));
	QObject::connect(pList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editButton(QListWidgetItem*)));
	QObject::connect(pList, SIGNAL(currentRowChanged(int)), this, SLOT(currentRowChangedInternal(int)));
	QObject::connect(pList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(currentItemChangedInternal(QListWidgetItem*, QListWidgetItem*)));
}

void ZDLListWidget::currentItemChangedInternal(QListWidgetItem *current, QListWidgetItem *previous){
	emit currentItemChanged(current, previous);
}

void ZDLListWidget::currentRowChangedInternal(int currentRow){
	emit currentRowChanged(currentRow);
}


void ZDLListWidget::doDragDrop(int enabled){
	setAcceptDrops(enabled);
}

void ZDLListWidget::newDrop(QStringList files){
	Q_UNUSED(files);
}

void ZDLListWidget::dragEnterEvent(QDragEnterEvent *event){
	setBackgroundRole(QPalette::Highlight);
	event->acceptProposedAction();
}

void ZDLListWidget::dragMoveEvent(QDragMoveEvent *event){
	event->acceptProposedAction();
}

void ZDLListWidget::dragLeaveEvent(QDragLeaveEvent *event){
	setBackgroundRole(QPalette::Dark);
	event->accept();
}

void ZDLListWidget::dropEvent(QDropEvent *event){
	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasUrls()) {
		QList<QUrl> urlList(mimeData->urls());
		QStringList files;
		for (int i = 0; i < urlList.size() && i < 32; ++i) {
			QUrl url = urlList.at(i);
			LOGDATAO() << "url " << i << "=" << url.toString() << endl;
			if(url.scheme() == "file"){
				QString path = url.path();
#ifdef Q_WS_WIN
				if(path[2] == ':'){
					path.remove(0,1);
				}
#endif
				QFileInfo urlDecoder(path);
				LOGDATAO() << "Adding path " << urlDecoder.absoluteFilePath() << endl;
				files <<  urlDecoder.absoluteFilePath();
			}
		}

		newDrop(files);
		event->accept();
	}
}

void ZDLListWidget::insert(ZDLListable *item, int index){
	if(index < 0){
		pList->addItem(item);
	}else{
		pList->insertItem(index, item);
	}
}
int ZDLListWidget::count(){
	return pList->count();
}
void ZDLListWidget::remove(int index){
	QListWidgetItem *item = pList->takeItem(index);
	if (item){
		delete item;
	}else{
		QMessageBox::warning(this, "ZDL Error", "You didn't make a selection.");
	}
}
ZDLListable* ZDLListWidget::get(int index){
	if (index >= 0 && index < list.length()){
		return list[index];
	}
	return NULL;
}

void ZDLListWidget::addButton(){

}

void ZDLListWidget::removeButton(){
	QList<QListWidgetItem *> slist = pList->selectedItems();
	int selected = -1;
	if(slist.size() == 1){
		selected = pList->currentRow();
	}
	for(int i = 0; i < slist.size(); i++){
		int rowid = pList->row(slist[i]);
		remove(rowid);
	}
	if(selected != -1){
		if(pList->count() > 0){
			if(pList->count() <= selected){
				pList->setCurrentRow(pList->count() - 1);
			}else{
				pList->setCurrentRow(selected);
			}
		}
	}
}

static void sortItemsByRow(QListWidget *pList, QList<QListWidgetItem*> &items){
	if(items.size() <= 0){
		return;
	}
	QList<QListWidgetItem*> ilist;
	ilist.append(items.takeLast());
	while(items.size() > 0){
		QListWidgetItem* item = items.takeLast();
		int newRow = pList->row(item);
		int firstRow = pList->row(ilist.first());
		int lastRow = pList->row(ilist.last());
		if(newRow < firstRow){
			ilist.push_front(item);
		}else if(newRow > lastRow){
			ilist.push_back(item);
		}else{
			QList<QListWidgetItem*>::iterator i;
			for (i = ilist.begin(); i != ilist.end(); ++i){
				int oldRow = pList->row(*i);
				if(newRow < oldRow){
					i = ilist.insert(i, item);
					break;
				}
			}
		}
	}
	items.append(ilist);
}

void ZDLListWidget::upButton(){
	if (pList->selectedItems().size() == 1){
		if (pList->currentRow () > 0){
			int oldRow = pList->currentRow ();
			QListWidgetItem *item = pList->takeItem(oldRow);
			insert((ZDLListable*)item, oldRow-1);
			pList->setCurrentRow(oldRow-1);
		}
	} else if (pList->selectedItems().size() > 1){
		QList<QListWidgetItem*> items = pList->selectedItems();
		sortItemsByRow(pList, items);
		for(int i = 0; i < items.size(); i++) {
			QListWidgetItem* item = items[i];
			int row = pList->row(item);
			// Make sure we don't move up and out of the list
			if(row <= 0){
				return;
			}
		}
		for(int i = 0; i < items.size(); i++) {
			QListWidgetItem* item = items[i];
			int row = pList->row(item);
			item = pList->takeItem(row);
			pList->insertItem(row - 1, item);
		}
		for(int i = 0; i < items.size(); i++) {
			pList->setCurrentItem(items[i], QItemSelectionModel::Select);
		}
	}
}


void ZDLListWidget::downButton(){
	if (pList->selectedItems().size() == 1){
		if (pList->currentRow () < pList->count()-1){
			int oldRow = pList->currentRow ();
			QListWidgetItem *item = pList->takeItem(oldRow);
			insert((ZDLListable*)item, oldRow+1);
			pList->setCurrentRow(oldRow+1);
		}
	}else if (pList->selectedItems().size() > 1){
                QList<QListWidgetItem*> items = pList->selectedItems();
		// Need this check in here to ensure we don't use an invalid index
		if(items.size() <= 1){
			return;
		}

		sortItemsByRow(pList, items);
		int max = pList->count();
                for(int i = 0; i < items.size(); i++) {
                        QListWidgetItem* item = items[i];
                        int row = pList->row(item);
			// Make sure we don't run off the end
                        if(row >= max - 1){
                                return;
                        }
                }
                for(int i = items.size()-1; i >= 0; i--) {
                        QListWidgetItem* item = items[i];
                        int row = pList->row(item);
			item = pList->takeItem(row);
                        pList->insertItem(row + 1, item);
                }
		for(int i = 0; i < items.size(); i++) {
                        pList->setCurrentItem(items[i], QItemSelectionModel::Select);
                }
        }
}

void ZDLListWidget::editButton(QListWidgetItem * item)
{
	Q_UNUSED(item);
}

void ZDLListWidget::editButton() 
{ 
	editButton(pList->currentItem()); 
}


