#include <QtGui>
#include <QApplication>

#include "multiPane.h"


multiPane::multiPane(ZQWidget *parent): ZQWidget(parent){
	
	QHBoxLayout *box = new QHBoxLayout(this);

	QVBoxLayout *lbox = new QVBoxLayout();

	QVBoxLayout *rbox = new QVBoxLayout();
	box->addLayout(lbox);
	box->addLayout(rbox);
	box->setSpacing(0);
	lbox->setSpacing(0);
	rbox->setSpacing(0);

	lbox->addWidget(new QLabel("Game Mode", this));
	QComboBox *gMode = new QComboBox(this);
	gMode->addItem("Single-Player");
	lbox->addWidget(gMode);

	rbox->addWidget(new QLabel("Host IP/Address", this));
	QLineEdit *tHostAddy = new QLineEdit(this);
	rbox->addWidget(tHostAddy);

	QHBoxLayout *llbox = new QHBoxLayout();
	lbox->addLayout(llbox);
	llbox->setSpacing(0);
	
	QVBoxLayout *pbox = new QVBoxLayout();
	llbox->addLayout(pbox);
	pbox->addWidget(new QLabel("Players", this));
	QComboBox *noPlay = new QComboBox(this);
	noPlay->addItem("Joining");
	noPlay->addItem("1");
	noPlay->addItem("2");
	noPlay->addItem("3");
	noPlay->addItem("4");
	noPlay->addItem("5");
	noPlay->addItem("6");
	noPlay->addItem("7");
	noPlay->addItem("8");
	pbox->addWidget(noPlay);
	pbox->setSpacing(0);
	
	
	QVBoxLayout *fragbox = new QVBoxLayout();
	llbox->addLayout(fragbox);
	fragbox->addWidget(new QLabel("Frag Limit", this));
	QLineEdit *tFragLimit = new QLineEdit(this);
	fragbox->addWidget(tFragLimit);
	fragbox->setSpacing(0);
	

	QHBoxLayout *lrbox = new QHBoxLayout();
	rbox->addLayout(lrbox);
	lrbox->setSpacing(0);
	
	QVBoxLayout *dm1box = new QVBoxLayout();
	lrbox->addLayout(dm1box);
	dm1box->addWidget(new QLabel("DMFLAGS", this));
	QLineEdit *tDMFLAGS = new QLineEdit(this);
	dm1box->addWidget(tDMFLAGS);
	dm1box->setSpacing(0);
	
	QVBoxLayout *dm2box = new QVBoxLayout();
	lrbox->addLayout(dm2box);
	dm2box->addWidget(new QLabel("DMFLAGS2", this));
	QLineEdit *tDMFLAGS2 = new QLineEdit(this);
	dm2box->addWidget(tDMFLAGS2);
	dm2box->setSpacing(0);

	layout()->setContentsMargins(0,0,0,0);

}
