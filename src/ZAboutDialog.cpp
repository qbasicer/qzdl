#include "ZAboutDialog.h"
#include "configurationManager.h"
#include <QDialogButtonBox>
#include "bmp_logo.xpm"

extern QString versionString;

ZAboutDialog::ZAboutDialog(ZQWidget *parent):QDialog(parent){
	setWindowTitle("About qZDL");
	QVBoxLayout *box = new QVBoxLayout(this);
	QHBoxLayout *hbox = new QHBoxLayout();
	box->addLayout(hbox);
	QVBoxLayout *vbox = new QVBoxLayout();
	QLabel *title = new QLabel("ZDL<i>Sharp</i> "+versionString,this);
	
	QFont font;
	font.setPointSize(24);
	title->setFont(font);
	title->setAlignment(Qt::AlignHCenter);
	
	vbox->addWidget(title);
	vbox->addWidget(new QLabel("C Version Copyright (C) BioHazard 2005",this));
	vbox->addWidget(new QLabel("C# Version Copyright (C) QBasicer 2007",this));
	vbox->addWidget(new QLabel("Qt Version Copyright (C) Cody Harris 2007-2010",this));
	vbox->addWidget(new QLabel("http://zdlsharp.vectec.net",this));
	
	QLabel *pic = new QLabel(this);
	pic->setPixmap(QPixmap(aboutImg));
	
	hbox->addWidget(pic);
	hbox->addLayout(vbox);
	
	
	box->addWidget(new QLabel("Special thanks to BioHazard for the original version",this));
	box->addWidget(new QLabel("Special thanks to Risen, Enjay, DRDTeam.org, ZDoom.org",this));
	QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok,Qt::Horizontal,this);
	box->addWidget(btnBox);
	connect(btnBox, SIGNAL(accepted()), this, SLOT(close()));
}

void ZAboutDialog::close(){
	done(0);
}