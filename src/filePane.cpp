#include <QtGui>
#include <QApplication>
#include <QListWidget>
#include <iostream>

#include "zListWidget.h"
#include "filePane.h"


filePane::filePane(QWidget *parent):ZQWidget(parent){
	QVBoxLayout *box = new QVBoxLayout(this);
	box->setSpacing(2);

	box->addWidget(new QLabel("External Files",this));

	fList = new ZFileList(this);
	box->addWidget(fList);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	
}

void filePane::rebuild(){
	//std::cout << "Rebuilding config" << std::endl;
}
