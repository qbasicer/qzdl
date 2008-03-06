#include <QtGui>
#include <QApplication>
#include <QMainWindow>

#include "multiPane.h"
#include "zdlInterface.h"
#include "bottomPane.h"
#include "topPane.h"

zdlInterface::zdlInterface(QWidget *parent):ZQWidget(parent){
	QVBoxLayout *box = new QVBoxLayout(this);
	topPane *tpane = new topPane(this);
	bottomPane *bpane = new bottomPane(this);
	mpane = new multiPane(this);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(0);
	box->addWidget(tpane);
	box->addWidget(bpane);
	box->addWidget(mpane);
}

void zdlInterface::startRead(){
	emit readChildren(this);
}

void zdlInterface::writeConfig(){
	emit buildChildren(this);
}
