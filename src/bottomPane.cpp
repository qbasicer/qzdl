#include <QtGui>
#include <QApplication>
#include <QMainWindow>

#include "bottomPane.h"
#include "buttonPane.h"

QLineEdit *extraArgs;

bottomPane::bottomPane(ZQWidget *parent): ZQWidget(parent){
	QVBoxLayout *box = new QVBoxLayout(this);
	QLabel *ecla = new QLabel("Extra command line arguments",this);
	extraArgs = new QLineEdit(this);
	buttonPane *pan = new buttonPane(this);
	layout()->setContentsMargins(0,0,0,0);
	setContentsMargins(0,0,0,0);
	box->setSpacing(2);
	box->addWidget(ecla);
	box->addWidget(extraArgs);
	box->addWidget(pan);
}
