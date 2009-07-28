#include <QtGui>
#include <QApplication>

#include "multiPane.h"


multiPane::multiPane(ZQWidget *parent): ZQWidget(parent){
	QVBoxLayout *box = new QVBoxLayout(this);

	gMode = new QComboBox(this);
	
	tHostAddy = new QLineEdit(this);
	
	gPlayers = new QComboBox(this);
	
	tFragLimit = new QLineEdit(this);
	
	bDMFlags = new QPushButton("0",this);
	
	bDMFlags2 = new QPushButton("0",this);
	
	QGridLayout *topGrid = new QGridLayout();
	topGrid->addWidget(new QLabel("Game Mode",this),0,0,1,2);
	topGrid->addWidget(gMode,1,0,1,2);
	topGrid->addWidget(new QLabel("Hostname/IP Address",this),0,2,1,2);
	topGrid->addWidget(tHostAddy,1,2,1,2);
	topGrid->addWidget(new QLabel("Players",this),2,0);
	topGrid->addWidget(gPlayers,3,0);
	topGrid->addWidget(new QLabel("Frag Limit",this),2,1);
	topGrid->addWidget(tFragLimit,3,1);
	topGrid->addWidget(new QLabel("DMFLAGS",this),2,2);
	topGrid->addWidget(bDMFlags,3,2);
	topGrid->addWidget(new QLabel("DMFLAGS2",this),2,3);
	topGrid->addWidget(bDMFlags2,3,3);
	box->addLayout(topGrid);
	
	topGrid->setSpacing(0);

	layout()->setContentsMargins(0,0,0,0);

}
