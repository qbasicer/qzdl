#ifndef _ZABOUTDIALOG_H_
#define _ZABOUTDIALOG_H_

#include <QtGui>
#include <QObject>
#include "ZQWidget.h"

class ZAboutDialog: public QDialog{
Q_OBJECT
public: 
	ZAboutDialog(ZQWidget *parent=0);
	protected slots:
		void close();
		
};

#endif