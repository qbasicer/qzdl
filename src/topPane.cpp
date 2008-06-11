#include <QtGui>
#include <QApplication>
#include <QMainWindow>

#include "topPane.h"
#include "filePane.h"
#include "settingPane.h"
#include "ZQSplitter.h"

topPane::topPane(ZQWidget *parent): ZQWidget(parent){
	QHBoxLayout *box = new QHBoxLayout(this);

	ZQSplitter *split = new ZQSplitter(this);
	QSplitter *rsplit = split->getSplit();
	
	
	filePane *fpane = new filePane(rsplit);
	settingPane *spane = new settingPane(rsplit);
	
	split->addChild(fpane);
	split->addChild(spane);


	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(2);
	box->addWidget(rsplit);
}
