#include <QtGui>
#include <QApplication>

#include "multiPane.h"


multiPane::multiPane(ZQWidget *parent): ZQWidget(parent){
	QHBoxLayout *box = new QHBoxLayout(this);

	QVBoxLayout *lbox = new QVBoxLayout();

	QVBoxLayout *rbox = new QVBoxLayout();
	box->addLayout(lbox);
	box->addLayout(rbox);

	lbox->addWidget(new QLabel("Game Mode", this));
	QComboBox *gMode = new QComboBox(this);
	lbox->addWidget(gMode);

	rbox->addWidget(new QLabel("Host IP/Address", this));
	QLineEdit *tHostAddy = new QLineEdit(this);
	rbox->addWidget(tHostAddy);

	QHBoxLayout *llbox = new QHBoxLayout();
	lbox->addLayout(llbox);

	

	QHBoxLayout *lrbox = new QHBoxLayout();
	rbox->addLayout(lrbox);



}
