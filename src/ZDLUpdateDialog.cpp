#include "ZDLUpdateDialog.h"

#include "ZDLConfigurationManager.h"

// Healper
static QLabel *createLabel(QString text, QWidget *parent){
	QLabel *ret = new QLabel(text, parent);
	ret->setWordWrap(true);
	return ret;
}

ZDLUpdateDialog::ZDLUpdateDialog(QWidget *parent):QDialog(parent){
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(createLabel("There's an update available for "ZDL_ENGINE_NAME"!", this));
	layout->addWidget(createLabel("Here's a summary of the updates in the latest version: ", this));
	QPlainTextEdit *edit = new QPlainTextEdit(this);
	edit->setPlainText("Loading...");
	layout->addWidget(edit);
	layout->addWidget(createLabel("You can download the newest version at <a href=\"http://zdl.vectec.net\">zdl.vectec.net</a>", this));

	QDialogButtonBox *btnGrp = new QDialogButtonBox(this);
        QPushButton *btnOk = new QPushButton("Ok", this);
	QPushButton *btnLater = new QPushButton("Ask Later", this);
	btnGrp->addButton(btnOk, QDialogButtonBox::YesRole);
	btnGrp->addButton(btnLater, QDialogButtonBox::NoRole);
        layout->addWidget(btnGrp);
	connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
	connect(btnLater, SIGNAL(clicked()), this, SLOT(remindLater()));	
	setSizeGripEnabled(true);
	setWindowTitle("Update available for "ZDL_ENGINE_NAME);
}

void ZDLUpdateDialog::remindLater(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(zconf){
	        zconf->setValue("zdl.net", "lastchecked", QDateTime::currentDateTime().toString());
		zconf->deleteValue("zdl.net", "hasupdate");
	}
	
	reject();
	
}

