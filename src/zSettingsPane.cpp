#include <QtGui>
#include <QApplication>

#include "configurationManager.h"
#include "zListWidget.h"
#include "zSettingsPane.h"

zSettingsPane::zSettingsPane(QWidget *parent): ZQWidget(parent){
	QVBoxLayout *sections = new QVBoxLayout(this);
	
	alwaysArgs = new QLineEdit(this);
	
	QHBoxLayout *lrpane = new QHBoxLayout();
	
	//IWAD
	QVBoxLayout *lpane = new QVBoxLayout();
	iwadList = new zListWidget(this);
	lpane->addWidget(iwadList);
	
	//Source Port
	QVBoxLayout *rpane = new QVBoxLayout();
	sourceList = new zListWidget(this);
	rpane->addWidget(sourceList);
	
	//Add the left and right panes
	lrpane->addLayout(lpane);
	lrpane->addLayout(rpane);
	
	//Add all the sections together
	sections->addWidget(new QLabel("Always Add These Parameters", this));
	sections->addWidget(alwaysArgs);
	sections->addLayout(lrpane);
	
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
}
