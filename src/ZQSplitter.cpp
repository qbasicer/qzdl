#include <iostream>
#include <QtGui>
#include <QApplication>
#include "ZQSplitter.h"
#include <QMetaObject>


ZQSplitter::ZQSplitter(ZQWidget *parent):ZQWidget(parent){
	box = new QVBoxLayout(this);
	split = new QSplitter(this);
	//std::cout << "SPLIT: Using ZQWidget as parent" << std::endl;
	box->addWidget(split);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
}

ZQSplitter::ZQSplitter(QWidget *parent):ZQWidget(parent){
	box = new QVBoxLayout(this);
	split = new QSplitter(this);
	//std::cout << "SPLIT: Using QWidget as parent" << std::endl;
	box->addWidget(split);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
}

QSplitter *ZQSplitter::getSplit(){
	return split;
}

void ZQSplitter::addChild(ZQWidget *child){
	split->addWidget(child);
	//Hook the child up with our internal signal handling
	child->setZParent(this);
}

void ZQSplitter::addChild(QWidget *child){
	split->addWidget(child);
}
