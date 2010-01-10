#ifndef _ZDMFLAGMANAGER_H_
#define _ZDMFLAGMANAGER_H_

#include <QtCore>
#include <QObject>
#include "ZQWidget.h"
#include "ZDMFlagCheckbox.h"
#include <QVector>

class ZDMFlagManager: public QObject{
Q_OBJECT
	public: 
		ZDMFlagManager(QWidget *parent=0);
		void addCheckbox(ZDMFlagCheckbox* box);
		int getValue();
		void setValue(int value);
		void forceRecalc();
	signals:
		void valueChanged(int);
	public slots:
		void stateChanged(int);
	protected:
		QVector<ZDMFlagCheckbox*> checks;
};

#endif