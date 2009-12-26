#ifndef _ZADVANCEDMULTIPLAYERDIALOG_H_
#define _ZADVANCEDMULTIPLAYERDIALOG_H_

#include <QtGui>
#include <QObject>
#include "ZQWidget.h"

class ZAdvancedMultiplayerDialog: public QDialog{
Q_OBJECT
public: 
	ZAdvancedMultiplayerDialog(ZQWidget *parent=0);
private:
	QCheckBox *enable;
private slots:
	void save();
	void close();
};

#endif

