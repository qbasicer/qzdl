#ifndef _ZDMFLAGDIALOG_H_
#define _ZDMFLAGDIALOG_H_

#include <QtGui>
#include <QObject>
#include "ZQWidget.h"
#include "ZDMFlagManager.h"

class ZDMFlagDialog: public QDialog{
Q_OBJECT
public: 
	ZDMFlagDialog(ZQWidget *parent=0);
	void setValue(int val);
	void setValue2(int val);
	int value();
	int value2();
private slots:
	void save();
	void close();
	void dmflagChanged(int value);
	void dmflag2Changed(int value);
		
protected:
	ZDMFlagManager *dmflag;
	ZDMFlagManager *dmflag2;
	QLineEdit *txtDMFlag;
	QLineEdit *txtDMFlag2;
};

#endif

