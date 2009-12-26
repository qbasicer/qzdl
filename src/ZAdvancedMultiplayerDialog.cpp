#include "ZAdvancedMultiplayerDialog.h"
#include <QDialogButtonBox>

ZAdvancedMultiplayerDialog::ZAdvancedMultiplayerDialog(ZQWidget *parent):QDialog(parent){
	QVBoxLayout *box = new QVBoxLayout(this);
	QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);
	enable = new QCheckBox("Enable Advanced Multiplayer Settings", this);
	
	
	connect(btnBox, SIGNAL(accepted()), this, SLOT(save()));
	connect(btnBox, SIGNAL(rejected()), this, SLOT(close()));
	
	box->addWidget(enable);
	box->addWidget(btnBox);
}

void ZAdvancedMultiplayerDialog::close(){
	done(0);
}

void ZAdvancedMultiplayerDialog::save(){
	done(1);
}
