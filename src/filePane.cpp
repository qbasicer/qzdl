#include <QtGui>
#include <QApplication>
#include <QListWidget>
#include <iostream>

#include "filePane.h"

filePane::filePane(ZQWidget *parent): ZQWidget(parent){
	QVBoxLayout *box = new QVBoxLayout(this);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(2);

	QListWidget *fileList = new QListWidget(this);
	box->addWidget(new QLabel("External Files",this));
	box->addWidget(fileList);

	int minBtnWidth = 20;

	QHBoxLayout *buttonBox = new QHBoxLayout();
	QPushButton *btnAdd = new QPushButton("Add", this);
	btnAdd->setMinimumWidth(minBtnWidth);
	QPushButton *btnRem = new QPushButton("Rem", this);
	btnRem->setMinimumWidth(minBtnWidth);
	QPushButton *btnUp = new QPushButton("/\\", this);
	btnUp->setMinimumWidth(10);
	QPushButton *btnDn = new QPushButton("\\/", this);
	btnDn->setMinimumWidth(10);
	box->addLayout(buttonBox);
	buttonBox->addWidget(btnAdd);
	buttonBox->addWidget(btnRem);
	buttonBox->addWidget(btnUp);
	buttonBox->addWidget(btnDn);

}

void filePane::rebuild(){
	std::cout << "Rebuilding config" << std::endl;
}
