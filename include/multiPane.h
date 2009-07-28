#ifndef _MULTIPANE_H_
#define _MULTIPANE_H_

#include <QtGui>
#include <QObject>
#include "ZQWidget.h"

class multiPane: public ZQWidget{
Q_OBJECT
public: 
	multiPane( ZQWidget *parent=0);
private:
	QComboBox *gMode;
	QLineEdit *tHostAddy;
	QComboBox *gPlayers;
	QLineEdit *tFragLimit;
	QPushButton *bDMFlags;
	QPushButton *bDMFlags2;
};

#endif

