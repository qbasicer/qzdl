#ifndef _ZDMFLAGCHECKBOX_H_
#define _ZDMFLAGCHECKBOX_H_

#include <QtGui>
#include <QObject>
#include "ZQWidget.h"

class ZDMFlagCheckbox: public QCheckBox{
Q_OBJECT
public: 
	ZDMFlagCheckbox(int dmvalue, int highOn, QString text, QWidget *parent=0);
	int getValue();
	void setValue(int value);
private:
	int value;
	int logic;
};

#endif