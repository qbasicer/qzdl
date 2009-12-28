#include "ZTempDMFlagDialog.h"
#include "configurationManager.h"
#include <QDialogButtonBox>

ZTempDMFlagDialog::ZTempDMFlagDialog(ZQWidget *parent):QDialog(parent){
	setWindowTitle("qZDL DMFlag Editor");
	QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);
	QVBoxLayout *box = new QVBoxLayout(this);
	flagVal = new QLineEdit(this);
	box->addWidget(flagVal);
	box->addWidget(btnBox);
	dmflags = 0;
	connect(btnBox, SIGNAL(accepted()), this, SLOT(save()));
	connect(btnBox, SIGNAL(rejected()), this, SLOT(close()));
	flagVal->setFocus();
}

void ZTempDMFlagDialog::setValue(int val){
	dmflags = val;
	flagVal->setText(QString::number(dmflags));
}

int ZTempDMFlagDialog::value(){
	return dmflags;
}

void ZTempDMFlagDialog::save(){
	bool ok;
	dmflags = flagVal->text().toInt(&ok, 10);
	done(1);
}

void ZTempDMFlagDialog::close(){
	done(0);
}

