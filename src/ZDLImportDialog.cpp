#include "ZDLImportDialog.hpp"
#include "zdlcommon.h"

ZDLImportDialog::ZDLImportDialog(QWidget *parent):QDialog(parent){
	QVBoxLayout *layout = new QVBoxLayout(this);
	this->setLayout(layout);
	setWindowTitle("Configuration Import");

	QString preamble = ZDL_ENGINE_NAME;
	preamble += " has detected you have no user-level configuration!";
	preamble += "We encourage you to use the user-level configuration so";
	preamble += "that no matter where you are on your system, you will be";
	preamble += "able to access your ZDL configuration.  You don't have to,";
	preamble += "but we detected you haven't been asked this before, so we're";
	preamble += "asking this one time question. You have a few options:\n\n";
	preamble += "- Import the current .INI configuration as the new global\n";
	preamble += "- Do not import the current .INI and ask again for another\n";
	preamble += "- Never import and never ask this question again\n";
	preamble += "- Ask again later, do nothing now";
	QLabel *walloftext = new QLabel(preamble, this);
	walloftext->setWordWrap(true);	
	layout->addWidget(walloftext);
	QDialogButtonBox *btnGrp = new QDialogButtonBox(this);
	QPushButton *importNow = new QPushButton("Import Now", this);
	QPushButton *notThis = new QPushButton("Don't import this", this);
	QPushButton *noImport = new QPushButton("Never import", this);
	QPushButton *askLater = new QPushButton("Ask later", this);
	connect(importNow, SIGNAL(clicked()), this, SLOT(importNowClicked()));
	connect(notThis, SIGNAL(clicked()), this, SLOT(doNotImportThisClicked()));
	connect(noImport, SIGNAL(clicked()), this, SLOT(neverImportClicked()));
	connect(askLater, SIGNAL(clicked()), this, SLOT(askLaterClicked()));
	askLater->setDefault(true);
	askLater->setAutoDefault(true);
	importNow->setAutoDefault(false);
	importNow->setDefault(false);
	notThis->setAutoDefault(false);
	notThis->setDefault(false);
	noImport->setAutoDefault(false);
	noImport->setDefault(false);
	btnGrp->addButton(importNow, QDialogButtonBox::YesRole);
	btnGrp->addButton(notThis, QDialogButtonBox::ActionRole);
	btnGrp->addButton(noImport, QDialogButtonBox::ActionRole);
	btnGrp->addButton(askLater, QDialogButtonBox::NoRole);
	layout->addWidget(btnGrp);
	askLater->setDefault(true);
	askLater->setFocus(Qt::PopupFocusReason);
	importAction = UNKNOWN;
}

void ZDLImportDialog::askLaterClicked(){
	importAction = ASKLATER;
	reject();
}

void ZDLImportDialog::importNowClicked(){
	importAction = IMPORTNOW;
	accept();
}

void ZDLImportDialog::doNotImportThisClicked(){
	importAction = DONOTIMPORTTHIS;
	accept();
}

void ZDLImportDialog::neverImportClicked(){
	importAction = NEVERIMPORT;
	accept();
}

