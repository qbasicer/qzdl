#include <QtGui>
#include <QApplication>
#include <QMainWindow>

#include "topPane.h"
#include "filePane.h"
#include "settingPane.h"

topPane::topPane(ZQWidget *parent): ZQWidget(parent){
	QHBoxLayout *box = new QHBoxLayout(this);

	filePane *fpane = new filePane(this);
	settingPane *spane = new settingPane(this);


	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(2);
	box->addWidget(fpane);
	box->addWidget(spane);
}
