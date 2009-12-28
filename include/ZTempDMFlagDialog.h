#ifndef _ZTEMPDMFLAGDIALOG_H_
#define _ZTEMPDMFLAGDIALOG_H_

#include <QtGui>
#include <QObject>
#include "ZQWidget.h"

class ZTempDMFlagDialog: public QDialog{
Q_OBJECT
public: 
	ZTempDMFlagDialog(ZQWidget *parent=0);
	void setValue(int val);
	int value();
private:
	QLineEdit *flagVal;
private slots:
	void save();
	void close();
protected:
	int dmflags;
};

#endif

