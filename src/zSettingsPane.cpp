#include <QtGui>
#include <QApplication>

#include "configurationManager.h"
#include "zListWidget.h"
#include "zSettingsPane.h"

zSettingsPane::zSettingsPane(QWidget *parent): ZQWidget(parent){
	QVBoxLayout *sections = new QVBoxLayout(this);
	
	QVBoxLayout *iwadl = new QVBoxLayout();
	QVBoxLayout *spl = new QVBoxLayout();
	
	alwaysArgs = new QLineEdit(this);
	
	QHBoxLayout *lrpane = new QHBoxLayout();
	
	//IWAD
	QVBoxLayout *lpane = new QVBoxLayout();
	iwadList = new ZIWadList(this);
	iwadl->addWidget(new QLabel("IWADs:", this));
	iwadl->addWidget(iwadList);
	
	lpane->addLayout(iwadl);
	
	//Source Port
	QVBoxLayout *rpane = new QVBoxLayout();
	sourceList = new ZSPList(this);
	spl->addWidget(new QLabel("Source Ports/Engines:", this));
	spl->addWidget(sourceList);
	
	rpane->addLayout(spl);
	
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
