#ifndef _ZDLINTERFACE_H_
#define _ZDLINTERFACE_H_

#include <QtGui>
#include <QObject>
#include "ZQWidget.h"
#include "multiPane.h"


class zdlInterface: public ZQWidget{
Q_OBJECT
public: 
	zdlInterface( QWidget *parent=0);
	void startRead();
	void writeConfig();
private:
	multiPane *mpane;
};
#endif
