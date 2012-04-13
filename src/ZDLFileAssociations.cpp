#include "ZDLFileAssociations.h"

ZDLFileAssociations::ZDLFileAssociations(QWidget *parent):QDialog(parent){
	QVBoxLayout *layout = new QVBoxLayout(this);
	chkZDL = new QCheckBox("ZDL Saved configs (*.ZDL)", this);
	chkWAD = new QCheckBox("WAD Files (*.wad)", this);
	chkPK3 = new QCheckBox("PK3 Files (*.pk3)", this);
	chkZIP = new QCheckBox("ZIP Files (*.zip)", this);
	chkPATCH = new QCheckBox("Patch Files (*.deh, *.bex)", this);
	QPushButton *btn = new QPushButton("OK", this);

	layout->addWidget(chkZDL);
	layout->addWidget(chkWAD);
	layout->addWidget(chkPK3);
	layout->addWidget(chkZIP);
	layout->addWidget(chkPATCH);
	layout->addWidget(btn);
	connect(btn, SIGNAL(clicked()), this, SLOT(applyChecks()));
}

void ZDLFileAssociations::applyChecks(){
	accept();
}

